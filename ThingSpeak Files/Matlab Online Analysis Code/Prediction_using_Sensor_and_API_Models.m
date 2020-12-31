%This code will be triggered using 'React' when a set of 1-hour forecasted data is
%uploaded to the New Data Inputs Channel

%Load in sensorModel
dropboxUrl = strcat('https://www.dropbox.com/s/mppni3j0vgyamny/Sensor_Model.zip?dl=1');
websave('Sensor_Model',dropboxUrl);
unzip('Sensor_Model');
load('Sensor_Model.mat');

%Load in APIModel
dropboxUrl = strcat('https://www.dropbox.com/s/g7z518pkcq7rtql/API_Model.zip?dl=1');
websave('API_Model',dropboxUrl);
unzip('API_Model');
load('API_Model.mat');


% Channel ID to read new data from (New Data Inputs Channel)
InputChannelID = 1268592; 

% Channel ID to write new data to (Output Air Poll Predictions from New Data Channel)
OutputChannelID = 1268617; 

% Input data input Channel Read API Key 
readAPIKey = 'HVB07CCUTTHE87JQ';

% Output data Channel Write API Key (to upload calculated predicted air poll value to)
writeAPIKey = 'WVCB7HAVO0Q51XWK';


% Extract latest set of data from relevant sensor input fields from New Data Inputs Channel
newData = thingSpeakRead(InputChannelID,'Fields',[1 2 3 4 5 6],'NumPoints',1,'outputFormat','table', 'ReadKey',readAPIKey)

a1 = [newData.NewOutdoorHumidity] %Independent variable 1 new sensor input
b1 = [newData.NewOutdoorTemperature] %Independent variable 2 new sensor input
c1 = [newData.NewTemperatureDifference] %Independent variable 3 new sensor input
newy1 = feval(sensorModel,a1,b1,c1) %Dependent variable new air poll output

a2 = [newData.NewOutdoorHumidityAPI] %Independent variable 1 new api input
b2 = [newData.NewOutdoorTemperatureAPI] %Independent variable 2 new api input
c2 = [newData.NewTemperatureDifferenceAPI] %Independent variable 3 new api input
newy2 = feval(APIModel,a2,b2,c2) %Dependent variable new air poll output

%Writes the new pollution data to the data set's matching field and triggers another react to check if pollution level exceeds threshold or not
thingSpeakWrite(OutputChannelID,'Fields',[1 2],'Values',{newy1, newy2},'WriteKey',writeAPIKey);

