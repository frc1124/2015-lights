#include "FastLED.h"
#include <stdlib.h>
#include <Wire.h>

char charCommand[32];
char* robotState;
int counter = 0;

#define NUM_LEDS 234
#define NUM_LIFT_LIGHTS 87
#define NUM_DRIVE_LIGHTS 30

#define DRIVE_LEFT_DATA_PIN 10
#define LIFT_LEFT_DATA_PIN 9
#define DRIVE_RIGHT_DATA_PIN 11
#define LIFT_RIGHT_DATA_PIN 12

#define MAX_LIFT 59

CRGB leds[NUM_LEDS];
char allianceColor = 'R';
float liftPosition = 59.0;
int a, b, c, v, w, y, z;

#define TEST_ENABLED 1
#define TEST_DISABLED 2
#define AUTO_ENABLED 3
#define AUTO_DISABLED 4
#define TELEOP_ENABLED 5
#define TELEOP_DISABLED 6
#define DISCONNECTED 7
#define FINISHED 8
#define ERROR_MODE 9
#define OFF 10

byte mode = DISCONNECTED;
int frame = 24;


void setup() {
  FastLED.addLeds<NEOPIXEL, DRIVE_LEFT_DATA_PIN>(leds, NUM_DRIVE_LIGHTS);
  FastLED.addLeds<NEOPIXEL, LIFT_LEFT_DATA_PIN>(&leds[NUM_DRIVE_LIGHTS], NUM_LIFT_LIGHTS);
  FastLED.addLeds<NEOPIXEL, DRIVE_RIGHT_DATA_PIN>(&leds[NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS], NUM_DRIVE_LIGHTS);
  FastLED.addLeds<NEOPIXEL, LIFT_RIGHT_DATA_PIN>(&leds[NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS+NUM_DRIVE_LIGHTS], NUM_LIFT_LIGHTS);
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}
void testEnabled() {
  for(int x=0; x<NUM_DRIVE_LIGHTS; x++) {
    leds[x] = CRGB::Orange;
    leds[x+NUM_LIFT_LIGHTS] = CRGB::Orange;
  }
  int percent = liftPosition / MAX_LIFT * 100;
  int lights = NUM_LIFT_LIGHTS * percent / 100;
  int lightsOn = NUM_LIFT_LIGHTS - lights;
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn; x++) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::White;
    leds[x+NUM_DRIVE_LIGHTS] = CRGB::White;
  }
  int y = NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn;
  if (y >= NUM_LEDS) y = NUM_LEDS-1;
  leds[y] = CRGB::Orange;
  FastLED.show();
}

void testDisabled() {
  if (frame > 220) c = 1;
  if (frame < 25) c = 0;
  for(int x=0; x<NUM_DRIVE_LIGHTS; x++) {
    switch (c) {
      case 0:
        frame = frame + 1;
        break;
      case 1:
        frame = frame - 1;
        break;
      default:
        frame++;
      break;
    }
    if (x >= NUM_LEDS) break;
      leds[x] = CRGB(frame, frame, 0);
      leds[x+NUM_LIFT_LIGHTS] = CRGB(frame, frame, 0);
  }
  int percent = liftPosition / MAX_LIFT * 100;
  int lights = NUM_LIFT_LIGHTS * percent / 100;
  int lightsOn =  NUM_LIFT_LIGHTS - lights;
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn; x++) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::White;
    leds[x+NUM_DRIVE_LIGHTS] = CRGB::White;
  }
  int y = NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn;
  if (y >= NUM_LEDS) y = NUM_LEDS-1;
  leds[y] = CRGB::Orange;
  y += NUM_DRIVE_LIGHTS;
  if (y >= NUM_LEDS) y = NUM_LEDS-1;
  leds[y] = CRGB::Orange;
  FastLED.show();
  delay(50);
}

