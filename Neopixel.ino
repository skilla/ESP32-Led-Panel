#include <Adafruit_NeoPixel.h>
#include "arduinoFFT.h"

#ifdef __AVR__
    #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//#include "src/LetraCutre/LetraCutre.h"
#include "src/LetraSimple/LetraSimple.h"

#define STRIP_PIN     2
#define displayRows  10
#define displayCols  14
#define LED_COUNT   140
#define DAC_PIN      A3
#define SWITCH        3

const uint16_t samples = 64;
double vReal[samples];
double vImag[samples];

arduinoFFT FFT = arduinoFFT();
Adafruit_NeoPixel strip(LED_COUNT, STRIP_PIN, NEO_RGBW);
uint32_t negro  = strip.Color(000, 000, 000);
uint32_t azul   = strip.Color(000, 000, 255);
uint32_t rojo   = strip.Color(000, 255, 000);
uint32_t verde  = strip.Color(255, 000, 000);
uint32_t blanco = strip.Color(255, 255, 255);
int signal = LOW;
int action = 1;
long time  = 0;

char phrase[] = "  HOLA QUE HACES?";
int  pixel    = 0;
int  position = 0;

void setup() {
    pinMode(SWITCH, INPUT);
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
        clock_prescale_set(clock_div_1);
    #endif
    strip.begin();             // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();              //  Update strip to match
    strip.setBrightness(10);    // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {
    if (signal == LOW && digitalRead(SWITCH) == HIGH && millis() - time > 200) {
        action = ++action % 2;
        time = millis();
        signal = HIGH;
    }
    if (digitalRead(SWITCH) == LOW && signal == HIGH) {
        time = millis();
        signal = LOW;
    }
    if (action == 0) {
        messageFrame(position);
        strip.show();
        delay(200);
        position = ++position % ((sizeof(phrase) - 1) * letterColumns);
    }
    if (action == 1) {
        for(int i=0; i<samples; i++){
            vReal[i] = analogRead(DAC_PIN);
            vImag[i] = 0;
        }

        FFT.Windowing(vReal, samples, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
        FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
        FFT.ComplexToMagnitude(vReal, vImag, samples);

        for(uint16_t j = 0; j < displayCols; j++) {
            fillColumn(j, vReal[j + 1]);
        }
        strip.show();
    }
}

void messageFrame(int pos) {
    char actualLetter;
    byte phraseIndex;
    int col;
    int  letterIndex;
    byte  letterColumn;
    byte row;
    int pixel;

    for (col = 0; col < displayCols; col++) {
        phraseIndex = (pos + col) / letterColumns;
        if (phraseIndex >= (sizeof(phrase) - 1)) {
            actualLetter = ' ';
        } else {
            actualLetter = phrase[phraseIndex];
        }
        letterIndex  = letterPosition(actualLetter);
        letterColumn = (pos + col) % letterColumns;
        for (row = 0; row < displayRows; row++) {
            pixel = (row * displayCols) + (displayCols - (col + 1));
            if (letterPixel(letterIndex, row, letterColumn) == 1) {
                strip.setPixelColor(pixel, azul);
            } else {
                strip.setPixelColor(pixel, negro);
            }
        }
    }
}

void fillColumn(byte column, double value) {
    byte virtualRow = 0;
    byte realRow = 0;
    byte pixel = 0;
    byte height;

    height = (byte)((value * (double)displayRows) / 128.0);
    height = height > displayRows ? displayRows : height;
    for (virtualRow = 0; virtualRow < displayRows; virtualRow++) {
        realRow = displayRows - virtualRow;
        pixel = (realRow * displayCols) - (column + 1);
        strip.setPixelColor(pixel, (virtualRow+1) <= height ? azul : negro);
    }
}
