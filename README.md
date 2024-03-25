# LED Dimmer
Embedded Systems LED Dimmer Project

<ol><li>Employed the PIC24F16KA101 microcontroller for the precise control of LED intensity via Pulse Width Modulation (PWM), displaying data capture through Python programming.</li>
<li>The application integrates push buttons to change states of the system (i.e., LED ON, LED BLINKING), utilizing UART and timer peripherals within MPLAB IDE to capture PWM voltage and pulse frequency used.</li>

**State Diagram**

<img width="511" alt="image" src="https://github.com/zahwafatima/LEDdimmer/assets/95236604/ccbda479-d89d-45e3-83c8-de59ede90859">


**Breadboard Connections**

<img width="408" alt="image" src="https://github.com/zahwafatima/LEDdimmer/assets/95236604/68522839-75b1-4efa-a747-2fb6438b0150">


Using the PIC24F16KA101 Microcontroller and the driver functions developed so far, designed a LED Light control App to control the LED brightness or intensity connected on pin 12 and display it using Python. The App will use the push buttons (PBs) connected to the input ports RA2, RA4 and RB4, and LED connected to pin 12 with a 1K resistor.
PB1, PB2 and PB3 represent push buttons connected to ports RA2, RA4 and RB4 respectively. The
LED Light control should operate as follows. Used different MCU Peripherals and Drivers namely UART and Timer peripherals for designing this app.

| User Input    | Output |
| ------------- | ------ |
| PB1 Press  | If PB1 is pressed once, it should put the System in ON MODE i.e. turn on the LED (connected to Microcontroller pin 12) at 95-100% intensity (full brightness). At this point, turning the Potentiometer should adjust the LED intensity between 95-100% and 0% (LED off) using Pulse width modulation voltage (PWM) signals supplied to the LED as described below. If PB1 is pressed again, it should put the System in OFF MODE i.e. turn off the LED and put the system in low power Idle() mode   |
| PB2 Press | If PB2 is pressed once, it should blink the LED at 100% intensity level at 500 msec intervals. If PB2 is pressed again, it should stop the LED blinking. |
| Python Script | When Python script is run on the computer, it should for a period of 1 min: <ol><li>Capture and Store the intensity levels (0-100%) and average PWM voltage output (Vpwm) supplied by the Microcontroller to the LED vs time (in seconds) in a single CSV or Excel file with proper indexing and column names</li> <li>Plot intensity levels (0-100%) and average voltage output supplied by the Microcontroller to the LED (Vpwm) vs time in seconds on 2 Separate graphs</li> <li>When run in OFF mode, the CSV/Excel file and graphs should show only 0 for intensity and average voltage. The 2 Graphs should display: <ol><li>Title (Intensity in % or PWM voltage Vpwm)</li> <li>X and Y axes labels</li> <li>Pulse Frequency used</li></li></ol> |







