EESchema Schematic File Version 2
LIBS:ESP32CanBoard-rescue
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:ESP32CanBoard-cache
EELAYER 25 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L GND #PWR01
U 1 1 6102626D
P 4850 1950
F 0 "#PWR01" H 4850 1950 30  0001 C CNN
F 1 "GND" H 4850 1880 30  0001 C CNN
F 2 "" H 4850 1950 60  0001 C CNN
F 3 "" H 4850 1950 60  0001 C CNN
	1    4850 1950
	1    0    0    -1  
$EndComp
$Comp
L DIODE D203
U 1 1 61026462
P 4850 1550
F 0 "D203" H 4850 1650 40  0000 C CNN
F 1 "SMBJ30CA-TR TSV" H 4850 1450 40  0000 C CNN
F 2 "Diodes_SMD:DO-214AB" H 4850 1550 60  0001 C CNN
F 3 "https://uk.rs-online.com/web/p/tvs-diodes/7147197" H 4850 1550 60  0001 C CNN
	1    4850 1550
	0    -1   -1   0   
$EndComp
$Comp
L CAPAPOL C205
U 1 1 61053A5B
P 5350 1600
F 0 "C205" H 5400 1700 40  0000 L CNN
F 1 "100uF" H 5400 1500 40  0000 L CNN
F 2 "Capacitors_SMD:C_1210" H 5450 1450 30  0001 C CNN
F 3 "" H 5350 1600 300 0000 C CNN
	1    5350 1600
	1    0    0    -1  
$EndComp
$Comp
L C C206
U 1 1 61053BCD
P 6750 1600
F 0 "C206" H 6750 1700 40  0000 L CNN
F 1 "1uF" H 6756 1515 40  0000 L CNN
F 2 "Capacitors_SMD:C_1210" H 6788 1450 30  0001 C CNN
F 3 "" H 6750 1600 60  0000 C CNN
	1    6750 1600
	1    0    0    -1  
$EndComp
$Comp
L DIODE D204
U 1 1 61053C97
P 5150 1550
F 0 "D204" H 5150 1650 40  0000 C CNN
F 1 "1N5819" H 5150 1450 40  0000 C CNN
F 2 "Diodes_SMD:D_1206" H 5150 1550 60  0001 C CNN
F 3 "" H 5150 1550 60  0000 C CNN
	1    5150 1550
	0    -1   -1   0   
$EndComp
Text GLabel 7750 1150 2    60   Output ~ 0
5V
$Comp
L GND #PWR02
U 1 1 610553E6
P 6050 2050
F 0 "#PWR02" H 6050 2050 30  0001 C CNN
F 1 "GND" H 6050 1980 30  0001 C CNN
F 2 "" H 6050 2050 60  0001 C CNN
F 3 "" H 6050 2050 60  0001 C CNN
	1    6050 2050
	1    0    0    -1  
$EndComp
Text GLabel 3100 1150 0    60   Input ~ 0
12v
Wire Wire Line
	4850 1950 4850 1750
Wire Wire Line
	4850 1350 4850 1150
Wire Wire Line
	3100 1150 4100 1150
Wire Wire Line
	4600 1150 4850 1150
Wire Wire Line
	4850 1150 5150 1150
Wire Wire Line
	5150 1150 5350 1150
Wire Wire Line
	5350 1150 5600 1150
Wire Wire Line
	5350 1150 5350 1400
Connection ~ 5350 1150
Wire Wire Line
	5150 1850 5350 1850
Wire Wire Line
	5350 1850 5600 1850
Wire Wire Line
	5600 1850 6050 1850
Wire Wire Line
	6050 1850 6550 1850
Wire Wire Line
	6550 1850 6750 1850
Wire Wire Line
	6750 1850 6750 1800
Wire Wire Line
	5350 1850 5350 1800
Wire Wire Line
	6050 1850 6050 2050
Connection ~ 6050 1850
Wire Wire Line
	6550 1150 6750 1150
Wire Wire Line
	6750 1150 6950 1150
Wire Wire Line
	6950 1150 7050 1150
Wire Wire Line
	6750 1150 6750 1400
Wire Notes Line
	800  6000 900  6000
Wire Wire Line
	5150 1350 5150 1150
Connection ~ 5150 1150
Wire Wire Line
	5150 1750 5150 1850
Connection ~ 5350 1850
Connection ~ 6750 1150
Connection ~ 4850 1150
$Comp
L THERMISTOR TH201
U 1 1 612C2B50
P 4350 1150
F 0 "TH201" V 4450 1200 50  0000 C CNN
F 1 "500mA Thermistor" V 4250 1150 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM7mm" H 4350 1150 60  0001 C CNN
F 3 "" H 4350 1150 60  0000 C CNN
	1    4350 1150
	0    1    1    0   
