# esp8266-darksky-weather-station-color
An adaption of the esp8266-weather-station-color by Daniel Einhorn for the DarkSKY API.

See [https://github.com/ThingPulse/esp8266-weather-station-color](https://github.com/ThingPulse/esp8266-weather-station-color) for details on hos excellent work.

### Alterations
This project will show you daily forecast rather than every 12 hours.

I also removed the moon phases for more data about today. 

## Getting started

### Hardware

I **love** Wemos D1 Chips!  They are so easy to use!! 
I buy them from the offical Lolin(Wemos) store on AliExpress.
HEre's the Wemos D1 Min board [here](https://www.aliexpress.com/item/32529101036.html?spm=a2g0s.9042311.0.0.27424c4dulLe7M)
Here's the 2.4" TFT LCD [here](https://www.aliexpress.com/item/32919729730.html?spm=a2g0s.9042311.0.0.27424c4d83MVWp)

Look at the bottom of the LCD so you solder the connector on the Wemos witht he correct positioning so they match the LCD.

## Coding

Download Daniel Einhorn's project - you need the reference libraries.

Go to [DarkSky](https://darksky.net/dev) to create an API key.

### Weather Station Color project

#### Obtain the code

The WiFi Color DarkSky Kit project is publicly accessible on GitHub. Hence, there are two options to download the code:

Clone the repository with Git: git clone [https://github.com/tgreening/esp8266-darksky-weather-station-color](https://github.com/tgreening/esp8266-darksky-weather-station-color)

Download the sources from [https://github.com/tgreening/esp8266-darksky-weather-station-color/archive/master.zip](https://github.com/tgreening/esp8266-darksky-weather-station-color/archive/master.zip) and unpack them somewhere to your local file system as esp8266-darksky-weather-station-color (erase '-master' in the folder name).

### Open project in Arduino IDE

#### Start the Arduino IDE
File > Open
find and select esp8266-darksky-weather-station-color.ino from the previous step.

#### Configuration & customization

In one of the tabs the IDE opened settings.h. Go through the file and adjust the two handful of configuration parameters. They are all documented inside the file directly. Everything should be self-explanatory. Most importantly you will need to set the DArkSkyAPI key you obtained in a previous step.

#### Upload code to device

The very last step really is to upload the code and all its dependencies to the device from the Arduino IDE. You do this through CTRL + U (⌘ + U on macOS) or Sketch > Upload or the right-arrow icon in the toolbar.