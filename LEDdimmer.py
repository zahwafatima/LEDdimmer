import numpy as np
import math 
import csv
import serial
import time
import pandas as pd
import plotly.express as px
import matplotlib.pyplot as plt

# IMPORTS BELOW ONLY NEEDED IF USING SPYDER IDE
import plotly.io as pio  # needed to plot plotly graphs in spyder
# pio.renderers.default = 'svg'  # to plot plotly graphs in spyder
pio.renderers.default = 'browser' # to plot plotly graphs in browser

## OPEN SERIAL PORT 
ser = serial.Serial(port= "/dev/cu.usbserial-0001", baudrate = 9600, bytesize = 8, timeout =2, stopbits = serial.STOPBITS_ONE)

## INITIALIZATIONS
rxDataStr = ''
rxDataList = []     # list to store received data in tuple form (ADC value, analog voltage)
rxTimesList = []    # list to store time stamps of received data
startTime = time.time()
timeInterval = 60   # 1 minute intervals

## CAPTURE UART DATA
while (time.time() - startTime < timeInterval):
    line = ser.readline()       # reads uint16_t nums as single bytes till \n n stores in string
    if ((line != b' \n') and (line != b'\n') and (line != b'') and (line != b' ')):
        rxDataStr += line.decode('Ascii') + ' '
        time_meas = time.time() - startTime
        rxTimesList.append(time_meas)
    
## CLOSE SERIAL PORT    
ser.close()  # close any open serial ports


### Rx DATA CLEANUP AND STRING TO FLOAT CONVERSION
rxDataStr = rxDataStr.strip()  # remove unwanted chars and spaces 
rxDataStr = rxDataStr.split('\n')  # split string by \n and store in a list
refVoltage = 3.3            # refVoltage is the supply voltage to the PIC
resolution = 1023.0         # PIC resolution

# ensuring lengths match for data list and times list
rxDataStr = rxDataStr[10:-1]        # removing few leading captures to avoid issues
rxTimesList = rxTimesList[10:-1]    # removing few leading captures to avoid issues

intensity = [int(line) for line in rxDataStr]    # converting ADCBUF values to int and storing in a list
vpwm = [((duty_cycle/100) * refVoltage) for duty_cycle in intensity]        # calculating vpwm using intensity. duty cycle


### CONVERT Rx DATA INTO DATA FRAME

dF = pd.DataFrame()
dF['Rx Time (sec)'] = rxTimesList
dF['Rx Intensity'] = intensity
dF['Rx Voltage PWM (V)'] = vpwm


### DATA STATISTICS
print(dF)
print(dF.describe())


### COPY RX DATA AND RX TIME IN CSV AND XLS FILES
dF.to_csv('RxApp2_Group7_SabaIshaZahwa.csv', index = True)     
dF.to_excel('RxApp2_Group7_SabaIshaZahwa.xlsx', sheet_name='New Sheet')

# PLOT Rx BUFFER DATA VS Rx TIME
fig_buffer = px.line(dF, x='Rx Time (sec)', y='Rx Intensity', title='Rx Intensity vs Time (G7 Saba, Isha, Zahwa)')
fig_buffer.show()

# PLOT Rx VOLTAGE DATA VS Rx TIME
fig_voltage = px.line(dF, x='Rx Time (sec)', y='Rx Voltage PWM (V)', title='Rx Voltage PWM vs Time (G7 Saba, Isha, Zahwa)')
fig_voltage.show()



