This project implements a smart energy monitor capable of measuring and displaying the real-time energy usage of connected appliances. It utilizes various analog circuits, embedded software, and peripheral configurations to sample voltage and current, process power calculations, and display results on a seven-segment display.

System Components
Analog Circuitry: Conditions voltage and current signals for processing.
Embedded Software: Samples data, performs calculations, and controls display.
Peripherals: Configured for ADC sampling, UART communication, and display.
Code Overview
The code performs three primary functions:

Initializes peripherals and configuration settings.
Samples voltage and current signals and calculates power values.
Displays calculated power values on a seven-segment display.
Initialization
The initialization code configures all peripherals, including ADC, timers, UART, and interrupts, to ensure proper data sampling and communication.

Key Steps:
ADC Configuration: Sets the ADC to a 10 kHz sampling rate to capture high-resolution voltage and current signals.
Timer Configuration: Initializes timers to trigger ADC sampling and handle other time-sensitive operations.
Interrupt Setup: Configures external and timer-based interrupts to capture zero-crossing events for phase angle calculations.

For more infomration read the wiki