$EndComp
$Comp
L TJA1050 U102
U 1 1 61687594
P 11550 3600
F 0 "U102" H 11550 3500 60  0000 C CNN
F 1 "TJA1050" H 11550 3200 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 11550 3600 60  0001 C CNN
F 3 "" H 11550 3600 60  0001 C CNN
	1    11550 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	10400 3650 10900 3650
Text GLabel 10950 2950 2    60   Input ~ 0
5V
$Comp
L GND #PWR03
U 1 1 61688951
P 10650 3950
F 0 "#PWR03" H 10650 3950 30  0001 C CNN
F 1 "GND" H 10650 3880 30  0001 C CNN
F 2 "" H 10650 3950 60  0001 C CNN
F 3 "" H 10650 3950 60  0001 C CNN
	1    10650 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	10900 3800 10650 3800
$Comp
L GND #PWR04
U 1 1 61688C26
P 12300 3700
F 0 "#PWR04" H 12300 3700 30  0001 C CNN
F 1 "GND" H 12300 3630 30  0001 C CNN
F 2 "" H 12300 3700 60  0001 C CNN
F 3 "" H 12300 3700 60  0001 C CNN
	1    12300 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	12150 3650 12300 3650
Wire Wire Line
	12300 3650 12300 3700
$Comp
L C C105
U 1 1 6168C8B8
P 10650 3300
F 0 "C105" H 10650 3400 40  0000 L CNN
F 1 "100nF" H 10656 3215 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 10688 3150 30  0001 C CNN
F 3 "" H 10650 3300 60  0000 C CNN
	1    10650 3300
	-1   0    0    1   
$EndComp
Wire Wire Line
	12150 3800 12350 3800
Wire Wire Line
	12350 3800 12650 3800
Wire Wire Line
	12250 4000 12650 4000
Wire Wire Line
	12250 3950 12250 4000
Wire Wire Line
	12250 4000 12250 4150
Wire Wire Line
	12250 3950 12150 3950
NoConn ~ 12150 4100
$Comp
L DIODE D101
U 1 1 61696639
P 12250 4350
F 0 "D101" H 12250 4450 40  0000 C CNN
F 1 "SMBJ30CA-TR TSV" H 12250 4250 40  0000 C CNN
F 2 "Diodes_SMD:DO-214AB" H 12250 4350 60  0001 C CNN
F 3 "https://uk.rs-online.com/web/p/tvs-diodes/7147197" H 12250 4350 60  0001 C CNN
	1    12250 4350
	0    -1   -1   0   
$EndComp
$Comp
L DIODE D102
U 1 1 61697088
P 12400 4350
F 0 "D102" H 12400 4450 40  0000 C CNN
F 1 "SMBJ30CA-TR TSV" H 12400 4250 40  0000 C CNN
F 2 "Diodes_SMD:DO-214AB" H 12400 4350 60  0001 C CNN
F 3 "https://uk.rs-online.com/web/p/tvs-diodes/7147197" H 12400 4350 60  0001 C CNN
	1    12400 4350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	12400 4150 12400 3900
Wire Wire Line
	12400 3900 12350 3900
Wire Wire Line
	12350 3900 12350 3800
Connection ~ 12350 3800
Connection ~ 12250 4000
$Comp
L GND #PWR05
U 1 1 616976C9
P 12300 4700
F 0 "#PWR05" H 12300 4700 30  0001 C CNN
F 1 "GND" H 12300 4630 30  0001 C CNN
F 2 "" H 12300 4700 60  0001 C CNN
F 3 "" H 12300 4700 60  0001 C CNN
	1    12300 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	12250 4550 12250 4650
Wire Wire Line
	12250 4650 12300 4650
Wire Wire Line
	12300 4650 12400 4650
Wire Wire Line
	12400 4650 12400 4550
Wire Wire Line
	12300 4650 12300 4700
Connection ~ 12300 4650
$Comp
L THERMISTOR TH1
U 1 1 618007CC
P 12900 3800
F 0 "TH1" V 13000 3850 50  0000 C CNN
F 1 "Thermistor" V 12800 3800 50  0000 C CNN
F 2 "Resistors_SMD:R_1812" H 12900 3800 60  0001 C CNN
F 3 "" H 12900 3800 60  0000 C CNN
	1    12900 3800
	0    1    1    0   
$EndComp
$Comp
L THERMISTOR TH2
U 1 1 61800847
P 12900 4000
F 0 "TH2" V 13000 4050 50  0000 C CNN
F 1 "Thermistor" V 12800 4000 50  0000 C CNN
F 2 "Resistors_SMD:R_1812" H 12900 4000 60  0001 C CNN
F 3 "" H 12900 4000 60  0000 C CNN
	1    12900 4000
	0    1    1    0   
$EndComp
Wire Wire Line
	13150 4000 13450 4000
Wire Wire Line
	13150 3800 13450 3800
$Comp
L R R11
U 1 1 61805C36
P 13200 6600
F 0 "R11" V 13280 6600 40  0000 C CNN
F 1 "4K7" V 13207 6601 40  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 13130 6600 30  0001 C CNN
F 3 "" H 13200 6600 30  0000 C CNN
	1    13200 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 3500 10650 3800
