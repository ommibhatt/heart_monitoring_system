#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <SPI.h>

#define REPORTING_PERIOD_MS 1000

// OLED display pins (update with your specific pin numbers)
#define OLED_MOSI 23
#define OLED_CLK  18
#define OLED_DC   17
#define OLED_CS   5
#define OLED_RST  16

// Create a PulseOximeter object
PulseOximeter pox;

// Create the OLED display
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);

// Time at which the last beat occurred
uint32_t tsLastReport = 0;

// 'heart', 25x22px
const unsigned char epd_bitmap_heart [] PROGMEM = {
    0xf0, 0x7f, 0x07, 0x80, 0xe0, 0x1c, 0x03, 0x80, 0xc0, 0x08, 0x01, 0x80, 0x80, 0x00, 0x00, 0x80, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 
    0x00, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x80, 0xa4, 0x80, 0x80, 0x81, 0xe5, 0xc0, 0x80, 
    0xff, 0x65, 0x7f, 0x80, 0xe0, 0x07, 0x03, 0x80, 0xe0, 0x06, 0x03, 0x80, 0xf0, 0x02, 0x07, 0x80, 
    0xf8, 0x02, 0x0f, 0x80, 0xfc, 0x00, 0x1f, 0x80, 0xff, 0x00, 0x7f, 0x80, 0xff, 0x80, 0xff, 0x80, 
    0xff, 0xc1, 0xff, 0x80, 0xff, 0xf7, 0xff, 0x80
};

// 'spo2', 25x25px
const unsigned char epd_bitmap_spo2 [] PROGMEM = {
    0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0x80, 
    0xff, 0xf7, 0xff, 0x80, 0xff, 0xf7, 0xff, 0x80, 0xff, 0xe3, 0xff, 0x80, 0xff, 0xe3, 0xff, 0x80, 
    0xff, 0xc1, 0xff, 0x80, 0xff, 0xc0, 0xff, 0x80, 0xff, 0x80, 0xff, 0x80, 0xff, 0x10, 0x7f, 0x80, 
    0xff, 0x3c, 0x7f, 0x80, 0xfe, 0x24, 0x3f, 0x80, 0xfe, 0x36, 0x3f, 0x80, 0xfe, 0x27, 0xbf, 0x80, 
    0xfe, 0x3d, 0x3f, 0x80, 0xff, 0x19, 0x7f, 0x80, 0xff, 0x00, 0x7f, 0x80, 0xff, 0x80, 0xff, 0x80, 
    0xff, 0xc1, 0xff, 0x80, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0x80, 
    0xff, 0xff, 0xff, 0x80
};

// Callback routine executed when a pulse is detected
void onBeatDetected() {
    Serial.println("Beat!");
}

void setup() {
    Serial.begin(9600);

    // Initialize OLED
    display.begin(0, true);  // We don't use the I2C address but we will reset!
    display.clearDisplay();

    // Draw the initial bitmaps on the OLED
    display.drawBitmap(0, 0, epd_bitmap_heart, 25, 22, 1);
    display.drawBitmap(0, 39, epd_bitmap_spo2, 25, 25, 1);
    display.display();

    Serial.print("Initializing pulse oximeter...");

    // Initialize the MAX30100 sensor
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);  // Infinite loop indicating failure
    } else {
        Serial.println("SUCCESS");
    }

    // Configure sensor to use 7.6mA for LED drive
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register callback routine
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
    // Read from the sensor
    pox.update();

    // Update OLED display and Serial output with heart rate and SpO2 levels
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        // Fetch the heart rate and SpO2 levels
        float heartRate = pox.getHeartRate();
        float spO2 = pox.getSpO2();

        // Display on Serial Monitor
        Serial.print("Heart rate: ");
        Serial.print(heartRate);
        Serial.print(" bpm / SpO2: ");
        Serial.print(spO2);
        Serial.println(" %");

        // Clear the OLED display
        display.clearDisplay();

        // Display heart rate on OLED
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(30, 10);
        display.print("Heart Rate:");
        display.setCursor(30, 20);
        display.print(heartRate);
        display.print(" bpm");

        // Display SpO2 on OLED
        display.setCursor(30, 40);
        display.print("SpO2:");
        display.setCursor(30, 50);
        display.print(spO2);
        display.print(" %");

        // Update the OLED display
        display.display();

        // Update the last report timestamp
        tsLastReport = millis();
    }
}
