#include <Arduino.h>

#include <FastLED.h>

#include <LEDMatrix.h>
#include <LEDText.h>
#include <Font12x16.h>

// Change the next 6 defines to match your matrix type and size

#define LED_PIN        27
#define LED2_PIN       13 //! new
#define COLOR_ORDER    GRB
#define CHIPSET        WS2812B

#define MATRIX_TILE_WIDTH  -64  //!old-32 // width of EACH NEOPIXEL MATRIX (not total display)
#define MATRIX_TILE_HEIGHT  8   // height of each matrix
#define MATRIX_TILE_H       1   // number of matrices arranged horizontally (negative for reverse order)
#define MATRIX_TILE_V       2   // number of matrices arranged vertically (negative for reverse order)
#define MATRIX_SIZE         (MATRIX_WIDTH*MATRIX_HEIGHT)
#define MATRIX_PANEL        (MATRIX_WIDTH*MATRIX_HEIGHT)

#define MATRIX_WIDTH        (MATRIX_TILE_WIDTH*MATRIX_TILE_H)
#define MATRIX_HEIGHT       (MATRIX_TILE_HEIGHT*MATRIX_TILE_V)

#define NUM_LEDS            (MATRIX_WIDTH*MATRIX_HEIGHT)

cLEDMatrix<MATRIX_TILE_WIDTH, MATRIX_TILE_HEIGHT, VERTICAL_ZIGZAG_MATRIX, MATRIX_TILE_H, MATRIX_TILE_V, VERTICAL_BLOCKS> leds;
cLEDText ScrollingMsg;

const unsigned char TxtDemo[] = { EFFECT_FRAME_RATE "\x02" EFFECT_SCROLL_LEFT "      0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ" };

void setup()
{
  //!old FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size());

  FastLED.addLeds<CHIPSET,  LED_PIN, COLOR_ORDER>(leds[0], 0,             leds.Size()/2).setCorrection(TypicalLEDStrip);//TypicalSMD5050 //! new
  FastLED.addLeds<CHIPSET, LED2_PIN, COLOR_ORDER>(leds[0], leds.Size()/2, leds.Size()/2).setCorrection(TypicalLEDStrip); //! new

  FastLED.setBrightness(10);
  FastLED.clear(true);
  FastLED.show();

  ScrollingMsg.SetFont(Font12x16Data);
  ScrollingMsg.Init(&leds, leds.Width(), ScrollingMsg.FontHeight() + 1, 0, -1);
  ScrollingMsg.SetText((unsigned char *)TxtDemo, sizeof(TxtDemo) - 1);
  ScrollingMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0xff, 0x00, 0xff);

  ScrollingMsg.SetOptionsChangeMode(INSTANT_OPTIONS_MODE);
}


void loop()
{
  if (ScrollingMsg.UpdateText() == -1){
    ScrollingMsg.SetText((unsigned char *)TxtDemo, sizeof(TxtDemo) - 1);
    FastLED.clear(true);
  }
  else
  {
    FastLED.show();
   }
   delay(10);
}