Wire Wire Line
	10650 3800 10650 3950
Connection ~ 10650 3800
Wire Wire Line
	10650 2950 10650 3000
Wire Wire Line
	10650 3000 10650 3100
Wire Wire Line
	10650 2950 10800 2950
Wire Wire Line
	10800 2950 10950 2950
Wire Wire Line
	10800 2950 10800 3950
Wire Wire Line
	10800 3950 10900 3950
Connection ~ 10800 2950
Text GLabel 9000 7450 0    60   Input ~ 0
12v
$Comp
L GND #PWR06
U 1 1 6180C8A2
P 8650 7550
F 0 "#PWR06" H 8650 7550 30  0001 C CNN
F 1 "GND" H 8650 7480 30  0001 C CNN
F 2 "" H 8650 7550 60  0001 C CNN
F 3 "" H 8650 7550 60  0001 C CNN
	1    8650 7550
	1    0    0    -1  
$EndComp
$Comp
L CONN_3 K1
U 1 1 6180ECA0
P 13600 7100
F 0 "K1" V 13550 7100 50  0000 C CNN
F 1 "1Wire" V 13650 7100 40  0000 C CNN
F 2 "Connect:bornier3" H 13600 7100 60  0001 C CNN
F 3 "" H 13600 7100 60  0001 C CNN
	1    13600 7100
	1    0    0    -1  
$EndComp
Text GLabel 13000 7000 0    60   BiDi ~ 0
1Wire
Text GLabel 13000 7200 0    60   Input ~ 0
3v3
$Comp
L GND #PWR07
U 1 1 6180F09A
P 12600 7150
F 0 "#PWR07" H 12600 7150 30  0001 C CNN
F 1 "GND" H 12600 7080 30  0001 C CNN
F 2 "" H 12600 7150 60  0001 C CNN
F 3 "" H 12600 7150 60  0001 C CNN
	1    12600 7150
	1    0    0    -1  
$EndComp
Wire Wire Line
	13200 6350 13200 6300
Wire Wire Line
	13200 6300 13100 6300
Wire Wire Line
	13100 6300 13100 7000
Wire Wire Line
	13000 7000 13100 7000
Wire Wire Line
	13100 7000 13250 7000
Connection ~ 13100 7000
Wire Wire Line
	13000 7200 13200 7200
Wire Wire Line
	13200 7200 13250 7200
$Comp
L DCDCConverter U5
U 1 1 6181648E
P 6050 1350
F 0 "U5" H 6050 1350 60  0000 C CNN
F 1 "DCDCConverter" H 6050 1450 60  0000 C CNN
F 2 "Divers:DC-DC-Converter" H 6050 1350 60  0001 C CNN
F 3 "" H 6050 1350 60  0001 C CNN
	1    6050 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 1600 6550 1850
Connection ~ 6550 1850
Wire Wire Line
	5600 1600 5600 1850
Connection ~ 5600 1850
$Comp
L ESP32Dev U1
U 1 1 6181082A
P 5200 6100
F 0 "U1" H 5100 5750 60  0000 C CNN
F 1 "ESP32Dev" H 5100 5750 60  0000 C CNN
F 2 "Divers:ESP32Dev" H 5100 5750 60  0001 C CNN
F 3 "" H 5100 5750 60  0001 C CNN
	1    5200 6100
	1    0    0    -1  
$EndComp
Text GLabel 7450 7400 2    60   Output ~ 0
3v3
Text GLabel 7300 5400 2    60   BiDi ~ 0
1Wire
Text GLabel 7250 6000 2    60   BiDi ~ 0
SCL
Text GLabel 7300 5800 2    60   BiDi ~ 0
SDA
$Comp
L GND #PWR08
U 1 1 61811BA7
P 7550 7250
F 0 "#PWR08" H 7550 7250 30  0001 C CNN
F 1 "GND" H 7550 7180 30  0001 C CNN
F 2 "" H 7550 7250 60  0001 C CNN
F 3 "" H 7550 7250 60  0001 C CNN
	1    7550 7250
	1    0    0    -1  
$EndComp
Text GLabel 3150 7400 0    60   Input ~ 0
5V
$Comp
L GND #PWR09
U 1 1 61811C68
P 2850 7250
F 0 "#PWR09" H 2850 7250 30  0001 C CNN
F 1 "GND" H 2850 7180 30  0001 C CNN
F 2 "" H 2850 7250 60  0001 C CNN
F 3 "" H 2850 7250 60  0001 C CNN
	1    2850 7250
	1    0    0    -1  
$EndComp
Text GLabel 7250 4600 2    60   Output ~ 0
CAN-TX
Text GLabel 7250 4800 2    60   Input ~ 0
CAN-RX
Wire Wire Line
	7250 4800 7100 4800
