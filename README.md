# LibAirty - DE4 SIOT Coursework - Sana Pirmohamed

A predictive air quality sensing and monitoring system using live and historical 
data to predict hourly indoor air pollution levels and prompt closing/opening of
windows.

Webapp: https://libairty.herokuapp.com/ 

Video Link: https://youtu.be/dEb5gJz3n9o


## Running The Code
1. Clone the repository: ```git clone https://github.com/sanafp/LibAirty_SIOT.git```

2. cd into the directory: ```cd LibAirty_SIOT```

3. Install all required packages: ```pip install -r requirements.txt```

4. Execute the Python Flask script: ```python main.py```

5. Then open a new web browser page, go to the url shown in terminal, the default is: *127.0.0.1:5000* or *http://localhost:5000/*


## Application Structure
* ```Sensing_and_Storing.ino``` contains the script running on the Arduino that takes in hardware sensor data as well as OpenWeatherMap.org data and uploads it to a ThingSpeak Channel *(Sensor Files > Sensing_and_Storing > Sensing_and_Storing.ino)*

* ```Data_24122020_30122020.xlsm``` contains 1 week's worth of sensor and OpenWeatherMap data extracted from ThingSpeak *(Sensor Files > database_24122020_30122020.xlsm)*

* *Matlab Online Analysis Folder* contains each piece of code used on ThingSpeak's Matlab to analyse and manipulate the stored data.

* *Matlab Models* contains the Matlab Desktop code for each of the Sensor and OpenWeatherMap API models, as well as the .mat files of each model and their respective zipped equivalents. These are called in one of the online Matlab Analysis scripts.