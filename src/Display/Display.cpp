//
// Created by Sergio Zambrano on 05/10/2019.
//

#include "Display.h"

Display::Display(Adafruit_NeoPixel& strip, int displayRows, int displayCols, unsigned int delay, char * phrase, byte phraseSize)
{
    this->_strip       = strip;
    this->_displayRows = displayRows;
    this->_displayCols = displayCols;
    this->_delay       = delay;
    this->_negro       = strip.Color(000, 000, 000, 000);
    this->_azul        = strip.Color(000, 000, 050, 000);
    this->_rojo        = strip.Color(000, 050, 000, 000);
    this->_verde       = strip.Color(050, 000, 000, 000);
    this->_blanco      = strip.Color(000, 000, 000, 050);
    this->_position    = 0;
    this->_lastShow    = 0;
    this->_phrase      = phrase;
    this->_phraseSize  = phraseSize;
}

void Display::messageFrame(int pos)
{
    char actualLetter;
    byte phraseIndex;
    int  col;
    int  letterIndex;
    byte letterColumn;
    byte row;
    int  pixel;

    for (col = 0; col < this->_displayCols; col++) {
        phraseIndex = (pos + col) / letterColumns;
        if (phraseIndex >= this->_phraseSize) {
            actualLetter = ' ';
        } else {
            actualLetter = this->_phrase[phraseIndex];
        }
        letterIndex  = letterPosition(actualLetter);
        letterColumn = (pos + col) % letterColumns;
        for (row = 0; row < this->_displayRows; row++) {
            pixel = (row * this->_displayCols) + (this->_displayCols - (col + 1));
            if (letterPixel(letterIndex, row, letterColumn) == 1) {
                this->_strip.setPixelColor(pixel, this->_azul);
            } else {
                this->_strip.setPixelColor(pixel, this->_negro);
            }
        }
    }
    this->_strip.show();
}

void Display::show()
{
    if(millis() - this->_lastShow < this->_delay) {
        return;
    }
    this->_lastShow = millis();
    this->_position = ++this->_position % (this->_phraseSize * letterColumns);
    messageFrame(this->_position);
}

void Display::restart()
{
}
