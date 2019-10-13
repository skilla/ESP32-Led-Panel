//
// Created by Sergio Zambrano on 12/10/2019.
//

#ifndef AMBILIGHT_H
#define AMBILIGHT_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

struct LightPoint {
    float row;
    float column;
    word direction;
    float velocity;
    byte green;
    byte red;
    byte blue;
};

class Ambilight
{
    public:
        Ambilight(Adafruit_NeoPixel &strip, uint8_t displayRows, uint8_t displayCols, uint8_t lights, uint16_t delay);
        void show();
        void debug();

    protected:
        Adafruit_NeoPixel _strip;
        uint8_t           _displayRows;
        uint8_t           _displayCols;
        uint8_t           _lights;
        uint16_t          _delay;
        long              _lastShow;
        struct LightPoint *_points;
        float             _mincol;
        float             _maxcol;
        float             _minrow;
        float             _maxrow;
        int               _radio;
};

#endif //AMBILIGHT_H