Wire Wire Line
	7100 4600 7250 4600
Wire Wire Line
	7300 5400 7100 5400
Wire Wire Line
	7300 5600 7100 5600
Wire Wire Line
	7300 5800 7100 5800
Wire Wire Line
	7450 7400 7100 7400
Wire Wire Line
	7100 7200 7550 7200
Wire Wire Line
	7550 7200 7550 7250
Wire Wire Line
	3150 7400 3400 7400
Wire Wire Line
	3400 7200 2850 7200
Wire Wire Line
	2850 7200 2850 7250
Text GLabel 10400 3650 0    60   Input ~ 0
CAN-TX
Text GLabel 8850 4100 0    60   Output ~ 0
CAN-RX
$Comp
L CONN_1 P3
U 1 1 618234EB
P 3050 9500
F 0 "P3" H 3130 9500 40  0000 L CNN
F 1 "CONN_1" H 3050 9555 30  0001 C CNN
F 2 "Connect:1pin" H 3050 9500 60  0001 C CNN
F 3 "" H 3050 9500 60  0001 C CNN
	1    3050 9500
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P4
U 1 1 61823587
P 3050 9600
F 0 "P4" H 3130 9600 40  0000 L CNN
F 1 "CONN_1" H 3050 9655 30  0001 C CNN
F 2 "Connect:1pin" H 3050 9600 60  0001 C CNN
F 3 "" H 3050 9600 60  0001 C CNN
	1    3050 9600
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P5
U 1 1 61823618
P 3050 9700
F 0 "P5" H 3130 9700 40  0000 L CNN
F 1 "CONN_1" H 3050 9755 30  0001 C CNN
F 2 "Connect:1pin" H 3050 9700 60  0001 C CNN
F 3 "" H 3050 9700 60  0001 C CNN
	1    3050 9700
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P6
U 1 1 618236AB
P 3050 9800
F 0 "P6" H 3130 9800 40  0000 L CNN
F 1 "CONN_1" H 3050 9855 30  0001 C CNN
F 2 "Connect:1pin" H 3050 9800 60  0001 C CNN
F 3 "" H 3050 9800 60  0001 C CNN
	1    3050 9800
	1    0    0    -1  
$EndComp
NoConn ~ 2900 9500
NoConn ~ 2900 9600
NoConn ~ 2900 9700
NoConn ~ 2900 9800
Text Notes 2600 9400 0    60   ~ 0
mounting holes
Text GLabel 11750 8600 3    60   BiDi ~ 0
SDA
Text GLabel 11250 8600 3    60   BiDi ~ 0
SCL
$Comp
L R R17
U 1 1 61824A79
P 11750 8000
F 0 "R17" V 11830 8000 40  0000 C CNN
F 1 "4K7" V 11757 8001 40  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 11680 8000 30  0001 C CNN
F 3 "" H 11750 8000 30  0000 C CNN
	1    11750 8000
	1    0    0    -1  
$EndComp
$Comp
L R R16
U 1 1 61824B5C
P 11250 8000
F 0 "R16" V 11330 8000 40  0000 C CNN
F 1 "4K7" V 11257 8001 40  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 11180 8000 30  0001 C CNN
F 3 "" H 11250 8000 30  0000 C CNN
	1    11250 8000
	1    0    0    -1  
$EndComp
Text GLabel 11050 7600 0    60   Input ~ 0
3v3
Wire Wire Line
	11050 7600 11250 7600
Wire Wire Line
	11250 7600 11500 7600
Wire Wire Line
	11500 7600 11750 7600
Wire Wire Line
	11750 7600 11750 7750
Wire Wire Line
	11250 7750 11250 7600
Connection ~ 11250 7600
Wire Wire Line
	11250 8250 11250 8600
Wire Wire Line
	11750 8250 11750 8600
$Comp
L SP3481CN IC101
U 1 1 62C08ACF
P 9700 6400
F 0 "IC101" H 9500 6650 40  0000 C CNN
F 1 "SP3481CN" H 9900 6150 40  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 9700 6400 35  0000 C CIN
F 3 "" H 9700 6400 60  0000 C CNN
	1    9700 6400
	1    0    0    -1  
$EndComp
$Comp
L THERMISTOR TH102
U 1 1 62C08E45
P 10900 6500
F 0 "TH102" V 11000 6550 50  0000 C CNN
F 1 "Thermistor" V 10800 6500 50  0000 C CNN
F 2 "Resistors_SMD:R_1812" H 10900 6500 60  0001 C CNN
F 3 "" H 10900 6500 60  0000 C CNN
	1    10900 6500
	0    1    1    0   
$EndComp
$Comp
L THERMISTOR TH101
U 1 1 62C08ED2
P 10900 6300
F 0 "TH101" V 11000 6350 50  0000 C CNN
F 1 "Thermistor" V 10800 6300 50  0000 C CNN
F 2 "Resistors_SMD:R_1812" H 10900 6300 60  0001 C CNN
F 3 "" H 10900 6300 60  0000 C CNN
	1    10900 6300
	0    1    1    0   
