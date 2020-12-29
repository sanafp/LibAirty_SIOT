%Obtaining Sensor Forecast data
%-------------------------------------------%

%Extract historical data from the last 24hrs to base prediction off of
CleanedDataChannelID = 1265995;
CleanedDataReadAPIKey = 'WFWRRRJXJ0EIG7T8';

historicalData = thingSpeakRead(CleanedDataChannelID,'Fields',[1,2,3,4,6,7,8],'NumDays',1,'OutputFormat','table','ReadKey',CleanedDataReadAPIKey);
sampleTime = 15; %the sample time (minutes) of the data we are using from Cleaned data (i.e. the time between each data point), so that the program can develop a time-accurate model seeing as its a time series model

%Use ar model to predict future indoor temp etc:

%Outdoor Humidity Model + 1hr Forecast value
IDdata = iddata(historicalData.OutdoorHumidity,[],sampleTime,'OutputName',{'Outdoor Humidity'},'TimeUnit','minutes');
trend = getTrend(IDdata,0);
IDdata = detrend(IDdata,0);
modelOrder = 8;
mdl = ar(IDdata,modelOrder);
Forecast_Sensor_outdoorHum4Steps = forecast(mdl,IDdata,4); %forecasting data 4 steps into the future as each step is 13mins so four steps = 1hr into the future
IDdata = retrend(IDdata,trend);
Forecast_Sensor_outdoorHum4Steps = retrend(Forecast_Sensor_outdoorHum4Steps,trend);
Forecast_Sensor_outdoorHum = Forecast_Sensor_outdoorHum4Steps.OutputData(4)

%Outdoor Temperature Model + 1hr Forecast value
IDdata = iddata(historicalData.OutdoorTemperature,[],sampleTime,'OutputName',{'Outdoor Temperature'},'TimeUnit','minutes');
trend = getTrend(IDdata,0);
IDdata = detrend(IDdata,0);
modelOrder = 8;
mdl = ar(IDdata,modelOrder);
Forecast_Sensor_outdoorTemp4Steps = forecast(mdl,IDdata,4); %forecasting data 4 steps into the future as each step is 13mins so four steps = 1hr into the future
IDdata = retrend(IDdata,trend);
Forecast_Sensor_outdoorTemp4Steps = retrend(Forecast_Sensor_outdoorTemp4Steps,trend);
Forecast_Sensor_outdoorTemp = Forecast_Sensor_outdoorTemp4Steps.OutputData(4)


%Indoor Temperature Model + 1hr Forecast value
IDdata = iddata(historicalData.IndoorTemperature,[],sampleTime,'OutputName',{'Indoor Temperature'},'TimeUnit','minutes');
trend = getTrend(IDdata,0);
IDdata = detrend(IDdata,0);
modelOrder = 8;
mdl = ar(IDdata,modelOrder);
Forecast_Sensor_indoorTemp4Steps = forecast(mdl,IDdata,4); %forecasting data 4 steps into the future as each step is 13mins so four steps = 1hr into the future
IDdata = retrend(IDdata,trend);
Forecast_Sensor_indoorTemp4Steps = retrend(Forecast_Sensor_indoorTemp4Steps,trend);
Forecast_Sensor_indoorTemp = Forecast_Sensor_indoorTemp4Steps.OutputData(4)

%Temperature Difference 1hr Forecast value
Forecast_Sensor_tempDiff = Forecast_Sensor_outdoorTemp - Forecast_Sensor_indoorTemp

%Obtaining API Forecast data
%-------------------------------------------%

key = '37bae3845c03df1250abefee8358cea7';
options = weboptions('ContentType','json');
url = ['https://api.openweathermap.org/data/2.5/onecall?lat=25.178624&lon=55.237260&units=metric&exclude=alerts,minutely,daily,current&APPID=',key];
Forecast_API_Data = webread(url, options);
%Extract the relevant data from the first row/item of the struct, as it contains the info for the first hour forecast
Forecast_API_outdoorHum = Forecast_API_Data.hourly(1).humidity
Forecast_API_outdoorTemp = Forecast_API_Data.hourly(1).temp
Forecast_API_tempDiff = Forecast_API_outdoorTemp - Forecast_Sensor_indoorTemp

OutputChannelID = 1268592;
OutputWriteAPIKey = 'WIDBWKWBNUU7OHDR';

thingSpeakWrite(OutputChannelID,'Fields',[1,2,3,4,5,6],'Values',{Forecast_Sensor_outdoorHum, Forecast_Sensor_outdoorTemp, Forecast_Sensor_tempDiff, Forecast_API_outdoorHum, Forecast_API_outdoorTemp, Forecast_API_tempDiff},'WriteKey',OutputWriteAPIKey);



%Fault checking code ---- we don't need this at the start. It's okay if blank values are uploaded to the channels and no previous values are used from either output or input channels to determine future values. Only previous values from the Cleaned data channel are used. So only in that case is it important that blank values are not uploaded. In this case, we just need to be made aware of any fault so it can be fixed
%-----------------------------------------------%

%Check Historical Sensor Data exists over last 24hours to correctly forecast 1hourly future sensor data and thus predict output pollution
if isempty(Forecast_Sensor_outdoorHum) || isempty(Forecast_Sensor_outdoorTemp) || isempty(Forecast_Sensor_indoorTemp)

    %Sends an alert email if there has been no raw data send to ThingSpeak in the last 15mins, in case it is because some code/wiring is broken and needs to be checked or the battery of the powerpack has died, as opposed to because we've actively removed the power source
    alert_body = 'No sensor data has been received in the last 24 hours!';
    alert_subject = "No sensor data has been received in the last 24 hours! We can't forecast new hourly sensor data and thus predict hourly air pollution levels!";
    alert_api_key = 'TAKA9NZ9UFCCPNFK4V1C4';
    alert_url= "https://api.thingspeak.com/alerts/send";
    jsonmessage = sprintf(['{"subject": "%s", "body": "%s"}'], alert_subject,alert_body);
    options = weboptions("HeaderFields", {'Thingspeak-Alerts-API-Key', alert_api_key; 'Content-Type','application/json'});
    result = webwrite(alert_url, jsonmessage, options);
    
end

%Check Forecast API Data is correctly accessed/accessible to thus predict output pollution
if isempty(Forecast_API_outdoorHum) || isempty(Forecast_API_outdoorTemp) || isempty(Forecast_Sensor_indoorTemp)

    %Sends an alert email if there has been no raw data send to ThingSpeak in the last 15mins, in case it is because some code/wiring is broken and needs to be checked or the battery of the powerpack has died, as opposed to because we've actively removed the power source
    alert_body = 'No API data or indoor temp sensor data has been received in the last 24 hours!';
    alert_subject = "No API data or indoor temp sensor data has been received in the last 24 hours! We can't forecast new temp diff data or access any of the forecast API data to thus predict hourly air pollution levels!";
    alert_api_key = 'TAKA9NZ9UFCCPNFK4V1C4';
    alert_url= "https://api.thingspeak.com/alerts/send";
    jsonmessage = sprintf(['{"subject": "%s", "body": "%s"}'], alert_subject,alert_body);
    options = weboptions("HeaderFields", {'Thingspeak-Alerts-API-Key', alert_api_key; 'Content-Type','application/json'});
    result = webwrite(alert_url, jsonmessage, options);
    
end

