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
LIBS:class_e-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
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
S 1500 3000 900  1100
U 5832761A
F0 "Control" 60
F1 "control.sch" 60
F2 "RF_A" I R 2400 3100 60 
F3 "RF_B" I R 2400 3200 60 
$EndSheet
$Sheet
S 1500 4400 900  1100
U 58327666
F0 "Power" 60
F1 "Power.sch" 60
$EndSheet
$Sheet
S 1500 5800 900  1100
U 58327681
F0 "RF" 60
F1 "RF.sch" 60
F2 "RF_A" I R 2400 5900 60 
F3 "RF_B" I R 2400 6000 60 
$EndSheet
Wire Wire Line
	2400 3100 2800 3100
Wire Wire Line
	2800 3100 2800 5900
Wire Wire Line
	2800 5900 2400 5900
Wire Wire Line
	2400 3200 2600 3200
Wire Wire Line
	2600 3200 2600 6000
Wire Wire Line
	2600 6000 2400 6000
$EndSCHEMATC
