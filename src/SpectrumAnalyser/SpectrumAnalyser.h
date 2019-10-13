//
// Created by Sergio Zambrano on 12/10/2019.
//

#ifndef SPECTRUM_ANALYSER_H
#define SPECTRUM_ANALYSER_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "arduinoFFT.h"

#ifndef SA_SAMPLES
    #define SA_SAMPLES 64
#endif

#ifndef SA_SAMPLING_FREQUENCY
    #define SA_SAMPLING_FREQUENCY 4388
#endif

class SpectrumAnalyser {
public:
    SpectrumAnalyser(Adafruit_NeoPixel& strip, int displayRows, int displayCols, int dacPin, unsigned int delay);
    void show();

protected:
    Adafruit_NeoPixel _strip;
    byte _displayRows;
    byte _displayCols;
    byte _dacPin;
    unsigned int _delay;
    uint32_t _bgcolor;
    uint32_t _fgcolor;
    arduinoFFT _FFT;
    double _vReal[SA_SAMPLES];
    double _vImag[SA_SAMPLES];
    long _lastShow;

    void fillColumn(byte column, double value);
};

#endif // SPECTRUM_ANALYSER_H