$EndComp
$Comp
L DIODE D103
U 1 1 62C08F57
P 10350 6750
F 0 "D103" H 10350 6850 40  0000 C CNN
F 1 "SMBJ30CA-TR TSV" H 10350 6650 40  0000 C CNN
F 2 "Diodes_SMD:DO-214AB" H 10350 6750 60  0001 C CNN
F 3 "https://uk.rs-online.com/web/p/tvs-diodes/7147197" H 10350 6750 60  0001 C CNN
	1    10350 6750
	0    -1   -1   0   
$EndComp
$Comp
L DIODE D104
U 1 1 62C08FE6
P 10550 6750
F 0 "D104" H 10550 6850 40  0000 C CNN
F 1 "SMBJ30CA-TR TSV" H 10550 6650 40  0000 C CNN
F 2 "Diodes_SMD:DO-214AB" H 10550 6750 60  0001 C CNN
F 3 "https://uk.rs-online.com/web/p/tvs-diodes/7147197" H 10550 6750 60  0001 C CNN
	1    10550 6750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10100 6300 10550 6300
Wire Wire Line
	10550 6300 10650 6300
Wire Wire Line
	11150 6300 11550 6300
Wire Wire Line
	11150 6500 11550 6500
Wire Wire Line
	10100 6500 10350 6500
Wire Wire Line
	10350 6500 10650 6500
Wire Wire Line
	10550 6550 10550 6300
Connection ~ 10550 6300
Wire Wire Line
	10350 6550 10350 6500
Connection ~ 10350 6500
Wire Wire Line
	10350 6950 10350 7200
Wire Wire Line
	10350 7200 10450 7200
Wire Wire Line
	10450 7200 10550 7200
Wire Wire Line
	10550 7200 10550 6950
$Comp
L GND #PWR011
U 1 1 62C09522
P 10450 7300
F 0 "#PWR011" H 10450 7300 30  0001 C CNN
F 1 "GND" H 10450 7230 30  0001 C CNN
F 2 "" H 10450 7300 60  0001 C CNN
F 3 "" H 10450 7300 60  0001 C CNN
	1    10450 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	10450 7300 10450 7200
Connection ~ 10450 7200
$Comp
L GND #PWR012
U 1 1 62C09619
P 9700 6950
F 0 "#PWR012" H 9700 6950 30  0001 C CNN
F 1 "GND" H 9700 6880 30  0001 C CNN
F 2 "" H 9700 6950 60  0001 C CNN
F 3 "" H 9700 6950 60  0001 C CNN
	1    9700 6950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 6750 9700 6950
Text GLabel 9650 5700 0    60   Input ~ 0
5V
Wire Wire Line
	9650 5700 9700 5700
Wire Wire Line
	9700 5400 9700 5700
Wire Wire Line
	9700 5700 9700 6050
Wire Wire Line
	9300 6350 9100 6350
Wire Wire Line
	9100 6350 9100 6400
Wire Wire Line
	9100 6400 9100 6450
Wire Wire Line
	9100 6450 9300 6450
Text GLabel 8900 6550 0    60   Input ~ 0
RS485-TX
Wire Wire Line
	8900 6550 9300 6550
Text GLabel 8550 6250 0    60   Output ~ 0
RS485-RX
Text GLabel 8900 6400 0    60   Input ~ 0
RS485-EN
Wire Wire Line
	8900 6400 9100 6400
Connection ~ 9100 6400
Text GLabel 7250 6600 2    60   Output ~ 0
RS485-EN
Text GLabel 7250 6200 2    60   Output ~ 0
RS485-TX
Text GLabel 7250 6400 2    60   Input ~ 0
RS485-RX
Wire Wire Line
	7250 6000 7100 6000
Wire Wire Line
	7250 6200 7100 6200
Wire Wire Line
	7250 6400 7100 6400
$Comp
L C C101
U 1 1 62C0ADD8
P 10150 5750
F 0 "C101" H 10150 5850 40  0000 L CNN
F 1 "100nF" H 10156 5665 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 10188 5600 30  0001 C CNN
F 3 "" H 10150 5750 60  0000 C CNN
	1    10150 5750
	-1   0    0    1   
$EndComp
Wire Wire Line
	9200 5400 9700 5400
Wire Wire Line
	9700 5400 10150 5400
Wire Wire Line
	10150 5400 10150 5550
Connection ~ 9700 5700
$Comp
L GND #PWR013
U 1 1 62C0AF15
P 10150 6100
F 0 "#PWR013" H 10150 6100 30  0001 C CNN
F 1 "GND" H 10150 6030 30  0001 C CNN
F 2 "" H 10150 6100 60  0001 C CNN
F 3 "" H 10150 6100 60  0001 C CNN
	1    10150 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10150 5950 10150 6100
