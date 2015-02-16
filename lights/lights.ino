#include "FastLED.h"

#define NUM_LEDS 300

#define DATA_PIN 5
#define CLOCK_PIN 13
#define SHADES 16
#define MAX_ALLIANCE_INTENSITY 64

// Define the array of leds
CRGB leds[NUM_LEDS];
CRGB allianceColor[SHADES-2];

void setup() { 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() { 
  testAllianceColor();
}



/***************************************
 * Alliance Pulse
 ***************************************/
void testAllianceColor() {
  setBlueAlliance();
  pulseAllianceColor();
  pulseAllianceColor();
  pulseAllianceColor();

  setRedAlliance();
  pulseAllianceColor();
  pulseAllianceColor();
  pulseAllianceColor();
}

void pulseAllianceColor() {
  for (int x=0;x<SHADES-2;x++) {
    for (int i=0;i<NUM_LEDS;i++) {
      leds[i] = allianceColor[x];
    }
    FastLED.show();
    delay(50);
  }
  FastLED.show();
  delay(50);
  for (int x=SHADES-2;x>0;x--) {
    for (int i=0;i<NUM_LEDS;i++) {
      leds[i] = allianceColor[x-1];
    }
    FastLED.show();
    delay(50);
  }
}

void setBlueAlliance() {
  for (int i=1;i<SHADES-1;i++) {
    int x = ((MAX_ALLIANCE_INTENSITY / (SHADES)) * i) - 1;
    allianceColor[i-1] = CRGB(0,0,x);
  }
}

void setRedAlliance() {
  for (int i=1;i<SHADES-1;i++) {
    int x = ((MAX_ALLIANCE_INTENSITY / (SHADES)) * i) - 1;
    allianceColor[i-1] = CRGB(x,0,0);
  }
}
