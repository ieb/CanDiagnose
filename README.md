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
* [x] Implement Modbus master


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

# Connectors


    ---------------------------------------------
    |           a b c d e f g h                 |
    |  A                                        |
    |  B                                        |
    |  C                                        |
    |  D                                     H  |
    |  E                                     I  |
    |  F                                     J  |
    |  G           i j k l m                 K  |
    ---------------------------------------------

    a  SPI BL   GPIO13
    b  SPI RST  GPIO12
    c  SPI DC   GPIO26
    d  SPI CS   GPIO25
    e  SPI SCK  GPIO33
    f  SPI MOSI GPIO32
    g  SPI GND  
    h  SPI 3V    
    i  i2c GND      Display black/blue
    j  i2c SCL  D5    Display green
    k  i2c SDA  D18    Display white
    l  i2c BTN  D19    Display yellow
    m  i2c 3V       Display red

    A  1wire 1w  D21
    B  1wire GND
    C  1wire 3V
    D  RS-485 5.8V
    E  RS-485 GND
    F  RS-485 A 
    G  RS-485 B
    H  CAN 12V
    I  CAN 0V
    J  CAN CANH
    K  CAN CANL


    Other Pins Not mentioned above
    RS485-TX TX2/GPIO17
    RS485-RX RX2/GPIO16
    RS485-EN D4
    CAN-RX  D22
    CAN-TX  D23


# eInk Waveshare display

This uses SPI output only with a bunch of additional pins.

    eInk DIN <-  f  SPI MOSI GPIO32
    eInk CLK <-  e  SPI SCK  GPIO33
    eInk CS  <-  d  SPI CS   GPIO25
    eInk DC  <-  c  SPI DC   GPIO26
    eInk RST <-  b  SPI RST  GPIO12
    eInk BUSY -> a  SPI BL   GPIO13

 

# 4 inch TFT display 480x320 24 bit colour driven by a ILI9488 driver.

uses SPI + a PWM blacklight. Control via a TPP233 Touch switch. Library is TFT_eSPI library. Pin mappings defined as compile definitions.

    -D TFT_MISO=GPIO_NUM_35
    -D TFT_MOSI=GPIO_NUM_32
    -D TFT_SCLK=GPIO_NUM_33
    -D TFT_CS=GPIO_NUM_25
    -D TFT_DC=GPIO_NUM_26
    -D TFT_RST=GPIO_NUM_27
    -D TFT_PWM_BL=GPIO_NUM_14
    -D ILI9488_DRIVER

The display sleeps after 60s of inactivity to reduce power drain, but turning off the backlight. Sleeping the IMI9488 driver has not been done as of yet.  Pages are implemented as classes using widgets. When a page is selected it is allocated into heap and does not exist while not displaying. On other drivers here all pages were compiled in, however the RAM usage is higher on account of full colour and display size. 

In most cases attempts to update the screen, on screen causes flickering so double buffering of updates is done using sprits with DMA transfers from the sprite to the screen. Images for screen are stored in jpg on flash, consuming about 30KB per screen.

Drawing to sprites also works in 1bpp, 4bpp or 16bpp.

## TFT case

3d printed case, with 2 touch sensors and round shielded cable, as ribbon will emit too much interference to nearby devices. 

Wires

|| color || designation || ESP32 Pin ||
---------------------------------------
| Red    |  5v          | 5v           |
| black  |  0v          | 0v           |
| pink   |  CS          | GPIO_NUM_25  |
| cyan   |  Reset       | GPIO_NUM_27  |
| white  |  DC/RS       | GPIO_NUM_26  |
| blue   |  SDI(MOSI)   | GPIO_NUM_32  |
| green  |  SCK         | GPIO_NUM_33  |
| grey   |  LED         | GPIO_NUM_14  |
| Purple |  SDO(MISO)   | GPIO_NUM_35  |
| Orange |  3.3v        | 3.3v         |
| Brown  | Touch Lower  | GPIO_NUM_19  |
| Yellow | Touch Top    | TBD          |

Due to the additional touch pins a fresh PCB is probably needed.


