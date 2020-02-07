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
#pragma once
#include <JsonListener.h>
#include <JsonStreamingParser.h>

typedef struct DSForecast {
  String forecastIcon;
  long forecastTitle;
  String forecastLowTemp;
  String forecastHighTemp;
  String forecastDay;
  String forecastMonth;
  String forecastText;
  String forecastPressure;
  String forecastHumidity;
  String forecastPrecipTotal;
  String forecastWindDir;
  String forecastWindSpeed;
  String PoP;
} DSForecast;

typedef struct DarkSkyData {
  String currentTemp;
  String windSpeed;
  String windDir;
  String weatherIcon;
  String weatherText;
  String humidity;
  String pressure;
  String dewPoint;
  String precipitationToday;
  String feelslike;
  String UV;
  DSForecast *forecasts;
  String sunrise;
  String sunset;
} DarkSkyData;


class DarkSky : public JsonListener {
  private:
    boolean isMetric = true;
    String currentKey;
    int currentParent;
    DarkSkyData *data;
    const char* host = "api.darksky.net";
    const int httpsPort = 443;
    int maxForecasts;
    void getData();
    const int TODAYS_DATA = 1;
    const int FORECAST_DATA = 2;
    String TODAYS_DATA_KEY = "currently";
    String FORECAST_DATA_KEY = "daily";
    int forecastDay = -1;
    DSForecast dayData;
  public:
    DarkSky(int maxForecasts);
    void updateData(DarkSkyData *data, String apiKey, double latitude, double longitude);
    String getMeteoconIcon(String icon);
    virtual void whitespace(char c);

    virtual void startDocument();

    virtual void key(String key);

    virtual void value(String value);

    virtual void endArray();

    virtual void endObject();

    virtual void endDocument();

    virtual void startArray();

    virtual void startObject();
};
