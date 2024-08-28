# Heart Monitoring System

This project is a simple heart rate and SpO2 monitoring system using an SPI OLED LCD Module and a MAX30100 Pulse Oximeter Heart Rate Sensor Module.

## Features
- **Real-Time Monitoring**: Displays heart rate (bpm) and SpO2 (%) on an OLED screen.
- **Visual Feedback**: Shows heart and SpO2 symbols on the OLED screen.

## Hardware Used
- **MAX30100 Pulse Oximeter Sensor**
- **OLED LCD Module** (SPI interface)

### Software Requirements
- Arduino IDE
- Adafruit GFX and SH110X libraries for OLED display
- MAX30100 Pulse Oximeter library
- U8glib library

## How It Works
The system reads heart rate and SpO2 data from the MAX30100 sensor and displays the values on the OLED screen along with graphical symbols.

### Icon Conversion
The heart and SpO2 symbols were converted into byte arrays using [Image2Cpp](https://javl.github.io/image2cpp/).

## Code
-  **heart.ino**