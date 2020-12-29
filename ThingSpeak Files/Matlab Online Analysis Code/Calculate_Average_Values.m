% Read all input values over the past 15 minutes from a ThingSpeak channel and write 
% the average to another ThingSpeak channel. 

% Raw Data Channel ID
readChannelID = 1266016; 

% Raw Data Channel Read API Key 
readAPIKey = 'E61KMAQQHHGMFEBL'; 

% Smoothed Data Channel ID
writeChannelID = [1265995]; 
% Smoothed Data Write API Key
writeAPIKey = 'L4WF42RPFNKIQ3QY'; 
    

% Initialise Field ID variables to correspond to Raw Data Fields
outdoorHumidityFieldID = 1; 
outdoorTemperatureFieldID = 2; 
temperatureDifferenceFieldID = 3; 
indoorTemperatureFieldID = 4; 
indoorAirPollutionFieldID = 5; 
outdoorHumidityAPIFieldID = 6; 
outdoorTemperatureAPIFieldID = 7; 
temperatureDifferenceAPIFieldID = 8; 


%Extract last 15 minutes of data for each field into one variable
outdoorHumidity = thingSpeakRead(readChannelID,'Fields',outdoorHumidityFieldID,'NumMinutes',15,'ReadKey',readAPIKey);
outdoorTemperature = thingSpeakRead(readChannelID,'Fields',outdoorTemperatureFieldID,'NumMinutes',15,'ReadKey',readAPIKey);
temperatureDifference = thingSpeakRead(readChannelID,'Fields',temperatureDifferenceFieldID,'NumMinutes',15,'ReadKey',readAPIKey); 
indoorTemperature = thingSpeakRead(readChannelID,'Fields',indoorTemperatureFieldID,'NumMinutes',15,'ReadKey',readAPIKey); 
indoorAirPollution = thingSpeakRead(readChannelID,'Fields',indoorAirPollutionFieldID,'NumMinutes',15,'ReadKey',readAPIKey); 
outdoorHumidityAPI = thingSpeakRead(readChannelID,'Fields',outdoorHumidityAPIFieldID,'NumMinutes',15,'ReadKey',readAPIKey); 
outdoorTemperatureAPI = thingSpeakRead(readChannelID,'Fields',outdoorTemperatureAPIFieldID,'NumMinutes',15,'ReadKey',readAPIKey); 
temperatureDifferenceAPI = thingSpeakRead(readChannelID,'Fields',temperatureDifferenceAPIFieldID,'NumMinutes',15,'ReadKey',readAPIKey); 


%If the last 15mins worth of data for any of the fields in the raw data channel is empty (i.e. the hardware is not sending data to thingspeak/the power has been cut off, then don't upload this empty data to the smoothed data channel and instead send an error email
%This is done by making sure that all of the fields data from the last 15mins should not come up as empty (i.e. []). If any one comes up as empty, then the if statement is skipped and nothing is uploaded to the smoothed channel

if not(isempty(outdoorHumidity)) & not(isempty(outdoorTemperature)) & not(isempty(indoorTemperature)) & not(isempty(indoorAirPollution)) & not(isempty(outdoorHumidityAPI)) & not(isempty(outdoorTemperatureAPI))

    % Calculate the average values for each variable to get average of 15-minutes worth of data 
    avgOutdoorHumidity = mean(outdoorHumidity); 
    avgOutdoorTemperature = mean(outdoorTemperature); 
    avgTemperatureDifference = mean(temperatureDifference); 
    avgIndoorTemperature = mean(indoorTemperature); 
    avgIndoorAirPollution = mean(indoorAirPollution); 
    avgOutdoorHumidityAPI = mean(outdoorHumidityAPI); 
    avgOutdoorTemperatureAPI = mean(outdoorTemperatureAPI); 
    avgTemperatureDifferenceAPI = mean(temperatureDifferenceAPI); 
    
    %Plot new averaged data values to Smoothed Data channel
    thingSpeakWrite(writeChannelID,[avgOutdoorHumidity, avgOutdoorTemperature, avgTemperatureDifference, avgIndoorTemperature, avgIndoorAirPollution, avgOutdoorHumidityAPI, avgOutdoorTemperatureAPI, avgTemperatureDifferenceAPI],'WriteKey',writeAPIKey);
else
    %Sends an alert email if there has been no raw data send to ThingSpeak in the last 15mins, in case it is because some code/wiring is broken and needs to be checked or if the power to the board has been cut off
    alert_body = 'No raw data has been received in the last 15 minutes!';
    alert_subject = 'No raw data has been received in the last 15 minutes!';
    alert_api_key = 'TAKA9NZ9UFCCPNFK4V1C4';
    alert_url= "https://api.thingspeak.com/alerts/send";
    jsonmessage = sprintf(['{"subject": "%s", "body": "%s"}'], alert_subject,alert_body);
    options = weboptions("HeaderFields", {'Thingspeak-Alerts-API-Key', alert_api_key; 'Content-Type','application/json'});
    result = webwrite(alert_url, jsonmessage, options);
end