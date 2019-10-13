//
// Created by Sergio Zambrano on 05/10/2019.
//

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "../LetraSimple/LetraSimple.h"
//#include "../LetraCutre/LetraCutre.h"

class Display {
    public:
        Display(Adafruit_NeoPixel& strip, int displayRows, int displayCols, unsigned int delay, char * phrase, byte phraseSize);
        void show();
        void restart();

    protected:
        Adafruit_NeoPixel _strip;
        byte _displayRows;
        byte _displayCols;
        char * _phrase;
        int _phraseSize;
        byte _delay;
        uint32_t _negro;
        uint32_t _azul;
        uint32_t _rojo;
        uint32_t _verde;
        uint32_t _blanco;
        long _lastShow;
        int _position;
        void messageFrame(int pos);
};

#endif //DISPLAY_H