$Comp
L CONN_5 P103
U 1 1 62C0B845
P 9600 8350
F 0 "P103" V 9550 8350 50  0000 C CNN
F 1 "Display" V 9650 8350 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x05" H 9600 8350 60  0001 C CNN
F 3 "" H 9600 8350 60  0001 C CNN
	1    9600 8350
	1    0    0    -1  
$EndComp
Text GLabel 9000 8550 0    60   Input ~ 0
3v3
$Comp
L GND #PWR014
U 1 1 62C0B936
P 8600 8200
F 0 "#PWR014" H 8600 8200 30  0001 C CNN
F 1 "GND" H 8600 8130 30  0001 C CNN
F 2 "" H 8600 8200 60  0001 C CNN
F 3 "" H 8600 8200 60  0001 C CNN
	1    8600 8200
	1    0    0    -1  
$EndComp
Text GLabel 13300 8900 2    60   Input ~ 0
BTN
Text GLabel 9000 8250 0    60   BiDi ~ 0
SCL
Text GLabel 9000 8450 0    60   Input ~ 0
BTN
Text GLabel 9000 8350 0    60   BiDi ~ 0
SDA
Wire Wire Line
	8600 8150 9200 8150
Wire Wire Line
	9000 8250 9200 8250
Wire Wire Line
	9000 8350 9200 8350
Wire Wire Line
	9000 8550 9200 8550
$Comp
L DIODE D105
U 1 1 62C0C718
P 7250 1150
F 0 "D105" H 7250 1250 40  0000 C CNN
F 1 "1N5819" H 7250 1050 40  0000 C CNN
F 2 "Diodes_SMD:D_1206" H 7250 1150 60  0001 C CNN
F 3 "" H 7250 1150 60  0000 C CNN
	1    7250 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 1150 7750 1150
$Comp
L CONN_4 P101
U 1 1 62C0D94D
P 9650 7600
F 0 "P101" V 9600 7600 50  0000 C CNN
F 1 "CAN" V 9700 7600 50  0000 C CNN
F 2 "Connect:bornier4" H 9650 7600 60  0001 C CNN
F 3 "" H 9650 7600 60  0001 C CNN
	1    9650 7600
	1    0    0    -1  
$EndComp
Text GLabel 13450 3800 2    60   BiDi ~ 0
CAN-H
Text GLabel 13450 4000 2    60   BiDi ~ 0
CAN-L
Text GLabel 9050 7650 0    60   BiDi ~ 0
CAN-H
Text GLabel 9050 7750 0    60   BiDi ~ 0
CAN-L
Wire Wire Line
	9050 7750 9300 7750
Wire Wire Line
	9050 7650 9300 7650
Wire Wire Line
	9300 7550 8650 7550
Wire Wire Line
	9000 7450 9300 7450
Text GLabel 9750 9100 0    60   BiDi ~ 0
RS485-A
Text GLabel 9750 9200 0    60   BiDi ~ 0
RS485-B
$Comp
L CONN_4 P102
U 1 1 62C0E52F
P 10300 9050
F 0 "P102" V 10250 9050 50  0000 C CNN
F 1 "RS485" V 10350 9050 50  0000 C CNN
F 2 "Connect:bornier4" H 10300 9050 60  0001 C CNN
F 3 "" H 10300 9050 60  0001 C CNN
	1    10300 9050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9750 9100 9950 9100
Wire Wire Line
	9750 9200 9950 9200
Text GLabel 7200 800  2    60   Output ~ 0
5.8V
Wire Wire Line
	7200 800  6950 800 
Wire Wire Line
	6950 800  6950 1150
Connection ~ 6950 1150
Text GLabel 9550 8900 0    60   Input ~ 0
5.8V
Wire Wire Line
	9550 8900 9950 8900
$Comp
L GND #PWR015
U 1 1 62C0FFE2
P 9150 9000
F 0 "#PWR015" H 9150 9000 30  0001 C CNN
F 1 "GND" H 9150 8930 30  0001 C CNN
F 2 "" H 9150 9000 60  0001 C CNN
F 3 "" H 9150 9000 60  0001 C CNN
	1    9150 9000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 9000 9950 9000
Text GLabel 11550 6300 2    60   BiDi ~ 0
RS485-A
Text GLabel 11550 6500 2    60   BiDi ~ 0
RS485-B
Wire Wire Line
	7100 6600 7250 6600
$Comp
L R R101
U 1 1 62C15F60
P 13000 8400
F 0 "R101" V 13080 8400 40  0000 C CNN
F 1 "4K7" V 13007 8401 40  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 12930 8400 30  0001 C CNN
F 3 "" H 13000 8400 30  0000 C CNN
	1    13000 8400
	1    0    0    -1  
$EndComp
Wire Wire Line
	11500 7750 11500 7600
Connection ~ 11500 7600
Text GLabel 13500 8500 3    60   Input ~ 0
BTN
Wire Wire Line
	13300 8250 13300 8600
