//Libraries
#include <SPI.h>
#include <WiFiNINA.h>
#include <stdio.h>
#include <ArduinoJson.h>
#include "secrets.h"
#include <SimpleDHT.h>
#include <MQ135.h>

//API Key to connect to correct ThingSpeak channel
char vThingSpeakApiKey[] = SECRET_WRITE_APIKEY;
String ThingSpeakApiKey = String(vThingSpeakApiKey);

//Sensor definitions
#define OtdrHumTempSensor A0
#define IndrHumTempSensor A4
MQ135 IndrAirPollSensor = MQ135(A5);
SimpleDHT11 dht1(OtdrHumTempSensor);
SimpleDHT11 dht2(IndrHumTempSensor);

//variable declarations
float OtdrHumidityValue;
float OtdrTempValue;
float DiffTempValue;
float IndrTempValue;
float IndrAirPollValue;
float OtdrHumidityAPIValue;
float OtdrTempAPIValue;
float DiffTempAPIValue;
float IndrHumidityValue; //Won't use it, but need it for the dht.h program to work correctly

//Local Wireless Router Information
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;

// Initialize the Wifi client library
WiFiSSLClient client;

int samplingRate = 300000; //sample data every 5 mins = 300000 milliseconds to then average within the window of captured data
//So we capture data every 5 mins for 15 mins. So we get 3 sets of data. We then average it to get one set of data to upload after 15minutes
//60000*5, 60000 (final values, so we save data to thingspeak every 5mins, but capture data every minute to avergae out after 5mins and then upload)

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
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // you're connected now, so print out the status:
  printWifiStatus();

  Serial.println("Warming up Air Pollution Sensor...");
  delay(20000);
}

void loop() 
{    
  //Gathers sensor data every x seconds (x = sampling Rate)and posts to ThingSpeak
  
    dht1.read2(&OtdrTempValue, &OtdrHumidityValue, NULL); //temp is in C and humdity is in %
    dht2.read2(&IndrTempValue, &IndrHumidityValue, NULL);

    DiffTempValue = OtdrTempValue - IndrTempValue;
    IndrAirPollValue = IndrAirPollSensor.getPPM();
    getWeather();
    DiffTempAPIValue = OtdrTempAPIValue - IndrTempValue;
    
    Serial.println("OtdrHumidity: " + String(OtdrHumidityValue));
    Serial.println("OtdrTemp: " + String(OtdrTempValue)+ " C") ;
    Serial.println("DiffTemp: " + String(DiffTempValue)+ " C");
    Serial.println("IndrTemp: " + String(IndrTempValue)+ " C");
    Serial.println("IndrAirPoll: " + String(IndrAirPollValue) + " PPM");
    Serial.println("OtdrHumidityAPI: " + String(OtdrHumidityAPIValue));
    Serial.println("OtdrTempAPI: " + String(OtdrTempAPIValue)+ " C") ;
    Serial.println("DiffTempAPI: " + String(DiffTempAPIValue)+ " C");



    sendThingPost();
    delay(samplingRate);
}

void getWeather()
{    
  // if there's a successful connection:
  if (client.connect("api.openweathermap.org", 443)) 
  {
    Serial.println("Connecting to OpenWeatherMap...");
    // send the HTTP PUT request:
    client.println("GET /data/2.5/onecall?lat=25.178624&lon=55.237260&units=metric&exclude=alerts,minutely,daily,hourly&appid=37bae3845c03df1250abefee8358cea7 HTTP/1.1");
    client.println("Host: api.openweathermap.org");
    client.println("Connection: close");
    client.println();

    // Check HTTP status
    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
    if (strcmp(status + 9, "200 OK") != 0) 
    {
      Serial.print(F("Unexpected response: "));
      Serial.println(status);
      return;
    }

    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders)) 
    {
      Serial.println(F("Invalid response"));
      return;
    }

    // Allocate the JSON document
    // Use arduinojson.org/v6/assistant to compute the capacity.
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(0) + 2*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(14) + 280;
    DynamicJsonDocument doc(capacity);
    
    // Parse JSON object
    DeserializationError error = deserializeJson(doc, client);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
        
    OtdrTempAPIValue = doc["current"]["temp"].as<float>();
    OtdrHumidityAPIValue = doc["current"]["humidity"].as<float>();
    
    //Disconnect
    client.stop();
    
  } 
  else 
  {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}

void sendThingPost()
{    
  // if there's a successful connection:
  if (client.connect("api.thingspeak.com", 443)) 
  {
    Serial.println("Connecting to ThingSpeak...");
    String msg = "&field1=" + String(OtdrHumidityValue) + "&field2=" + String(OtdrTempValue) + "&field3=" + String(DiffTempValue) + "&field4=" + String(IndrTempValue) + "&field5=" + String(IndrAirPollValue) + "&field6=" + String(OtdrHumidityAPIValue) + "&field7=" + String(OtdrTempAPIValue) + "&field8=" + String(DiffTempAPIValue) ; //field number and corresponding sensor value to be uploaded are combined as required by ThingSpeak
    // send the HTTP PUT request:
    client.println(String("GET /update?api_key=" + ThingSpeakApiKey + msg + " HTTP/1.1"));
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println();

    // Check HTTP status
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
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}


void printWifiStatus() 
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
