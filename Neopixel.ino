#include <Arduino.h>
#ifdef __AVR__
    #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define STRIP_PIN                2
#define DISPLAY_ROWS            10
#define DISPLAY_COLS            14
#define DISPLAY_DELAY          150
#define SA_DAC_PIN              A3
#define SA_SAMPLES              64
#define SA_SAMPLING_FREQUENCY 4388
#define SA_DELAY               150
#define SWITCH                   3

extern "C" {
    #include <Adafruit_NeoPixel.h>
//    #include "src/Display/Display.h"
    #include "src/SpectrumAnalyser/SpectrumAnalyser.h"
    #include "src/Ambilight/Ambilight.h"
};

char phrase[] = "  HOLA QUE HACES?";

Adafruit_NeoPixel strip(DISPLAY_ROWS * DISPLAY_COLS, STRIP_PIN, NEO_RGBW);
//Display display(strip, DISPLAY_ROWS, DISPLAY_COLS, DISPLAY_DELAY, phrase, sizeof(phrase) - 1);
SpectrumAnalyser spectrumAnalyser(strip, DISPLAY_ROWS, DISPLAY_COLS, SA_DAC_PIN, SA_DELAY);
Ambilight ambilight(strip, DISPLAY_ROWS, DISPLAY_COLS, 3, 0);

int signal = LOW;
int action = 2;
long time  = 0;

void setup() {
    pinMode(SWITCH, INPUT);
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
        clock_prescale_set(clock_div_1);
    #endif
    strip.begin();             // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();              //  Update strip to match
    strip.setBrightness(255);  // Set BRIGHTNESS to about 1/5 (max = 255)
    Serial.begin(9600);

}

void loop() {
    if (signal == LOW && digitalRead(SWITCH) == HIGH && millis() - time > 200) {
        action = ++action % 3;
        time = millis();
        signal = HIGH;
    }
    if (digitalRead(SWITCH) == LOW && signal == HIGH) {
        time = millis();
        signal = LOW;
    }
    if (action == 0) {
        //display.show();
    }
    if (action == 1) {
        spectrumAnalyser.show();
    }
    if (action == 2) {
        ambilight.show();
    }
}
