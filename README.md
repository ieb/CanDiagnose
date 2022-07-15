# Can Diagnose

Runs on a ESP32 with a webserver exposing an API to diagnose and monitor CanBus traffic.
Based on Datadisplay and DeviceAnalyser from the NMEA2000 libraries.


* [x] Setup Wifi
* [x] Setup MDNS registration
* [x] Implement webserver
* [x] Serve files from Flash (SPIFFS)
* [x] Implement data api /api/data/<n> where n == 0-10.
* [x] Expose set of standard CAN messages on /api/data
* [x] Implement simple UI in Javascript hosted on SPIFFS, targetting eink and normal browsers (based on SignalK eink)
* [x] Implement BPM280 source
* [x] Implement calibration mechanism using DAC
* [Fail] Implement ADC Source

The ESP32 ADC source cannot be made reliable enough or accurate enough to be used to monitor a battery. 
It is possible to get down to a 0.2% error with a callibrated lookup table, however the ADC mechamism (Sucessive Aproximation Register)
produced noisy values that are frequetly as much as +- 3%. At 12.6v this is 0.4v which is unworkable. Offsetting the voltage with a precision opamp so the ESP32 reads from 11 to 15v would reduce this to 0.15v, however this is still to large to be usable. Multi sampling helps generate a stable reading, however this requires 64 samples with a /5 moving average so over at least 60s. This does give and acceptable accuracy, but the speed of measuring the voltage is close to unworkable.

All attenuations were tried, and 1,2,4,16,32,64 clock divisions were tried to see if the rate of the SAR made any difference. It does not appear to change the results.

On that basis, I am giving up on using the ESP32 ADC for this purpose.

It still provides a good platform for a Wifi data server, and Can reader.

* [x] Redesign PCB to use 16bit ADC over i2c  (ADS1115)
* [x] Implement ADC sensor code.  
* [x] Implement admin interface.
* [x] Support configuration and calibration via web browser.
* [x] Support Configuraiton of single and differential ADC.
* [x] Add OLED display 
* [x] Add touch sensor to control oled display
* [x] Bench Calibrate expecialy shunt
* [x] Install test and calibrate. - Failed, the distance beween the board and the batteries is too great to get reliable shunt measurements.
* [x] Investigate using a shunt amplifier as used by VRA Alternator controll
* [x] Implement remote battery sensor using Modbus  (see BatterySensor project)
* [x] Reimplement PCB to have no BME280 and no ADS1115 replacing with a RS485 interface to ModBus devices.  (see CanPressure for Can based pressure, humidity, temperature sensor)
* [ ] Implement Modbus master

# Usage

The device will boot and connect to wifi controlled by its burned in config file (data/config.txt). 
There are 2 modes it can run in, a WiFi client or a AP. If its configured to be an AP, then join its network. If its a client find its ip. From now on this will be <ip>

Go to its ip on http://<ip> and you will see a data view of the data it is capturing.
Go to http://<ip>/admin.html and you will see a admin view with the ability to upload a new configuration file and reboot.

Both these views ae SPAs served from static files burned into the ESP32 Flash using APIs.

Connecting to the serial port monitor allows lower level control and diagnostics, enter 'h<CR>' to get help.

# Developing

Project uses PlatformIO in VSCode.  WebUI is in  ui/einkweb with build files added to data/ to be built into a flash image using ./buidui.sh

## PIO commands

because I alwaysforget.

* pio run -t upload
* pio device monitor

See buildui.sh for SPIFFS image commands.


