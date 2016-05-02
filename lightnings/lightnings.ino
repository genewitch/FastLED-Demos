//  Lightnings is a program that lets you make an LED strip look like a 1D cloud of lightning
//
//  Original by: Daniel Wilson, 2014
//
//  Modified by: Andrew Tuline 2015
//
//  This modified version creates lightning along various sections of the strip. Looks great inside my poly fill constructed cloud.
//
/* edited for nodeMCU with arduino IDE by @genewitch
    pin 2 is D2 on my nodeMCU for this sketch
*/


#include "FastLED.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define LED_DT 2                                             // Dpin 2 is D2 on my nodeMCU
#define LED_CK 11                                             // Irrelevant
#define COLOR_ORDER GRB                                       // It's GRB for WS2812 named below
#define LED_TYPE WS2812B                                       //  WS2812B 5050 RGB LED Strip     
#define NUM_LEDS 144                                           // Number of LEDs.

// Global variables can be changed on the fly.
uint8_t max_bright = 7;                                      // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

uint8_t frequency = 50;                                       // controls the interval between strikes
uint8_t flashes = 8;                                          //the upper limit of flashes per strike
unsigned int dimmer = 1;

uint8_t ledstart;                                             // Starting location of a flash
uint8_t ledlen;                                               // Length of a flash


void setup() {
  delay(1000);                                                // allows reprogramming if accidently blowing power w/leds
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(OvercastSky);  // Use this for WS2812
//  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
  FastLED.setBrightness(max_bright);
} // setup()


void loop() {
  ledstart = random8(NUM_LEDS);           // Determine starting location of flash
  ledlen = random8(NUM_LEDS-ledstart);    // Determine length of flash (not to go beyond NUM_LEDS-1)
  for (int flashCounter = 0; flashCounter < random8(3,flashes); flashCounter++) {
    if(flashCounter == 0) dimmer = 5;     // the brightness of the leader is scaled down by a factor of 5
    else dimmer = random8(1,3);           // return strokes are brighter than the leader
    fill_solid(leds+ledstart,ledlen,CHSV(255, 0, 255/dimmer));
    FastLED.show();                       // Show a section of LED's
    delay(random8(4,10));                 // each flash only lasts 4-10 milliseconds
    fill_solid(leds+ledstart,ledlen,CHSV(255,0,0));   // Clear the section of LED's
    FastLED.show();     
    if (flashCounter == 0) delay (150);   // longer delay until next flash after the leader
    delay(50+random8(100));               // shorter delay between strokes  
  } // for()
  delay(random8(frequency)*100);          // delay between strikes
} // loop()

