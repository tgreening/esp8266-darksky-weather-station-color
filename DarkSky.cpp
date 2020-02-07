/**The MIT License (MIT)

  Copyright (c) 2020 by Tony Greening

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/

//Based on the work of Daniel Einhorn -- adapted for DarkSky API


#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#include "DarkSky.h"

const char* fingerprint = "EA C3 0B 36 E8 23 4D 15 12 31 9B CE 08 51 27 AE C1 1D 67 2B";
DSForecast *tempForecasts;
DarkSky::DarkSky(int maxForecasts) {
  this->maxForecasts = maxForecasts;
}

void DarkSky::updateData(DarkSkyData *data, String apiKey, double latitude, double longitude) {
  this->data = data;
  tempForecasts = new DSForecast[this->maxForecasts];
  JsonStreamingParser parser;
  parser.setListener(this);
  int isBody = false;

  WiFiClientSecure secureClient;
  secureClient.setFingerprint(fingerprint);
  //Serial.printf("Before connection: %d", ESP.getFreeHeap());
  //Serial.println();
  secureClient.setBufferSizes(10240, 512);
  if (!secureClient.connect(host, httpsPort)) {
    //Serial.println("connection failed");
    return;
  }
  String url = "/forecast/" + apiKey + "/" + String(latitude) + "," + String(longitude) + "?exclude=[minutely,hourly,alerts,flags]";

  secureClient.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "User-Agent: BuildFailureDetectorESP8266\r\n" +
                     "Connection: close\r\n\r\n");

  while (secureClient.connected()) {
    String line = secureClient.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }

  while (secureClient.available()) {
    char c = secureClient.read();
    if (c == '{' || c == '[') {
      isBody = true;
    }
    if (isBody) {
      parser.parse(c);
    }
  }

  secureClient.stop();
  data->forecasts = tempForecasts;
}


void DarkSky::getData()  {
}

void DarkSky::whitespace(char c) {
  Serial.println("whitespace");
}

void DarkSky::startDocument() {
  //Serial.println("start document");
}

void DarkSky::key(String key) {
  currentKey = key;
  if (forecastDay < this->maxForecasts) {
    //Serial.println(key);
  }
  if (currentKey == TODAYS_DATA_KEY) {
    currentParent = TODAYS_DATA;
  } else if (currentKey == FORECAST_DATA_KEY) {
    currentParent = FORECAST_DATA;
  }
}

void DarkSky::value(String value) {
  if (currentKey == "time" && forecastDay < this->maxForecasts) {
    if (currentParent == FORECAST_DATA) {
      forecastDay++;
      //Serial.println(forecastDay);
      if (forecastDay > 0) {
        dayData.forecastDay = "1";
        dayData.forecastMonth = "1";
        tempForecasts[forecastDay - 1] = dayData;
        dayData = DSForecast();
      }
      dayData.forecastTitle = value.toInt();
    }
  }

  if (currentKey == "icon" && forecastDay < this->maxForecasts) {
    if (currentParent == FORECAST_DATA) {
      dayData.forecastIcon = value;
    } else if (currentParent == TODAYS_DATA) {
      data->weatherIcon = value;
    }
  }

  if (currentKey == "temperature" && forecastDay < this->maxForecasts) {
    if (currentParent == TODAYS_DATA) {
      data->currentTemp = String(value.toInt());
    }
  }

  if (currentKey == "summary" && forecastDay < this->maxForecasts) {
    if (currentParent == FORECAST_DATA) {
      dayData.forecastText = value;
    } else if (currentParent == TODAYS_DATA) {
      data->weatherText = value;
    }
  }

  if (currentKey == "windSpeed" && forecastDay < this->maxForecasts) {
    if (currentParent == TODAYS_DATA) {
      data->windSpeed = String(value.toInt()) + " mph";
      //Serial.println("windSpeed " + value);
    } if (currentParent == FORECAST_DATA) {
      dayData.forecastWindSpeed = String(value.toInt());
    }
  }

  if (currentKey == "windBearing" && forecastDay < this->maxForecasts) {
    if (currentParent == TODAYS_DATA) {
      data->windDir = String(value.toInt());
      //Serial.println("windBearing " + value);
    } if (currentParent == FORECAST_DATA) {
      dayData.forecastWindDir = String(value.toInt());
    }
  }

  if (currentKey == "humidity" && forecastDay < this->maxForecasts) {
    if (currentParent == TODAYS_DATA) {
      data->humidity = String((int)(value.toDouble() * 100));;
    } else if (currentParent == FORECAST_DATA) {
      dayData.forecastHumidity = String((int)(value.toDouble() * 100));
    }
  }

  if (currentKey == "pressure" && forecastDay < this->maxForecasts) {
    if (currentParent == TODAYS_DATA) {
      data->pressure = String(value.toInt()) + " mb" ;
      //Serial.println("pressure " + value);
    } else if (currentParent == FORECAST_DATA) {
      dayData.forecastPressure = String(value.toInt());
    }
  }

    if (currentKey == "precipAccumulation" && forecastDay < this->maxForecasts) {
    if (currentParent == FORECAST_DATA) {
      dayData.forecastPrecipTotal = String(value);
      //Serial.println("pressure " + value);
    }
  }


  if (currentKey == "apparentTemperature" && forecastDay < this->maxForecasts) {
    if (currentParent == TODAYS_DATA) {
      data->feelslike = String(value);
      //Serial.println("apparent " + value);
    }
  }

  if (currentKey == "uvIndex" && forecastDay < this->maxForecasts) {
    if (currentParent == TODAYS_DATA) {
      data->UV = String(value.toInt());
      //Serial.println("uvIndex " + value);
    }
  }

  if (currentKey == "dewPoint" && forecastDay < this->maxForecasts) {
    if (currentParent == TODAYS_DATA) {
      data->dewPoint = String(value.toInt());
      //Serial.println("dewPoint " + value);
    }
  }

  if (currentKey == "precipProbability" && forecastDay < this->maxForecasts) {
    if (currentParent == TODAYS_DATA) {
      data->precipitationToday = String((int)(value.toDouble() * 100));
    } else if (currentParent == FORECAST_DATA) {
      dayData.PoP = String((int)(value.toDouble() * 100));
    }
    //Serial.println(dayData.PoP + " : " + value);
  }

  if (currentKey == "temperatureMin" && forecastDay < this->maxForecasts) {
    if (currentParent == FORECAST_DATA) {
      dayData.forecastLowTemp = String(value.toInt());
      //Serial.println(dayData.forecastLowTemp + " : " + value);
    }
  }

  if (currentKey == "temperatureMax" && forecastDay < this->maxForecasts) {
    if (currentParent == FORECAST_DATA) {
      dayData.forecastHighTemp = String(value.toInt());
      //Serial.println(dayData.forecastHighTemp + " : " + value);
    }
  }

  if (currentKey == "sunriseTime" && forecastDay == 0) {
    if (currentParent == FORECAST_DATA) {
      time_t tSet = value.toInt() - 5 * 3600;
      struct tm * timeinfo = localtime (&tSet);
      char tmpTimeBuff[8];
      sprintf(tmpTimeBuff, "%02i:%02i %s", timeinfo->tm_hour > 12 ? timeinfo->tm_hour - 12 : timeinfo->tm_hour , timeinfo->tm_min, timeinfo->tm_hour > 11 ? "pm" : "am");
      data->sunrise = "Rise: " + String(tmpTimeBuff);
      //Serial.println(dayData.forecastHighTemp + " : " + value);
    }
  }

  if (currentKey == "sunsetTime" && forecastDay == 0) {
    if (currentParent == FORECAST_DATA) {
      time_t tSet = value.toInt() - 5 * 3600;
      struct tm * timeinfo = localtime (&tSet);
      char tmpTimeBuff[8];
      sprintf(tmpTimeBuff, "%02i:%02i %s", timeinfo->tm_hour > 12 ? timeinfo->tm_hour - 12 : timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_hour > 11 ? "pm" : "am");
      data->sunset = "Set:  " + String(tmpTimeBuff);
      //Serial.println(dayData.forecastHighTemp + " : " + value);
    }
  }

}

void DarkSky::endArray() {
}


void DarkSky::startObject() {
}

void DarkSky::endObject() {
}

void DarkSky::endDocument() {
}

void DarkSky::startArray() {
}

String DarkSky::getMeteoconIcon(String iconText) {
  if (iconText == "chanceflurries") return "F";
  if (iconText == "chancerain") return "Q";
  if (iconText == "chancesleet") return "W";
  if (iconText == "chancesnow") return "V";
  if (iconText == "chancetstorms") return "S";
  if (iconText == "flurries") return "F";
  if (iconText == "windy") return "E";
  if (iconText == "mostlycloudy") return "Y";
  if (iconText == "mostlysunny") return "H";
  if (iconText == "partly-cloudy-day") return "H";
  if (iconText == "partlysunny") return "J";


  if (iconText == "sleet") return "W";
  if (iconText == "rain") return "R";
  if (iconText == "snow") return "W";
  if (iconText == "sunny") return "B";
  if (iconText == "fog") return "M";
  if (iconText == "clear-day") return "B";
  if (iconText == "cloudy") return "Y";


  if (iconText == "partly-cloudy-night") return "4";


  if (iconText == "nt_chanceflurries") return "F";
  if (iconText == "nt_chancerain") return "7";
  if (iconText == "nt_chancesleet") return "#";
  if (iconText == "nt_chancesnow") return "#";
  if (iconText == "nt_chancetstorms") return "&";
  if (iconText == "clear-night") return "2";
  if (iconText == "nt_cloudy") return "Y";
  if (iconText == "nt_flurries") return "9";
  if (iconText == "nt_fog") return "M";
  if (iconText == "nt_hazy") return "E";
  if (iconText == "nt_mostlycloudy") return "5";
  if (iconText == "nt_mostlysunny") return "3";
  if (iconText == "nt_partlysunny") return "4";
  if (iconText == "nt_sleet") return "9";
  if (iconText == "nt_rain") return "7";
  if (iconText == "nt_snow") return "#";
  if (iconText == "nt_sunny") return "4";
  if (iconText == "nt_tstorms") return "&";

  return ")";
}