void pulseDriveTrain() {
  if (frame > 220) c = 1;
  if (frame < 25) c = 0;
  for(int x=0; x<NUM_DRIVE_LIGHTS; x++) {
    switch (c) {
      case 0:
        frame = frame + 1;
        break;
      case 1:
        frame = frame - 1;
        break;
      default:
        frame++;
      break;
    }
    switch (allianceColor) {
    case 'B':
      leds[x] = CRGB(0, 0, frame);
      leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB(0, 0, frame);
      break;
    case 'R':
      leds[x] = CRGB(frame, 0, 0);
      leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB(frame, 0, 0);
      break;
    default:
      leds[x] = CRGB::Black;
      leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::Black;
      break;
    }
  }
}

void autoDisabled() {
  pulseDriveTrain();
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS; x+=2) {
      if (x >= NUM_LEDS) break;
      leds[x] = CRGB::White;
      leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::White;
    }
    for(int x=NUM_DRIVE_LIGHTS+1; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS; x+=2) {
      if (x >= NUM_LEDS) break;
      leds[x] = CRGB::Yellow;
      leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::Yellow;
    }
    for(int x=NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS; x<NUM_LEDS; x+=2) {
      if (x >= NUM_LEDS) break;
      leds[x] = CRGB::White;
      leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::White;
    }
    for(int x=NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS+1; x<NUM_LEDS; x+=2) {
      if (x >= NUM_LEDS) break;
      leds[x] = CRGB::Yellow;
      leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::Yellow;
    }
  FastLED.show();
  delay(10);
}

void autoEnabled() {
  for(int x=0; x<NUM_DRIVE_LIGHTS; x++) {
    if (x >= NUM_LEDS) break;
    switch (allianceColor) {
    case 'B':
      leds[x] = CRGB::Blue;
      leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::Blue;
      break;
    case 'R':
      leds[x] = CRGB::Red;
      leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::Red;
      break;
    default:
      leds[x] = CRGB::Black;
      leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::Black;
      break;
    }
  }
  int percent = liftPosition / MAX_LIFT * 100;
  int lights = NUM_LIFT_LIGHTS * percent / 100;
  int lightsOn = NUM_LIFT_LIGHTS - lights;
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn; x++) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::White;
    leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::White;
  }
  leds[NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS - lightsOn] = CRGB::Red;
  leds[NUM_DRIVE_LIGHTS + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS - lightsOn] = CRGB::Red;
  FastLED.show();
}

void teleopEnabled() {
  for(int x=0; x<NUM_DRIVE_LIGHTS; x++) {
    if (x >= NUM_LEDS) break;
    switch (allianceColor) {
    case 'B':
      leds[x] = CRGB::Blue;
      leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::Blue;
      break;
    case 'R':
      leds[x] = CRGB::Red;
      leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::Red;
      break;
    default:
      leds[x] = CRGB::Black;
      leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::Black;
      break;
    }
  }
  int percent = liftPosition / MAX_LIFT * 100;
  int lights = NUM_LIFT_LIGHTS * percent / 100;
  int lightsOn = NUM_LIFT_LIGHTS - lights;
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn; x++) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::White;
    leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::White;
  }
  leds[NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS-lightsOn] = CRGB::Blue;
  leds[NUM_DRIVE_LIGHTS + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::Blue;
  FastLED.show();
}

void teleopDisabled() {
  pulseDriveTrain();
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS; x+=2) {
    if (x >= NUM_LEDS) break;
      leds[x] = CRGB::White;
      leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::White;
    }
    for(int x=NUM_DRIVE_LIGHTS+1; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS; x+=2) {
    if (x >= NUM_LEDS) break;
      leds[x] = CRGB::Green;
      leds[x + NUM_DRIVE_LIGHTS + NUM_LIFT_LIGHTS] = CRGB::Green;
    }
  FastLED.show();
  delay(100);
}

