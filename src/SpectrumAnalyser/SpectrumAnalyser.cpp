//
// Created by Sergio Zambrano on 12/10/2019.
//

#include "SpectrumAnalyser.h"

SpectrumAnalyser::SpectrumAnalyser(Adafruit_NeoPixel &strip, int displayRows, int displayCols, int dacPin, unsigned int delay)
{
    this->_strip = strip;
    this->_displayRows = displayRows;
    this->_displayCols = displayCols;
    this->_dacPin = dacPin;
    this->_delay = delay;
    this->_bgcolor = strip.Color(000, 000, 000, 000);
    this->_fgcolor = strip.Color(000,  50, 000, 000);
    this->_FFT = arduinoFFT();
    this->_lastShow = 0;
}

void SpectrumAnalyser::fillColumn(byte column, double value)
{
    byte virtualRow = 0;
    byte realRow = 0;
    byte pixel = 0;
    byte height;


    height = (byte)((value * (double)this->_displayRows) / 256.0);
    height = height > this->_displayRows ? this->_displayRows : height;
    for (virtualRow = 0; virtualRow < this->_displayRows; virtualRow++) {
        realRow = this->_displayRows - virtualRow;
        pixel = (realRow * this->_displayCols) - (column + 1);
        this->_strip.setPixelColor(pixel, (virtualRow + 1) <= height ? this->_fgcolor : this->_bgcolor);
    }
}

void SpectrumAnalyser::show()
{
//    if(millis() - this->_lastShow < this->_delay) {
//        return;
//    }

    int sampling_period_us = round(1000000*(1.0/SA_SAMPLING_FREQUENCY));
    long microseconds = 0;

    for(int i=0; i<SA_SAMPLES; i++){
        microseconds = micros();
        this->_vReal[i] = analogRead(this->_dacPin);
        this->_vImag[i] = 0;

        while(micros() < (microseconds + sampling_period_us)) {}
    }

    this->_FFT.Windowing(this->_vReal, SA_SAMPLES, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);
    this->_FFT.Compute(this->_vReal, this->_vImag, SA_SAMPLES, FFT_FORWARD);
    this->_FFT.ComplexToMagnitude(this->_vReal, this->_vImag, SA_SAMPLES);
    //double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

    for(uint16_t j = 0; j < this->_displayCols; j++) {
        fillColumn(j, this->_vReal[j + 1]);
    }
    this->_strip.show();
}
