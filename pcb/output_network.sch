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
LIBS:ionophoney-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 6
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
L CONN_01X03 P?
U 1 1 57056EDB
P 4800 3400
F 0 "P?" H 4800 3600 50  0000 C CNN
F 1 "CONN_01X03" V 4900 3400 50  0000 C CNN
F 2 "" H 4800 3400 50  0000 C CNN
F 3 "" H 4800 3400 50  0000 C CNN
	1    4800 3400
	1    0    0    -1  
$EndComp
Text HLabel 2600 2800 0    60   Input ~ 0
Inverter_Out
Text HLabel 4350 3400 0    60   Input ~ 0
Inverter_Return
$Comp
L GND #PWR?
U 1 1 57057053
P 4500 3600
F 0 "#PWR?" H 4500 3350 50  0001 C CNN
F 1 "GND" H 4500 3450 50  0000 C CNN
F 2 "" H 4500 3600 50  0000 C CNN
F 3 "" H 4500 3600 50  0000 C CNN
	1    4500 3600
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 5705707F
P 3050 2800
F 0 "C?" H 3075 2900 50  0000 L CNN
F 1 "C" H 3075 2700 50  0000 L CNN
F 2 "" H 3088 2650 50  0000 C CNN
F 3 "" H 3050 2800 50  0000 C CNN
	1    3050 2800
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 570570E2
P 3050 3100
F 0 "C?" H 3075 3200 50  0000 L CNN
F 1 "C" H 3075 3000 50  0000 L CNN
F 2 "" H 3088 2950 50  0000 C CNN
F 3 "" H 3050 3100 50  0000 C CNN
	1    3050 3100
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 57057121
P 3050 3400
F 0 "C?" H 3075 3500 50  0000 L CNN
F 1 "C" H 3075 3300 50  0000 L CNN
F 2 "" H 3088 3250 50  0000 C CNN
F 3 "" H 3050 3400 50  0000 C CNN
	1    3050 3400
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 57057156
P 3050 3700
F 0 "C?" H 3075 3800 50  0000 L CNN
F 1 "C" H 3075 3600 50  0000 L CNN
F 2 "" H 3088 3550 50  0000 C CNN
F 3 "" H 3050 3700 50  0000 C CNN
	1    3050 3700
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 57057181
P 3050 4000
F 0 "C?" H 3075 4100 50  0000 L CNN
F 1 "C" H 3075 3900 50  0000 L CNN
F 2 "" H 3088 3850 50  0000 C CNN
F 3 "" H 3050 4000 50  0000 C CNN
	1    3050 4000
	0    1    1    0   
$EndComp
Wire Wire Line
	2600 2800 2900 2800
Wire Wire Line
	2900 2800 2900 4000
Connection ~ 2900 3100
Connection ~ 2900 3400
Connection ~ 2900 3700
Wire Wire Line
	3200 2800 3200 4000
Connection ~ 3200 3100
Connection ~ 3200 3400
Connection ~ 3200 3700
Wire Wire Line
	3200 2800 4200 2800
Wire Wire Line
	4200 2800 4200 3300
Wire Wire Line
	4200 3300 4600 3300
Wire Wire Line
	4350 3400 4600 3400
Wire Wire Line
	4600 3500 4500 3500
Wire Wire Line
	4500 3500 4500 3600
Text Notes 2600 4400 0    60   ~ 0
DC Blocking / Tuning Capacitors
$EndSCHEMATC