Wire Wire Line
	9000 8450 9200 8450
Wire Wire Line
	8600 8150 8600 8200
Wire Wire Line
	13200 6850 13200 7200
Connection ~ 13200 7200
Wire Wire Line
	13250 7100 12600 7100
Wire Wire Line
	12600 7100 12600 7150
Text GLabel 3400 5600 0    60   Output ~ 0
MOSI
Text GLabel 3400 5800 0    60   Output ~ 0
SCK
Text GLabel 3400 6000 0    60   Output ~ 0
CS
Text GLabel 3400 5400 0    60   Input ~ 0
MISO
Text GLabel 3400 6200 0    60   Output ~ 0
DC
Text GLabel 3400 6400 0    60   Output ~ 0
RST
$Comp
L BSS138 Q?
U 1 1 62D10656
P 9500 4000
F 0 "Q?" H 9500 3851 40  0000 R CNN
F 1 "BSS138" H 9500 4150 40  0000 R CNN
F 2 "SOT-23" H 9370 4102 29  0000 C CNN
F 3 "" H 9500 4000 60  0000 C CNN
	1    9500 4000
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 62D10E07
P 9900 3550
F 0 "R?" V 9980 3550 40  0000 C CNN
F 1 "10K" V 9907 3551 40  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 9830 3550 30  0001 C CNN
F 3 "" H 9900 3550 30  0000 C CNN
	1    9900 3550
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 62D10FA8
P 9050 3650
F 0 "R?" V 9130 3650 40  0000 C CNN
F 1 "10K" V 9057 3651 40  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8980 3650 30  0001 C CNN
F 3 "" H 9050 3650 30  0000 C CNN
	1    9050 3650
	-1   0    0    1   
$EndComp
Wire Wire Line
	8850 4100 9050 4100
Wire Wire Line
	9050 4100 9300 4100
Wire Wire Line
	9050 3900 9050 4100
Connection ~ 9050 4100
Wire Wire Line
	9450 3300 9450 3800
Wire Wire Line
	8650 3300 9050 3300
Wire Wire Line
	9050 3300 9450 3300
Wire Wire Line
	9050 3300 9050 3400
Wire Wire Line
	9700 4100 9900 4100
Wire Wire Line
	9900 4100 10900 4100
Wire Wire Line
	9900 3800 9900 4100
Connection ~ 9900 4100
Wire Wire Line
	9900 3300 9900 3000
Wire Wire Line
	9900 3000 10650 3000
Connection ~ 10650 3000
$Comp
L BSS138 Q?
U 1 1 62D120FB
P 8950 6150
F 0 "Q?" H 8950 6001 40  0000 R CNN
F 1 "BSS138" H 8950 6300 40  0000 R CNN
F 2 "SOT-23" H 8820 6252 29  0000 C CNN
F 3 "" H 8950 6150 60  0000 C CNN
	1    8950 6150
	0    1    1    0   
$EndComp
Wire Wire Line
	9150 6250 9200 6250
Wire Wire Line
	9200 6250 9300 6250
Wire Wire Line
	8550 6250 8650 6250
Wire Wire Line
	8650 6250 8750 6250
Text GLabel 8650 3300 0    60   Input ~ 0
3v3
Connection ~ 9050 3300
$Comp
L R R?
U 1 1 62D12BA0
P 8650 5800
F 0 "R?" V 8730 5800 40  0000 C CNN
F 1 "10K" V 8657 5801 40  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8580 5800 30  0001 C CNN
F 3 "" H 8650 5800 30  0000 C CNN
	1    8650 5800
	-1   0    0    1   
$EndComp
Text GLabel 8450 5450 0    60   Input ~ 0
3v3
$Comp
L R R?
U 1 1 62D12EBB
P 9200 5800
F 0 "R?" V 9280 5800 40  0000 C CNN
F 1 "10K" V 9207 5801 40  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 9130 5800 30  0001 C CNN
F 3 "" H 9200 5800 30  0000 C CNN
	1    9200 5800
	-1   0    0    1   
$EndComp
Wire Wire Line
	9200 5400 9200 5550
Connection ~ 9700 5400
Wire Wire Line
	9200 6050 9200 6250
Connection ~ 9200 6250
Wire Wire Line
	8650 6050 8650 6250
Connection ~ 8650 6250
Wire Wire Line
	8650 5550 8650 5450
Wire Wire Line
	8450 5450 8650 5450
Wire Wire Line
	8650 5450 8900 5450
Wire Wire Line
	8900 5450 8900 5950
Connection ~ 8650 5450
Text Notes 8050 5300 0    60   ~ 0
RO is high impedence when RE/DE \nare high, so a level shifter is required.
Text Notes 8700 4550 0    60   ~ 0
Pin RXD is driven so this level shifter is strictly \nnot required and could be replaced by a R divider.\n\n3v into TXD is high enough for 5V logic.\n
Text GLabel 3400 6600 0    60   Output ~ 0
LED
$Comp
L CONN_12 P?
U 1 1 63CF9FAE
P 2100 6000
F 0 "P?" V 2050 6000 60  0000 C CNN
F 1 "Display" V 2150 6000 60  0000 C CNN
F 2 "" H 2100 6000 60  0001 C CNN
F 3 "" H 2100 6000 60  0001 C CNN
	1    2100 6000
	-1   0    0    1   
