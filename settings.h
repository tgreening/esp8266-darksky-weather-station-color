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

#include <simpleDSTadjust.h>

// Setup
#define WIFI_HOSTNAME "DarkSky-weather-station-color"
#define WIFI_SSID "SSID"
#define WIFI_PASS  "PASSWORD"

const int UPDATE_INTERVAL_SECS = 20 * 60; // Update every 10 minutes
const int SLEEP_INTERVAL_SECS = 0;        // Going to sleep after idle times, set 0 for insomnia

const String DISPLAYED_CITY_NAME = "CITY";
const String DARKSKY_API_KEY = "API KEY";
const double LATITUDE = 0;
const double LONGITUDE = 0;

const uint8_t MAX_FORECASTS = 7;

// Adjust according to your language
const String WDAY_NAMES[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
const String MONTH_NAMES[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
const String SUN_MOON_TEXT[] = {"Sun", "Rise", "Set", "Moon", "Age", "Illum"};
const String MOON_PHASES[] = {"New Moon", "Waxing Crescent", "First Quarter", "Waxing Gibbous",
                              "Full Moon", "Waning Gibbous", "Third quarter", "Waning Crescent"
                             };
#define UTC_OFFSET -5
//US Eastern Time Zone (New York, Boston)
struct dstRule StartRule = {"EDT", Second, Sun, Mar, 2, 3600};    // Daylight time = UTC/GMT -4 hours
struct dstRule EndRule = {"EST", First, Sun, Nov, 2, 0};          // Standard time = UTC/GMT -5 hour

// Settings for Boston
// #define UTC_OFFSET -5
// struct dstRule StartRule = {"EDT", Second, Sun, Mar, 2, 3600}; // Eastern Daylight time = UTC/GMT -4 hours
// struct dstRule EndRule = {"EST", First, Sun, Nov, 1, 0};       // Eastern Standard time = UTC/GMT -5 hour

// values in metric or imperial system?
const boolean IS_METRIC = false;

// Change for 12 Hour/ 24 hour style clock
bool IS_STYLE_12HR = true;

// change for different NTP (time servers)
#define NTP_SERVERS "0.ch.pool.ntp.org", "1.ch.pool.ntp.org", "2.ch.pool.ntp.org"
// #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"

// August 1st, 2018
#define NTP_MIN_VALID_EPOCH 1533081600

// Pins for the ILI9341
#define TFT_DC D8
#define TFT_CS D0
#define TFT_LED D1

#define HAVE_TOUCHPAD
#define TOUCH_CS D3
#define TOUCH_IRQ -1


/***************************
   End Settings
 **************************/
