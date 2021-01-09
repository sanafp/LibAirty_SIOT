
//------- Initialisation -------//

//Libraries
#include <SPI.h>
#include <WiFiNINA.h>
#include <stdio.h>
#include <ArduinoJson.h>
#include "secrets.h"
#include <SimpleDHT.h>
#include <MQ135.h>

//API Key to connect to OpenWatherMap.org and obtain current weather data
char vOpenWeatherApiKey[] = SECRET_OPENWEATHER_APIKEY;
String OpenWeatherApiKey = String(vOpenWeatherApiKey);

//API Key to connect to correct ThingSpeak channel
char vThingSpeakApiKey[] = SECRET_WRITE_APIKEY;
String ThingSpeakApiKey = String(vThingSpeakApiKey);

//Sensor definitions
#define OtdrHumTempSensor A0
#define IndrHumTempSensor A4
MQ135 IndrAirPollSensor = MQ135(A5);
SimpleDHT11 dht1(OtdrHumTempSensor);
SimpleDHT11 dht2(IndrHumTempSensor);

//Variable declarations
float OtdrHumidityValue;
float OtdrTempValue;
float DiffTempValue;
float IndrTempValue;
float IndrAirPollValue;
float OtdrHumidityAPIValue;
float OtdrTempAPIValue;
float DiffTempAPIValue;
float IndrHumidityValue; //Won't store its data, but need it for the dht.h program to work correctly

//Local Wireless Router Information
char ssid[] = SECRET_SSID;        // Network SSID (name)
char pass[] = SECRET_PASS;    // Network password

int status = WL_IDLE_STATUS;

// Initialize the Wifi client library
WiFiSSLClient client;

//Data capture sample rate
int samplingRate = 300000; //sample data every 5 mins = 300000 milliseconds to then average within the window of captured data
//So we capture data every 5 mins for 15 mins. So we get 3 sets of data. We then average it to get one set of data to upload after 15 minutes