$EndComp
Text GLabel 2450 6350 2    60   Input ~ 0
3v3
$Comp
L GND #PWR?
U 1 1 63CFA138
P 2450 6450
F 0 "#PWR?" H 2450 6450 30  0001 C CNN
F 1 "GND" H 2450 6380 30  0001 C CNN
F 2 "" H 2450 6450 60  0001 C CNN
F 3 "" H 2450 6450 60  0001 C CNN
	1    2450 6450
	1    0    0    -1  
$EndComp
Text GLabel 2450 6550 2    60   Input ~ 0
5V
Text GLabel 2450 5750 2    60   Input ~ 0
MOSI
Text GLabel 2450 5650 2    60   Output ~ 0
MISO
Text GLabel 2450 5850 2    60   Input ~ 0
SCK
Text GLabel 2450 5950 2    60   Input ~ 0
CS
Text GLabel 2450 6050 2    60   Input ~ 0
DC
Text GLabel 2450 6150 2    60   Input ~ 0
RST
Text GLabel 2450 6250 2    60   Input ~ 0
LED
Text GLabel 2450 5550 2    60   Output ~ 0
B1
Text GLabel 2450 5450 2    60   Output ~ 0
B2
Text GLabel 3400 5200 0    60   Input ~ 0
B1
Text GLabel 3400 5000 0    60   Input ~ 0
B2
Text Notes 1650 6400 0    60   ~ 0
Orange
Text Notes 1650 6600 0    60   ~ 0
Red
Text Notes 1650 6500 0    60   ~ 0
Black
Text Notes 500  7950 0    60   ~ 0
 Red    |  5v          | 5v           |\n| black  |  0v          | 0v           |\n| pink   |  CS          | GPIO_NUM_25  |\n| cyan   |  Reset       | GPIO_NUM_27  |\n| white  |  DC/RS       | GPIO_NUM_26  |\n| blue   |  SDI(MOSI)   | GPIO_NUM_32  |\n| green  |  SCK         | GPIO_NUM_33  |\n| grey   |  LED         | GPIO_NUM_14  |\n| Purple |  SDO(MISO)   | GPIO_NUM_35  |\n| Orange |  3.3v        | 3.3v         |\n| Brown  | Touch Lower  | GPIO_NUM_19  |\n| Yellow | Touch Top    | TBD          |
Text Notes 1650 6000 0    60   ~ 0
Pink
Text Notes 1650 6200 0    60   ~ 0
Cyan
Text Notes 1650 6100 0    60   ~ 0
White
Text Notes 1650 5800 0    60   ~ 0
Blue
Text Notes 1650 5900 0    60   ~ 0
Green
Text Notes 1650 6300 0    60   ~ 0
Grey
Text Notes 1650 5700 0    60   ~ 0
Purple
Text Notes 1650 5600 0    60   ~ 0
Brown
Text Notes 1650 5500 0    60   ~ 0
Yellow
$Comp
L CONN_12 P?
U 1 1 64180578
P 1700 8800
F 0 "P?" V 1650 8800 60  0000 C CNN
F 1 "Display" V 1750 8800 60  0000 C CNN
F 2 "" H 1700 8800 60  0001 C CNN
F 3 "" H 1700 8800 60  0001 C CNN
	1    1700 8800
	-1   0    0    1   
$EndComp
Text GLabel 2050 9350 2    60   Input ~ 0
5V
$Comp
L GND #PWR?
U 1 1 641806B2
P 2050 9250
F 0 "#PWR?" H 2050 9250 30  0001 C CNN
F 1 "GND" H 2050 9180 30  0001 C CNN
F 2 "" H 2050 9250 60  0001 C CNN
F 3 "" H 2050 9250 60  0001 C CNN
	1    2050 9250
	1    0    0    -1  
$EndComp
Text GLabel 2050 9150 2    60   Input ~ 0
CS
Text GLabel 2050 9050 2    60   Input ~ 0
RST
Text GLabel 2050 8950 2    60   Input ~ 0
DC
Text GLabel 2050 8850 2    60   Input ~ 0
MOSI
Text GLabel 2050 8750 2    60   Input ~ 0
SCK
Text GLabel 2050 8650 2    60   Input ~ 0
LED
Text GLabel 2050 8550 2    60   Input ~ 0
MOSI
Text GLabel 2050 8450 2    60   Input ~ 0
3v3
Text GLabel 2050 8350 2    60   Output ~ 0
B1
Text GLabel 2050 8250 2    60   Output ~ 0
B2
$EndSCHEMATC
