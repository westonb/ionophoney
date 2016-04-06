EESchema Schematic File Version 2
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
LIBS:wbraun_ic_lib
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 1200 1300 1400 1400
U 57033973
F0 "Power Supply" 60
F1 "power_supply.sch" 60
F2 "Supply_Current_Sense" I R 2600 1500 60 
$EndSheet
$Sheet
S 1200 3100 1400 1400
U 5703397E
F0 "RF" 60
F1 "rf.sch" 60
F2 "HI" I R 2600 3300 60 
F3 "LI" I R 2600 3400 60 
F4 "Inverter_Out" I R 2600 3600 60 
F5 "Inverter_Return" I R 2600 3700 60 
$EndSheet
$Sheet
S 3800 3100 1400 1400
U 57033981
F0 "Control" 60
F1 "control.sch" 60
F2 "FET_H" I R 5200 3200 60 
F3 "FET_L" I R 5200 3300 60 
F4 "RF_Current_Sense" I R 5200 3500 60 
F5 "Supply_Current_Sense" I R 5200 3800 60 
F6 "RF_Current_Phase" I R 5200 3600 60 
$EndSheet
$Sheet
S 3800 1300 1400 1400
U 57033984
F0 "Measurement" 60
F1 "measurement.sch" 60
F2 "RF_Current_Sense" I R 5200 1500 60 
F3 "RF_Current_Phase" I R 5200 1700 60 
F4 "Inverter_A" I R 5200 2000 60 
F5 "Inverter_B" I R 5200 2100 60 
$EndSheet
$Sheet
S 6400 1300 1400 1400
U 57056E8A
F0 "Output Network" 60
F1 "output_network.sch" 60
F2 "Inverter_Out" I R 7800 1500 60 
F3 "Inverter_Return" I R 7800 1600 60 
$EndSheet
Wire Wire Line
	2600 3300 2700 3300
Wire Wire Line
	2600 3400 2700 3400
Wire Wire Line
	2600 3600 2700 3600
Wire Wire Line
	2600 3700 2700 3700
Wire Wire Line
	2600 1500 2700 1500
Wire Wire Line
	5200 1500 5300 1500
Wire Wire Line
	5200 1700 5300 1700
Wire Wire Line
	5200 2000 5300 2000
Wire Wire Line
	5200 2100 5300 2100
Wire Wire Line
	5200 3200 5300 3200
Wire Wire Line
	5200 3300 5300 3300
Wire Wire Line
	5200 3500 5300 3500
Wire Wire Line
	5200 3600 5300 3600
Wire Wire Line
	5200 3800 5300 3800
Wire Wire Line
	7800 1500 7900 1500
Wire Wire Line
	7800 1600 7900 1600
Text Label 2700 1500 0    60   ~ 0
Supply_Current
Text Label 2700 3300 0    60   ~ 0
FET_H
Text Label 2700 3400 0    60   ~ 0
FET_L
Text Label 5300 1500 0    60   ~ 0
RF_Current
Text Label 5300 1700 0    60   ~ 0
RF_Phase
Text Label 5300 3200 0    60   ~ 0
FET_H
Text Label 5300 3300 0    60   ~ 0
FET_L
Text Label 5300 3500 0    60   ~ 0
RF_Current
Text Label 5300 3600 0    60   ~ 0
RF_Phase
Text Label 5300 3800 0    60   ~ 0
Supply_Current
Text Label 2700 3600 0    60   ~ 0
Inverter+
Text Label 2700 3700 0    60   ~ 0
Inverter-
Text Label 5300 2000 0    60   ~ 0
Inverter+
Text Label 5300 2100 0    60   ~ 0
Inverter_Out
Text Label 7900 1500 0    60   ~ 0
Inverter_Out
Text Label 7900 1600 0    60   ~ 0
Inverter-
$EndSCHEMATC
