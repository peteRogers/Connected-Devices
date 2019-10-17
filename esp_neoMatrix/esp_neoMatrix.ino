/**
 * esp_neoMatrix is the starter project for connecting an 8x8 rgb LED neomatrix 
 * display to an esp32.
 */

#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>

#define PIN 21
#define mw 8 //amount of pixels width
#define mh 8 //amount of pixels height
#define NUMMATRIX (mw*mh)
CRGB matrixleds[NUMMATRIX];

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(matrixleds, mw, mh, mw/8, 1, 
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE );



void setup() {
  FastLED.addLeds<NEOPIXEL,PIN>(matrixleds, NUMMATRIX); 
  matrix->begin();
  matrix->setBrightness(255);
}

void loop() {
  matrix->drawRect(0,0,4,4, matrix->Color(0,255,0));
  matrix->show();//this sends data to the board
  delay(100);
}