void disconnected2() {
  CRGB blue  = CRGB(13, 91, 223);
  CRGB gold = CRGB(215, 180, 36);
  CRGB red = CRGB(128, 3, 5);
/*
  CRGB red = CRGB(255,0,0);
  CRGB gold = CRGB(255,0,0);
  CRGB blue = CRGB(255,0,0);
*/
  CRGB pattern[12];
  pattern[0] = pattern[1] = pattern[2] = red;
  pattern[3] = CRGB::Black;
  pattern[4] = pattern[5] = pattern[6] = gold;
  pattern[7] = CRGB::Black;
  pattern[8] = pattern[9] = pattern[10] = blue;
  pattern[11] = CRGB::Black;

  for (int start=0;start<12;start++) {
    for (int x=0;x<NUM_LEDS;x++) {
      int y = (start+(x%12))%12;
      if (x >= NUM_LEDS) break;
      leds[x] = pattern[y];
    }
    FastLED.show();
    delay(50);
  }
}

void disconnected() { 
  // Turn the LED on, then pause
  CRGB blue  = CRGB(13, 91, 223);
  CRGB gold = CRGB(215, 180, 36);
  CRGB red = CRGB(128, 3, 5);
  for (int i=0;i<NUM_LEDS-10;i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = blue;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = blue;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = blue;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = CRGB::Black;
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = gold; //Red
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = gold;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = gold;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = CRGB::Black;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = red; //Blue
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = red;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = red;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = CRGB::Black;
  }
  FastLED.show();
  if (mode != DISCONNECTED) return;
  delay(75);
  for (int i=0;i<NUM_LEDS-10;i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = blue;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = blue;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = CRGB::Black;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = gold;
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = gold; //Red
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = gold;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = CRGB::Black;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = red;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = red; //Blue
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = red;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = CRGB::Black;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = blue;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0;i<NUM_LEDS-10;i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = blue;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = CRGB::Black;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = gold;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = gold;
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = gold; //Red
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = CRGB::Black;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = red;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = red;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = red; //Blue
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = CRGB::Black;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = blue;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = blue;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0;i<NUM_LEDS-10;i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = CRGB::Black;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = gold;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = gold;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = gold;
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = CRGB::Black; //Red
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = red;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = red;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = red;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = CRGB::Black; //Blue
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = blue;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = blue;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = blue;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0;i<NUM_LEDS-10;i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = gold;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = gold;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = gold;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = CRGB::Black;
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = red; //Red
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = red;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = red;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = CRGB::Black;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = blue; //Blue
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = blue;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = blue;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = CRGB::Black;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  // Now turn the LED off, then pause
  for (int i=0;i<NUM_LEDS-10;i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = gold;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = gold;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = CRGB::Black;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = red;
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = red; //Red
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = red;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = CRGB::Black; //Blue
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = blue;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = blue;
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = blue;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = CRGB::Black;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = gold;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0; i<NUM_LEDS-10; i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = gold;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = CRGB::Black;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = red;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = red; //Red
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = red; //Blue
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = CRGB::Black;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = blue;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = blue;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = blue;
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = CRGB::Black;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = gold;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = gold;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0; i<NUM_LEDS-10; i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = CRGB::Black;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = red;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = red;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = red; //Red
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = CRGB::Black; //Blue
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = blue;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = blue;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = blue;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = CRGB::Black;
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = gold;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = gold;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = gold;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0; i<NUM_LEDS-10; i+=12) {
    if (i+2 >= NUM_LEDS) break;
    leds[i] = leds[i+1] = leds[i+2] = red;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = CRGB::Black;
    if (i+6 >= NUM_LEDS) break;
    leds[i+4] = leds[i+5] = leds[i+6] = blue;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = CRGB::Black;
    if (i+10 >= NUM_LEDS) break;
    leds[i+8] = leds[i+9] = leds[i+10] = gold;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = CRGB::Black;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0; i<NUM_LEDS-10; i+=12) {
    if (i+1 >= NUM_LEDS) break;
    leds[i] = leds[i+1] = red;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = CRGB::Black;
    if (i+5 >= NUM_LEDS) break;
    leds[i+3] = leds[i+4] = leds[i+5] = blue;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = CRGB::Black;
    if (i+8 >= NUM_LEDS) break;
    leds[i+7] = leds[i+8] = leds[i+9] = gold;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = CRGB::Black;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = red;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0; i<NUM_LEDS-10; i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = red;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = CRGB::Black;
    if (i+4 >= NUM_LEDS) break;
    leds[i+2] = leds[i+3] = leds[i+4] = blue;
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = CRGB::Black;
    if (i+8 >= NUM_LEDS) break;
    leds[i+6] = leds[i+7] = leds[i+8] = gold;
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = CRGB::Black;
    if (i+11 >= NUM_LEDS) break;
    leds[i+10] = leds[i+11] = red;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0; i<NUM_LEDS-10; i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = CRGB::Black;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = blue;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = blue;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = blue; //Red
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = CRGB::Black; //Blue
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = gold;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = gold;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = gold;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = CRGB::Black;
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = red;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = red;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = red;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
}

void on(int t) {
  CRGB blue = CRGB(13, 91, 223);
  CRGB gold = CRGB(255, 255, 36);
  CRGB red = CRGB(128, 3, 5);
  for (int z=0; z<20; z++) {
  y = random(NUM_LEDS-3);
  for(int x=y; x<y+3; x++) {
    if (x >= NUM_LEDS) break;
    int color = random(3);
    switch (color) {
      case 0:
        leds[x] = blue;
        break;
      case 1:
        leds[x] = gold;
        break;
      case 2:
        leds[x] = red;
        break;
      default:
      leds[x] = CRGB::Black;
      break;
    }
  }
  }
  FastLED.show();
  delay(t);
}

void off(int t) {
  for(int x=0; x<NUM_LEDS; x++) {
    if (x >= NUM_LEDS) break;
    leds[x]= CRGB::Black;
  }
  FastLED.show();
  delay(t);
}

void finished() {
  a = random(4);
  switch (a) {
    case 0:
      //Hi, Fischler
      b = 19;
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(300);
      off(75);
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(75);
      on(300);
      off(75);
      on(300);
      off(500);
      if (mode != FINISHED) return;
      
      on(75);
      off(75);
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(300);
      off(75);
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(500);
      if (mode != FINISHED) return;
      break;
    case 1:
      //Ubernerds
      b = 22;
      on(75);
      off(75);
      on(75);
      off(75);
      on(300);
      off(300);
      if (mode != FINISHED) return;
  
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(300);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(500);
      if (mode != FINISHED) return;
      break;
    case 2:
      //Uberbots
      b = 22;
      on(75);
      off(75);
      on(75);
      off(75);
      on(300);
      off(300);
      if (mode != FINISHED) return;
  
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(300);
      off(75);
      on(300);
      off(75);
      on(300);
      off(300);
      if (mode != FINISHED) return;
  
      on(300); 
      off(300);
      if (mode != FINISHED) return;
  
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(500);
      if (mode != FINISHED) return;
      break;
    case 3:
      //Niezreicki's rule
      b = 16;
      on(300);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(300);
      off(75);
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
  on(75);
  off(75);
  on(300);
  off(75);
  on(75);
  off(300);
  if (mode != FINISHED) return;
  
  on(75);
  off(300);
  if (mode != FINISHED) return;
  
  on(75);
  off(75);
  on(75);
  off(300);
  if (mode != FINISHED) return;
  
  on(300);
  off(75);
  on(75);
  off(75);
  on(300);
  off(75);
  on(75);
  off(300);
  if (mode != FINISHED) return;
  
  on(300);
  off(75);
  on(75);
  off(75);
  on(300);
  off(300);
  if (mode != FINISHED) return;
  
  on(75);
  off(75);
  on(75);
  off(300);
  if (mode != FINISHED) return;
  
  on(75);
  off(75);
  on(300);
  off(75);
  on(300);
  off(75);
  on(300);
  off(75);
  on(300);
  off(75);
  on(75);
  off(300);
  if (mode != FINISHED) return;
  
  on(75);
  off(75);
  on(75);
  off(75);
  on(75);
  off(500);
  if (mode != FINISHED) return;
  
  on(75);
  off(75);
  on(300);
  off(75);
  on(75);
  off(300);
  if (mode != FINISHED) return;
  
  on(75);
  off(75);
  on(75);
  off(75);
  on(300);
  off(300);
  if (mode != FINISHED) return;
  
  on(75);
  off(75);
  on(300);
  off(75);
  on(75);
  off(75);
  on(75);
  off(300);
  if (mode != FINISHED) return;
  
  on(75);
  off(500);
  if (mode != FINISHED) return;
      break;
    default:
    b = 30;
      for (int x=0; x<NUM_LEDS; x++) {
        if (x >= NUM_LEDS) break;
        leds[x] = CRGB::Black;
      }
      break;
  }
  for (int x=0; x<b; x++) {
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+4 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = CRGB::Red;
    if (z+9 >= NUM_LEDS) break;
    leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = CRGB::White;
    if (z+14 >= NUM_LEDS) break;
    leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = CRGB::Blue;
    if (z+19 >= NUM_LEDS) break;
    leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+3 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = CRGB::Red;
    if (z+8 >= NUM_LEDS) break;
    leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = CRGB::White;
    if (z+13 >= NUM_LEDS) break;
    leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = CRGB::Blue;
    if (z+18 >= NUM_LEDS) break;
    leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = CRGB::Black;
    if (z+19 >= NUM_LEDS) break;
    leds[z+19] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+2 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = CRGB::Red;
    if (z+7 >= NUM_LEDS) break;
    leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = CRGB::White;
    if (z+12 >= NUM_LEDS) break;
    leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = CRGB::Blue;
    if (z+17 >= NUM_LEDS) break;
    leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = CRGB::Black;
    if (z+19 >= NUM_LEDS) break;
    leds[z+18] = leds[z+19] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+1 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = CRGB::Red;
    if (z+6 >= NUM_LEDS) break;
    leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = CRGB::White;
    if (z+11 >= NUM_LEDS) break;
    leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = CRGB::Blue;
    if (z+16 >= NUM_LEDS) break;
    leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = CRGB::Black;
    if (z+19 >= NUM_LEDS) break;
    leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z >= NUM_LEDS) break;
    leds[z] = CRGB::Red;
    if (z+5 >= NUM_LEDS) break;
    leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = CRGB::White;
    if (z+10 >= NUM_LEDS) break;
    leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = CRGB::Blue;
    if (z+15 >= NUM_LEDS) break;
    leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = CRGB::Black;
    if (z+19 >= NUM_LEDS) break;
    leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+4 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = CRGB::White;
    if (z+9 >= NUM_LEDS) break;
    leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = CRGB::Blue;
    if (z+14 >= NUM_LEDS) break;
    leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = CRGB::Black;
    if (z+19 >= NUM_LEDS) break;
    leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+3 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = CRGB::White;
    if (z+8 >= NUM_LEDS) break;
    leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = CRGB::Blue;
    if (z+13 >= NUM_LEDS) break;
    leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = CRGB::Black;
    if (z+18 >= NUM_LEDS) break;
    leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = CRGB::Red;
    if (z+19 >= NUM_LEDS) break;
    leds[z+19] = CRGB::White;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+2 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = CRGB::White;
    if (z+7 >= NUM_LEDS) break;
    leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = CRGB::Blue;
    if (z+12 >= NUM_LEDS) break;
    leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = CRGB::Black;
    if (z+17 >= NUM_LEDS) break;
    leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = CRGB::Red;
    if (z+19 >= NUM_LEDS) break;
    leds[z+18] = leds[z+19] = CRGB::White;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+1 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = CRGB::White;
    if (z+6 >= NUM_LEDS) break;
    leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = CRGB::Blue;
    if (z+11 >= NUM_LEDS) break;
    leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = CRGB::Black;
    if (z+16 >= NUM_LEDS) break;
    leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = CRGB::Red;
    if (z+19 >= NUM_LEDS) break;
    leds[z+17] = leds[z+18] = leds[z+19] = CRGB::White;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z >= NUM_LEDS) break;
    leds[z] = CRGB::White;
    if (z+5 >= NUM_LEDS) break;
    leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = CRGB::Blue;
    if (z+10 >= NUM_LEDS) break;
    leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = CRGB::Black;
    if (z+15 >= NUM_LEDS) break;
    leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = CRGB::Red;
    if (z+19 >= NUM_LEDS) break;
    leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::White;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+4 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = CRGB::Blue;
    if (z+9 >= NUM_LEDS) break;
    leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = CRGB::Black;
    if (z+14 >= NUM_LEDS) break;
    leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = CRGB::Red;
    if (z+19 >= NUM_LEDS) break;
    leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::White;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+3 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = CRGB::Blue;
    if (z+8 >= NUM_LEDS) break;
    leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = CRGB::Black;
    if (z+13 >= NUM_LEDS) break;
    leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = CRGB::Red;
    if (z+18 >= NUM_LEDS) break;
    leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = CRGB::White;
    if (z+19 >= NUM_LEDS) break;
    leds[z+19] = CRGB::Blue;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+2 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = CRGB::Blue;
    if (z+7 >= NUM_LEDS) break;
    leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = CRGB::Black;
    if (z+12 >= NUM_LEDS) break;
    leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = CRGB::Red;
    if (z+17 >= NUM_LEDS) break;
    leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = CRGB::White;
    if (z+19 >= NUM_LEDS) break;
    leds[z+18] = leds[z+19] = CRGB::Blue;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+1 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = CRGB::Blue;
    if (z+6 >= NUM_LEDS) break;
    leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = CRGB::Black;
    if (z+11 >= NUM_LEDS) break;
    leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = CRGB::Red;
    if (z+16 >= NUM_LEDS) break;
    leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = CRGB::White;
    if (z+19 >= NUM_LEDS) break;
    leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Blue;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z >= NUM_LEDS) break;
    leds[z] = CRGB::Blue;
    if (z+5 >= NUM_LEDS) break;
    leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = CRGB::Black;
    if (z+10 >= NUM_LEDS) break;
    leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = CRGB::Red;
    if (z+15 >= NUM_LEDS) break;
    leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = CRGB::White;
    if (z+19 >= NUM_LEDS) break;
    leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Blue;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+4 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = CRGB::Black;
    if (z+9 >= NUM_LEDS) break;
    leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = CRGB::Red;
    if (z+14 >= NUM_LEDS) break;
    leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = CRGB::White;
    if (z+19 >= NUM_LEDS) break;
    leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Blue;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+3 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = CRGB::Black;
    if (z+8 >= NUM_LEDS) break;
    leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = CRGB::Red;
    if (z+13 >= NUM_LEDS) break;
    leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = CRGB::White;
    if (z+18 >= NUM_LEDS) break;
    leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = CRGB::Blue;
    if (z+19 >= NUM_LEDS) break;
    leds[z+19] = CRGB::Black;
    if (z+4 >= NUM_LEDS) break;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+2 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = CRGB::Black;
    if (z+7 >= NUM_LEDS) break;
    leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = CRGB::Red;
    if (z+12 >= NUM_LEDS) break;
    leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = CRGB::White;
    if (z+17 >= NUM_LEDS) break;
    leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = CRGB::Blue;
    if (z+19 >= NUM_LEDS) break;
    leds[z+18] = leds[z+19] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+1 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = CRGB::Black;
    if (z+6 >= NUM_LEDS) break;
    leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = CRGB::Red;
    if (z+11 >= NUM_LEDS) break;
    leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = CRGB::White;
    if (z+16 >= NUM_LEDS) break;
    leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = CRGB::Blue;
    if (z+19 >= NUM_LEDS) break;
    leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z >= NUM_LEDS) break;
    leds[z] = CRGB::Black;
    if (z+5 >= NUM_LEDS) break;
    leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = CRGB::Red;
    if (z+10 >= NUM_LEDS) break;
    leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = CRGB::White;
    if (z+15 >= NUM_LEDS) break;
    leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = CRGB::Blue;
    if (z+19 >= NUM_LEDS) break;
    leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
 }
}

