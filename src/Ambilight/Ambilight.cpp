//
// Created by Sergio Zambrano on 12/10/2019.
//

#include "Ambilight.h"

Ambilight::Ambilight(Adafruit_NeoPixel &strip, uint8_t displayRows, uint8_t displayCols, uint8_t lights, uint16_t delay)
{
    randomSeed(analogRead(0));
    this->_strip = strip;
    this->_displayRows = displayRows;
    this->_displayCols = displayCols;
    this->_lights      = lights;
    this->_delay       = delay;
    this->_lastShow    = -1000000;
    this->_mincol      =       -3;
    this->_maxcol      =       16;
    this->_minrow      =       -3;
    this->_maxrow      =       12;
    this->_radio       =        13;

    this->_points = (struct LightPoint*) malloc(sizeof(struct LightPoint) * this->_lights);
    for (int i = 0; i < this->_lights; ++i) {
        randomSeed(analogRead(0));
        (_points + i)->green     = (byte)random(i % 3 == 0 ? 75 : 0);
        (_points + i)->red       = (byte)random(i % 3 == 1 ? 75 : 0);
        (_points + i)->blue      = (byte)random(i % 3 == 2 ? 75 : 0);
        (_points + i)->column    = (float)random(this->_mincol, this->_maxcol);
        (_points + i)->row       = (float)random(this->_minrow, this->_maxrow);
        (_points + i)->direction = random(360);
        (_points + i)->velocity  = random(25, 75) / 100.0;
    }
}

void Ambilight::show()
{
    if (millis() - this->_lastShow < this->_delay) {
        return;
    }

    this->_lastShow = millis();

    float rojo;
    float verde;
    float azul;
    float distX;
    float distY;
    float distancia;
    for (int row = 0; row < this->_displayRows; row++) {
        for (int col = 0; col < this->_displayCols; col++) {
            rojo = 0;
            verde = 0;
            azul = 0;

            for (int light = 0; light < this->_lights; light++) {
                distX = abs(row - (_points + light)->row);
                distY = abs(col - (_points + light)->column);
                distancia = sqrt(pow(distX, 2) + pow(distY, 2));
                int pixel = (row * this->_displayCols) + (this->_displayCols - (col + 1));

                rojo += max(0, (_points + light)->red - ((_points + light)->red * (distancia / this->_radio)));
                verde += max(0, (_points + light)->green - ((_points + light)->green * (distancia / this->_radio)));
                azul += max(0, (_points + light)->blue - ((_points + light)->blue * (distancia / this->_radio)));
                this->_strip.setPixelColor(pixel, this->_strip.Color(min(255, verde), min(255, rojo), min(255, azul)));
            }
        }
    }
    for (int light = 0; light < this->_lights; light++) {
        float thesin = -sin(((_points + light)->direction / 360.0) * (2.0 * PI));
        float thecos = cos(((_points + light)->direction / 360.0) * (2.0 * PI));
        (_points + light)->row    += round(100.0 * thesin * (_points + light)->velocity) / 100.0;
        (_points + light)->column += round(100.0 * thecos * (_points + light)->velocity) / 100.0;

        if ((_points + light)->column > this->_maxcol) {
            (_points + light)->column = this->_maxcol;
            if ((_points + light)->direction < 90) {
                (_points + light)->direction = 180 - (_points + light)->direction;
            } else {
                (_points + light)->direction = 540 - (_points + light)->direction;
            }
        }
        if ((_points + light)->column < this->_mincol) {
            (_points + light)->column = this->_mincol;
            if ((_points + light)->direction > 180) {
                (_points + light)->direction = 540 - (_points + light)->direction;
            } else {
                (_points + light)->direction = 180 - (_points + light)->direction;
            }
        }
        if ((_points + light)->row > this->_maxrow) {
            (_points + light)->row = this->_maxrow;
            if ((_points + light)->direction < 270) {
                (_points + light)->direction = 540 - (_points + light)->direction;
            } else {
                (_points + light)->direction = 360 - (_points + light)->direction;
            }
        }
        if ((_points + light)->row < this->_minrow) {
            (_points + light)->row = this->_minrow;
            (_points + light)->direction = 360 - (_points + light)->direction;
        }
        (_points + light)->direction = (_points + light)->direction % 360;
    }
    this->_strip.show();
}

void Ambilight::debug()
{
    for (int i = 0; i < this->_lights; ++i) {
        Serial.print("(_points + ");
        Serial.print(i);
        Serial.print(")->column    = ");
        Serial.println((_points + i)->column);

        Serial.print("(_points + ");
        Serial.print(i);
        Serial.print(")->row       = ");
        Serial.println((_points + i)->row);

        Serial.print("(_points + ");
        Serial.print(i);
        Serial.print(")->direction = ");
        Serial.println((_points + i)->direction);

        Serial.print("(_points + ");
        Serial.print(i);
        Serial.print("->seno       = ");
        float mysin = -sin(((_points + i)->direction / 360.0) * (2.0 * PI));
        Serial.println(mysin);

        Serial.print("(_points + ");
        Serial.print(i);
        Serial.print("->coseno     = ");
        float mycos = cos(((_points + i)->direction / 360.0) * (2.0 * PI));
        Serial.println(mycos);
        Serial.print("(_points + ");
        Serial.print(i);
        Serial.print(")->new col   = ");
        Serial.println((_points + i)->column + (round(100.0 * mycos * (_points + i)->velocity) / 100.0));
        Serial.print("(_points + ");
        Serial.print(i);
        Serial.print(")->new row   = ");
        Serial.println((_points + i)->row + (round(100.0 * mysin * (_points + i)->velocity) / 100.0));
        Serial.println("--------");
    }
}