void setup()
{  

  Serial.begin(9600);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) 
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") 
  {
    Serial.println("Upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network.
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // We're connected now, so print out the wifi status and information:
  printWifiStatus();

  //Give the air pollution sensor enough time to warm up so that it can take accurate readings right from the start
  Serial.println("Warming up Air Pollution Sensor...");
  delay(20000);
}

void loop() 
{    
  //Gathers weather data every x seconds (x = sampling Rate) and posts to ThingSpeak

    //temp is in C and humdity is in %
    dht1.read2(&OtdrTempValue, &OtdrHumidityValue, NULL); //Captures outdoor humidity and temperature readings
    dht2.read2(&IndrTempValue, &IndrHumidityValue, NULL); //Captures indoor humidity and temperature readings (but only temperature value will be used going forward)

    DiffTempValue = OtdrTempValue - IndrTempValue; //Difference in temperature between outdoors and indoors (using sensors) is calculated
    IndrAirPollValue = IndrAirPollSensor.getPPM(); //Captures indoor air pollution level in PPM
    getWeather(); //Triggers function to capture current weather data from OpenWeatherMap API
    DiffTempAPIValue = OtdrTempAPIValue - IndrTempValue; //Difference in temperature between outdoors (API value) and indoors (sensor value) is calculated

    //Printed for debugging purposes
    Serial.println("OtdrHumidity: " + String(OtdrHumidityValue));
    Serial.println("OtdrTemp: " + String(OtdrTempValue)+ " C") ;
    Serial.println("DiffTemp: " + String(DiffTempValue)+ " C");
    Serial.println("IndrTemp: " + String(IndrTempValue)+ " C");
    Serial.println("IndrAirPoll: " + String(IndrAirPollValue) + " PPM");
    Serial.println("OtdrHumidityAPI: " + String(OtdrHumidityAPIValue));
    Serial.println("OtdrTempAPI: " + String(OtdrTempAPIValue)+ " C") ;
    Serial.println("DiffTempAPI: " + String(DiffTempAPIValue)+ " C");

    //Triggers function to send data to Raw Data ThingSpeak Channel
    sendThingPost();

    //Delays next loop by specified sampling rate to avoid excessive or sparse capturing of data
    delay(samplingRate);
}

void getWeather()

//--------- Function to capture weather data from OpenWeatherMap API ---------//

{    
  //If there's a successful connection:
  if (client.connect("api.openweathermap.org", 443)) 
  {
    Serial.println("Connecting to OpenWeatherMap...");
    // send the HTTP GET request:
    client.println(String("GET /data/2.5/onecall?lat=25.178624&lon=55.237260&units=metric&exclude=alerts,minutely,daily,hourly&appid=" + OpenWeatherApiKey + " HTTP/1.1")); //API is localised (using lat-lon coords) to home containing sensors, and is highly specific to exclude excess data and reduce size of extracted JSON file
    client.println("Host: api.openweathermap.org");
    client.println("Connection: close");
    client.println();

    //Check HTTP status
    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    //It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
    if (strcmp(status + 9, "200 OK") != 0) 
    {
      Serial.print(F("Unexpected response: "));
      Serial.println(status);
      return;
    }

    //Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders)) 
    {
      Serial.println(F("Invalid response"));
      return;
    }

    //Allocate the JSON document
    //Use arduinojson.org/v6/assistant to compute the capacity.
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(0) + 2*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(14) + 280;
    DynamicJsonDocument doc(capacity);
    
    //Parse JSON object
    DeserializationError error = deserializeJson(doc, client);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    //Extract required values from JSON file
    OtdrTempAPIValue = doc["current"]["temp"].as<float>(); //Stores outdoor humidity reading from OpenWeatherMap
    OtdrHumidityAPIValue = doc["current"]["humidity"].as<float>(); //Stores outdoor temperature reading from OpenWeatherMap
    Serial.println("Succcessfully grabbed info from OpenWeatherMap!");

    
    //Disconnect
    client.stop();
    
  } 
  else 
  {
    //If a connection could not be made:
    Serial.println("Connection failed");
  }
}

void sendThingPost()

//--------- Function to send data to Raw Data ThingSpeak Channel ---------//

{    
  //If there's a successful connection:
  if (client.connect("api.thingspeak.com", 443)) 
  {
    Serial.println("Connecting to ThingSpeak...");
   
    //Create a message containing each value to store in ThingSpeak as well as which field it will be stored
    String msg = "&field1=" + String(OtdrHumidityValue) + "&field2=" + String(OtdrTempValue) + "&field3=" + String(DiffTempValue) + "&field4=" + String(IndrTempValue) + "&field5=" + String(IndrAirPollValue) + "&field6=" + String(OtdrHumidityAPIValue) + "&field7=" + String(OtdrTempAPIValue) + "&field8=" + String(DiffTempAPIValue) ; //field number and corresponding sensor value to be uploaded are combined as required by ThingSpeak
    
    //Send the HTTP PUT request:
    client.println(String("GET /update?api_key=" + ThingSpeakApiKey + msg + " HTTP/1.1"));
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println();
    Serial.println("Successfully stored in ThingSpeak!");


    //Check HTTP status
    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    
    // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
    if (strcmp(status + 9, "200 OK") != 0) 
    {
      Serial.print(F("Unexpected response: "));
      Serial.println(status);
      return;
    }   

  } 
  else 
  {
    //If a connection could not be made:
    Serial.println("Connection failed");
  }
}


void printWifiStatus() 
{
  //Print the SSID of the network the board is connected to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  //Print the board's IP address:
  IPAddress ip_Address = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip_Address);

  //Print the received signal strength:
  long rssi_value = WiFi.RSSI();
  Serial.print("Received Signal strength:");
  Serial.print(rssi_value);
  Serial.println(" dBm");
}
