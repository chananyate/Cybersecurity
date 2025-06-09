from flask import Flask, render_template, request
import requests
from datetime import datetime

app = Flask(__name__)

API_KEY = 'a77c2df77fe02b4616496168c7da8f91'
BASE_URL = "https://api.openweathermap.org/data/2.5/forecast"

# This decorator tells Flask that the index() function should be executed whenever a user accesses the root URL (/) of the website
@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":  # Checks that the user has submitted the form
        location = request.form["location"]
        # Queries the openweathermap API to get the latitude and longitude for the entered location
        geocode_url = f"http://api.openweathermap.org/data/2.5/weather?q={location}&appid={API_KEY}"
        geo_response = requests.get(geocode_url)  # Makes a GET request to the API
        geo_data = geo_response.json()  # Using the requests library, this converts the json response into a python dictionary for easier manipulation

        # The API response includes a field "cod" which indicates the status code.
        if geo_data.get("cod") != 200:
            return "Location not found, please try again."

        # Extracts the latitude and longitude from the json response received from the openweathermap API
        lat = geo_data["coord"]["lat"]
        lon = geo_data["coord"]["lon"]
        # Fetches weather forecast data using the longitude and latitude
        forecast_url = f"http://api.openweathermap.org/data/2.5/forecast?lat={lat}&lon={lon}&units=metric&appid={API_KEY}"
        forecast_response = requests.get(forecast_url)
        forecast_data = forecast_response.json()

        location_name = geo_data["name"]  # Extracts the name of the location
        country = geo_data["sys"]["country"]  # Extracts the country code (like GB for Great Britain)

        # Groups data by day
        daily_data = {}  # Initializes an empty dictionary to store the weather data for each day. The keys are the dates and the values are the weather details for that day (temp, humidity etc.). Each weather detail (like temp) is itself a key with a corresponding value (like 13 degrees).
        for entry in forecast_data["list"]:  # Loops through all the forecast entries (3 hour intervals)
            dt_txt = entry["dt_txt"]  # Extracts the date and time field
            date = dt_txt.split(" ")[0]  # Extracts the date by splitting the string where there's a space and selecting the first part [0]
            time = dt_txt.split(" ")[1]  # Extracts the time

            # Stores weather data, once per day
            if date not in daily_data:
                daily_data[date] = {
                    "day_temp": None,  # Initially None until the loop reaches the 12 AM/PM entries
                    "night_temp": None,
                    "humidity": entry["main"]["humidity"],
                    "description": entry["weather"][0]["description"],
                    "icon": entry["weather"][0]["icon"],
                }

            # Assigns day and night temperatures
            if time == "12:00:00":  # Daytime temperature
                daily_data[date]["day_temp"] = entry["main"]["temp"]  # Retrieves the daytime temperature value from the "temp" field in the "main" part of the API response and stores it in the dict
            elif time == "00:00:00":  # Nighttime temperature
                daily_data[date]["night_temp"] = entry["main"]["temp"]

        weather = list(daily_data.items())[:8]  # items() converts the daily_data dict into a list of tuples so it can be easily looped over in the HTML. Slice 8 is used to limit the forecast to the first 8 items (days) if applicable

        # Renders the index.html file and passes the weather data along with location name and country to the template
        return render_template(
            "index.html", location_name=location_name, country=country, weather=weather
        )
    return render_template("index.html")  # If the request method is GET and not POST (the user initially visits the page without submitting a form it simply renders the index.html page without any weather data


if __name__ == "__main__":
    app.run(debug=True)
