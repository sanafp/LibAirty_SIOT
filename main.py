

import requests
from flask import Flask, render_template, url_for


app = Flask(__name__)


@app.route("/")
@app.route("/home")
def home():
	# ThingSpeak api-endpoint 
	OutputAirPollURL = "https://api.thingspeak.com/channels/1268617/feeds.json?results=1"
	CurrentAirPollURL = "https://api.thingspeak.com/channels/1265995/fields/5.json?results=1"
	  
	# HTTP Get Method and saving of data in variables
	r1 = requests.get(url = OutputAirPollURL)
	data1=r1.json()

	r2 = requests.get(url = CurrentAirPollURL)
	data2=r2.json()

	sensorOutputAirpollDataFloat=float(data1['feeds'][0]['field1'])
	sensorOutputAirpollData="{:.0f}".format(sensorOutputAirpollDataFloat)

	APIOutputAirpollDataFloat=float(data1['feeds'][0]['field2'])
	APIOutputAirpollData="{:.0f}".format(APIOutputAirpollDataFloat)

	currentAirpollDataFloat=float(data2['feeds'][0]['field5'])
	currentAirpollData="{:.0f}".format(currentAirpollDataFloat)


	if APIOutputAirpollDataFloat >= 500:
		APIWindowAction = "Close"
	else:
		APIWindowAction = "Open"

	if sensorOutputAirpollDataFloat >= 500:
		sensorWindowAction = "Close"
	else:
		sensorWindowAction = "Open"

	if currentAirpollDataFloat >= 500:
		currentAirQuote = '"Boy, is the air looking bad! Follow our instructions above!"'

	elif currentAirpollDataFloat >= 300:
		currentAirQuote = '"The air is not amazing, but it is definitely not bad!'
	else:
		currentAirQuote = '"Looking real good! Whatever it is your doing, keep it up!"'

	return render_template('home.html',title='LibAirty', route="/home", sensorOutputAirpollData=sensorOutputAirpollData, APIOutputAirpollData=APIOutputAirpollData, sensorWindowAction=sensorWindowAction, APIWindowAction=APIWindowAction, currentAirpollData=currentAirpollData, currentAirQuote=currentAirQuote)


@app.route("/about")
def about():
    return render_template('about.html', title='About', route="/about")


@app.route("/data")
def data():
    return render_template('data.html', title='View Live Data', route="/data")


  

if __name__ == '__main__':
    app.run(debug=True)