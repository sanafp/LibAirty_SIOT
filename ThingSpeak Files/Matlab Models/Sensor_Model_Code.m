%This program generates a Linear Regression Model and its co-efficients
%using existing weather sensor data and attempting to correlate it with matching indoor air pollution
%sensor values. The co-efficients will then be used with new ThinkgSpeak
%data to predict indoor pollution values

% Channel ID to read model data from (Smoothed Data)
readModelChannelID = 1265995; 

% Smoothed Data Channel Read API Key 
readModelAPIKey = 'WFWRRRJXJ0EIG7T8'; 

startDate = [2020,12,23,09,09,09];
endDate = [2020,12,30,23,58,54];

% Extract relevant fields from Cleaned Data channel, between 23rd Dec 09:09:09pm and 30th Dec 23:58:54pm
modelData = thingSpeakRead(readModelChannelID,'Fields',[1 2 3 5], 'DateRange',[datetime(startDate),datetime(endDate)], 'outputFormat','table', 'ReadKey',readModelAPIKey);
modelSpec = 'IndoorAirPollution ~ OutdoorHumidity + OutdoorTemperature + TemperatureDifference'; %Create a linear formula for 'modelspec' as a function of the three inputs

%Initial test for correlations
    %corrcoef(modelData{:,2},modelData{:,5})    %Outdoor Humidity vs Indoor air pollution
    %corrcoef(modelData{:,3},modelData{:,5})    %Outdoor Temperature vs Indoor air pollution
    %corrcoef(modelData{:,4},modelData{:,5})    %Temperature Difference vs Indoor air pollution

%Create final predictive model
sensorModel = fitlm(modelData,modelSpec) %Create a model using the relevant columns from modelData based on modelSpec