void errorMode() {
  CRGB yellow = CRGB(255, 32, 0);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = yellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = yellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = yellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = yellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = yellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = yellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = yellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = yellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    if (x >= NUM_LEDS) break;
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
}

void setState() {
  // Make a copy in case more input comes in that moves the counter
  int c = counter;
  if (c == 0) return;
  char *b = (char *)malloc(c);
  strcpy(b,charCommand);
  b[c] = 0;

  // Make sure c is long enough to validate
  if (c<7) {
    free(b);
    return;
  }

  // Validate format
  int j = 0;
  if (b[0] != '<') {
    free(b);
    return;
  }
  if (b[2] != ':') {
    free(b);
    return;
  }
  if (b[5] != ';') {
    free(b);
    return;
  }
  if (b[c-1] != '>') {
    free(b);
    return;
  }

  // Get the color
  allianceColor = b[1];
  if (allianceColor != 'R' && allianceColor != 'B') {
    allianceColor = 'X';
  }

  // Get the state
  switch (charCommand[3]) {
  case 'S':
    switch (charCommand[4]) {
    case 'E':
      mode = TEST_ENABLED;
      break;
    case 'D':
      mode = TEST_DISABLED;
      break;
    default:
      free(b);
      return;
    }
    break;
  case 'A':
    switch (charCommand[4]) {
    case 'E':
      mode = AUTO_ENABLED;
      break;
    case 'D':
      mode = AUTO_DISABLED;
      break;
    default:
      free(b);
      return;
    }
    break;
  case 'T':
    switch (charCommand[4]) {
    case 'E':
      mode = TELEOP_ENABLED;
      break;
    case 'D':
      mode = TELEOP_DISABLED;
      break;
    default:
      free(b);
      return;
    }
    break;
  case 'D':
    if (charCommand[4] == 'C') {
      mode = DISCONNECTED;
    } else {
      free(b);
      return;
    }
    break;
  case 'F':
    if (charCommand[4] == 'N') {
      mode = FINISHED;
    } else {
      free(b);
      return;
    }
    break;
  case 'E':
    if (charCommand[4] == 'S') {
      mode = ERROR_MODE;
    } else {
      free(b);
      return;
    }
    break;
  case 'O':
    if (charCommand[4] == 'F') {
      mode = OFF;
    } else {
      free(b);
      return;
    }
  }

  // Parse the lights
  char *liftString = (char *)malloc(c-7);
  strcpy(liftString,&charCommand[6]);
  liftString[c-8] = 0;
  liftPosition = atof(liftString);
  free(liftString);
}

void receiveEvent(int howMany)
{
  while (1 <= Wire.available()) 
  {
    char incomingChar = Wire.read();
    if (incomingChar == '<')
    {
      counter = 0;
    }
    charCommand[counter] = incomingChar;
    counter++;
  }
  if (counter == 0 || charCommand[counter-1] != '>') {
    // fail
    return ;
  }
  setState();
}

void loop() {
  switch (mode) {
  case TEST_ENABLED:
    testEnabled();
    break;
  case TEST_DISABLED:
    testDisabled();
    break;
  case AUTO_ENABLED:
    autoEnabled();
    break;
  case AUTO_DISABLED:
    autoDisabled();
    break;
  case TELEOP_ENABLED:
    teleopEnabled();
    break;
  case TELEOP_DISABLED:
    teleopDisabled();
    break;
  case DISCONNECTED:
    disconnected2();
    break;
  case FINISHED:
    finished();
    break;
  case ERROR_MODE:
    errorMode();
    break;
  case OFF:
    off(0);
    break;
  }
}

