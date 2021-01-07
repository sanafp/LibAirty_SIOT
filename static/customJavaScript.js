$(function(){
  var str = '#len'; //increment by 1 up to 1-nelemnts
  $(document).ready(function(){
    var i, stop;
    i = 1;
    stop = 4; //num elements
    setInterval(function(){
      if (i > stop){
        return;
      }
      $('#len'+(i++)).toggleClass('bounce');
    }, 500)
  });
});


function toggleFunctions(sensorOutputAirpollData,APIOutputAirpollData,sensorWindowAction,APIWindowAction) {
  var x = document.getElementById("toggleValue");
  var airpoll = document.getElementById("outputAirPollValue");
  var winAct = document.getElementById("windowActionValue");
  var airpollgauge = document.getElementById("airpollgauge");


  if (x.innerHTML === "Sensor-Based Information") {
    x.innerHTML = "OpenWeatherMapAPI-Based Information";
    airpoll.innerHTML = APIOutputAirpollData + ' PPM';
    winAct.innerHTML = APIWindowAction;
    airpollgauge.innerHTML = '<iframe width="450" height="260" style="border: 0px solid #cccccc; position: relative; left: -60px; top: 0px" src="https://thingspeak.com/channels/1268617/widgets/253478"></iframe> ';


  } else {
    x.innerHTML = "Sensor-Based Information";
    airpoll.innerHTML = sensorOutputAirpollData + ' PPM';
    winAct.innerHTML = sensorWindowAction;
    airpollgauge.innerHTML = '<iframe width="450" height="260" style="border: 0px solid #cccccc; position: relative; left: -60px; top: 0px" src="https://thingspeak.com/channels/1268617/widgets/253477"></iframe> ';

  }
}


function rawgraphchangeFunction() {
  var rawV = document.getElementById("rawfields");
  var rawC = document.getElementById("rawchart");

  
  if (rawV.value === "IndoorAirPoll") {
    rawC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1266016/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (rawV.value === "SensorOutdoorHum") {
    rawC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1266016/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (rawV.value === "SensorOutdoorTemp") {
    rawC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1266016/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (rawV.value === "SensorTempDiff") {
    rawC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1266016/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (rawV.value === "SensorIndoorTemp") {
    rawC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1266016/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (rawV.value === "APIOutdoorHum") {
    rawC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1266016/charts/6?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (rawV.value === "APIOutdoorTemp") {
    rawC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1266016/charts/7?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (rawV.value === "APITempDiff") {
    rawC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1266016/charts/8?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

}


function smoothedgraphchangeFunction() {
  var smoothedV = document.getElementById("smoothedfields");
  var smoothedC = document.getElementById("smoothedchart");

  
  if (smoothedV.value === "IndoorAirPoll") {
    smoothedC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1265995/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (smoothedV.value === "SensorOutdoorHum") {
    smoothedC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1265995/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (smoothedV.value === "SensorOutdoorTemp") {
    smoothedC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1265995/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (smoothedV.value === "SensorTempDiff") {
    smoothedC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1265995/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (smoothedV.value === "SensorIndoorTemp") {
    smoothedC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1265995/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (smoothedV.value === "APIOutdoorHum") {
    smoothedC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1265995/charts/6?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (smoothedV.value === "APIOutdoorTemp") {
    smoothedC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1265995/charts/7?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (smoothedV.value === "APITempDiff") {
    smoothedC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1265995/charts/8?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

}


function predictinputgraphchangeFunction() {
  var predictinputV = document.getElementById("predictinputfields");
  var predictinputC = document.getElementById("predictinputchart");

  
  if (predictinputV.value === "SensorOutdoorHum") {
    predictinputC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1268592/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (predictinputV.value === "SensorOutdoorTemp") {
    predictinputC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1268592/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (predictinputV.value === "SensorTempDiff") {
    predictinputC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1268592/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (predictinputV.value === "APIOutdoorHum") {
    predictinputC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1268592/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (predictinputV.value === "APIOutdoorTemp") {
    predictinputC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1268592/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (predictinputV.value === "APITempDiff") {
    predictinputC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1268592/charts/6?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

}


function predictoutputgraphchangeFunction() {
  var predictoutputV = document.getElementById("predictoutputfields");
  var predictoutputC = document.getElementById("predictoutputchart");

  
  if (predictoutputV.value === "PredictSensorAirPoll") {
    predictoutputC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1268617/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

  else if (predictoutputV.value === "PredictAPIAirPoll") {
    predictoutputC.innerHTML='<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1268617/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&type=line&update=15"></iframe>';
  } 

}