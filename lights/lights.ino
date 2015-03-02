#include "FastLED.h"
#include <stdlib.h>
#include <Wire.h>

char charCommand[32];
char* robotState;
int counter = 0;

#define NUM_LEDS 117
#define DRIVE_DATA_PIN 0
#define LIFT_DATA_PIN 11
#define MAX_LIFT 59
#define NUM_LIFT_LIGHTS 87
#define NUM_DRIVE_LIGHTS 30
CRGB leds[NUM_LEDS];
char allianceColor = 'B';
float liftPosition = 59.0;
int a, b, c, v, w, y, z;

boolean uninit = true;

CRGB orange = CRGB(191, 105, 0);
CRGB red = CRGB(191, 0, 0);
CRGB blue = CRGB(0, 0, 155);
CRGB white = CRGB(155, 155, 155);
CRGB yellow = CRGB(191, 101, 0);
CRGB green = CRGB(0, 96, 0);
CRGB teamBlue  = CRGB(13, 91, 223);
CRGB teamGold = CRGB(215, 180, 36);
CRGB teamRed = CRGB(128, 3, 5);

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
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  FastLED.addLeds<NEOPIXEL, DRIVE_DATA_PIN>(leds, NUM_DRIVE_LIGHTS);
  FastLED.addLeds<NEOPIXEL, LIFT_DATA_PIN>(&leds[NUM_DRIVE_LIGHTS], NUM_LIFT_LIGHTS);
}

void testEnabled() {
  for(int x=0; x<NUM_DRIVE_LIGHTS; x++) {
    leds[x] = orange;
  }
  int percent = liftPosition / MAX_LIFT * 100;
  int lights = NUM_LIFT_LIGHTS * percent / 100;
  int lightsOn = NUM_LIFT_LIGHTS - lights;
  int topOffset = NUM_LEDS-lightsOn;
  for(int x=NUM_DRIVE_LIGHTS; x<topOffset; x++) {
    leds[x] = white;
  }
  for(int x=topOffset; x<NUM_LEDS; x++) {
    leds[x] = CRGB::Black;
  }
  leds[NUM_LEDS-1] = orange;
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
      leds[x] = CRGB(frame, frame, 0);
  }
  int percent = liftPosition / MAX_LIFT * 100;
  int lights = NUM_LIFT_LIGHTS * percent / 100;
  int lightsOn =  NUM_LIFT_LIGHTS - lights;
  int topOffset = NUM_LEDS-lightsOn;
  for(int x=NUM_DRIVE_LIGHTS; x<topOffset; x++) {
    leds[x] = white;
  }
  for(int x=topOffset; x<NUM_LEDS; x++) {
    leds[x] = CRGB::Black;
  }
  leds[NUM_LEDS-1] = orange;
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
      break;
    case 'R':
      leds[x] = CRGB(frame, 0, 0);
      break;
    default:
      leds[x] = CRGB::Black;
      break;
    }
  }
}

void autoDisabled() {
  pulseDriveTrain();
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_LEDS; x+=2) {
      leds[x] = white;
    }
    for(int x=NUM_DRIVE_LIGHTS+1; x<NUM_LEDS; x+=2) {
      leds[x] = yellow;
    }
  leds[NUM_LEDS-1] = red;
  FastLED.show();
  delay(10);
}

void autoEnabled() {
  for(int x=0; x<NUM_DRIVE_LIGHTS; x++) {
    switch (allianceColor) {
    case 'B':
      leds[x] = blue;
      break;
    case 'R':
      leds[x] = red;
      break;
    default:
      leds[x] = CRGB::Black;
      break;
    }
  }
  int percent = liftPosition / MAX_LIFT * 100;
  int lights = NUM_LIFT_LIGHTS * percent / 100;
  int lightsOn = NUM_LIFT_LIGHTS - lights;
  int topOffset = NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn;
  for(int x=NUM_DRIVE_LIGHTS; x<topOffset; x++) {
    leds[x] = white;
  }
  for(int x=topOffset; x<NUM_LEDS; x++) {
    leds[x] = CRGB::Black;
  }
  leds[NUM_LEDS-1] = red;
  FastLED.show();
}

void teleopEnabled() {
  for(int x=0; x<NUM_DRIVE_LIGHTS; x++) {
    switch (allianceColor) {
    case 'B':
      leds[x] = blue;
      break;
    case 'R':
      leds[x] = red;
      break;
    default:
      leds[x] = CRGB::Black;
      break;
    }
  }
  int percent = liftPosition / MAX_LIFT * 100;
  int lights = NUM_LIFT_LIGHTS * percent / 100;
  int lightsOn = NUM_LIFT_LIGHTS - lights;
  int topOffset = NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn;
  for(int x=NUM_DRIVE_LIGHTS; x<topOffset; x++) {
    leds[x] = white;
  }
  for(int x=topOffset; x<NUM_LEDS; x++) {
    leds[x] = CRGB::Black;
  }
  leds[NUM_LEDS-1] = blue;
  FastLED.show();
}

void teleopDisabled() {
  pulseDriveTrain();
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_LEDS; x+=2) {
      leds[x] = white;
    }
    for(int x=NUM_DRIVE_LIGHTS+1; x<NUM_LEDS; x+=2) {
      leds[x] = green;
    }
  leds[NUM_LEDS-1] = blue;
  FastLED.show();
  delay(100);
}

void disconnected2() {
  CRGB pattern[12];
  pattern[0] = pattern[1] = pattern[2] = teamRed;
  pattern[3] = CRGB::Black;
  pattern[4] = pattern[5] = pattern[6] = teamGold;
  pattern[7] = CRGB::Black;
  pattern[8] = pattern[9] = pattern[10] = teamBlue;
  pattern[11] = CRGB::Black;

  for (int start=0;start<12;start++) {
    for (int x=0;x<NUM_DRIVE_LIGHTS;x++) {
      int y = (start+x)%12;
      leds[x] = pattern[y];
    }
    for (int x=NUM_LEDS-1;x>=NUM_DRIVE_LIGHTS;x--) {
      int y = (start+x)%12;
      leds[x] = pattern[y];
    }
    FastLED.show();
    delay(50);
  }
}

void disconnected() { 
  // Turn the LED on, then pause
  for (int i=0;i<NUM_LEDS-10;i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = teamBlue;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = teamBlue;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = teamBlue;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = CRGB::Black;
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = teamGold; //teamRed
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = teamGold;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = teamGold;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = CRGB::Black;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = teamRed; //teamBlue
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = teamRed;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = teamRed;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = CRGB::Black;
  }
  FastLED.show();
  if (mode != DISCONNECTED) return;
  delay(75);
  for (int i=0;i<NUM_LEDS-10;i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = teamBlue;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = teamBlue;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = CRGB::Black;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = teamGold;
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = teamGold; //teamRed
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = teamGold;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = CRGB::Black;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = teamRed;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = teamRed; //teamBlue
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = teamRed;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = CRGB::Black;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = teamBlue;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0;i<NUM_LEDS-10;i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = teamBlue;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = CRGB::Black;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = teamGold;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = teamGold;
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = teamGold; //teamRed
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = CRGB::Black;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = teamRed;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = teamRed;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = teamRed; //teamBlue
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = CRGB::Black;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = teamBlue;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = teamBlue;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0;i<NUM_LEDS-10;i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = CRGB::Black;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = teamGold;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = teamGold;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = teamGold;
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = CRGB::Black; //teamRed
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = teamRed;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = teamRed;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = teamRed;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = CRGB::Black; //teamBlue
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = teamBlue;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = teamBlue;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = teamBlue;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0;i<NUM_LEDS-10;i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = teamGold;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = teamGold;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = teamGold;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = CRGB::Black;
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = teamRed; //teamRed
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = teamRed;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = teamRed;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = CRGB::Black;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = teamBlue; //teamBlue
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = teamBlue;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = teamBlue;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = CRGB::Black;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  // Now turn the LED off, then pause
  for (int i=0;i<NUM_LEDS-10;i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = teamGold;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = teamGold;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = CRGB::Black;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = teamRed;
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = teamRed; //teamRed
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = teamRed;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = CRGB::Black; //teamBlue
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = teamBlue;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = teamBlue;
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = teamBlue;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = CRGB::Black;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = teamGold;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0; i<NUM_LEDS-10; i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = teamGold;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = CRGB::Black;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = teamRed;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = teamRed; //teamRed
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = teamRed; //teamBlue
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = CRGB::Black;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = teamBlue;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = teamBlue;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = teamBlue;
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = CRGB::Black;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = teamGold;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = teamGold;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0; i<NUM_LEDS-10; i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = CRGB::Black;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = teamRed;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = teamRed;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = teamRed; //teamRed
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = CRGB::Black; //teamBlue
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = teamBlue;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = teamBlue;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = teamBlue;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = CRGB::Black;
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = teamGold;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = teamGold;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = teamGold;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0; i<NUM_LEDS-10; i+=12) {
    if (i+2 >= NUM_LEDS) break;
    leds[i] = leds[i+1] = leds[i+2] = teamRed;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = CRGB::Black;
    if (i+6 >= NUM_LEDS) break;
    leds[i+4] = leds[i+5] = leds[i+6] = teamBlue;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = CRGB::Black;
    if (i+10 >= NUM_LEDS) break;
    leds[i+8] = leds[i+9] = leds[i+10] = teamGold;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = CRGB::Black;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0; i<NUM_LEDS-10; i+=12) {
    if (i+1 >= NUM_LEDS) break;
    leds[i] = leds[i+1] = teamRed;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = CRGB::Black;
    if (i+5 >= NUM_LEDS) break;
    leds[i+3] = leds[i+4] = leds[i+5] = teamBlue;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = CRGB::Black;
    if (i+8 >= NUM_LEDS) break;
    leds[i+7] = leds[i+8] = leds[i+9] = teamGold;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = CRGB::Black;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = teamRed;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0; i<NUM_LEDS-10; i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = teamRed;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = CRGB::Black;
    if (i+4 >= NUM_LEDS) break;
    leds[i+2] = leds[i+3] = leds[i+4] = teamBlue;
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = CRGB::Black;
    if (i+8 >= NUM_LEDS) break;
    leds[i+6] = leds[i+7] = leds[i+8] = teamGold;
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = CRGB::Black;
    if (i+11 >= NUM_LEDS) break;
    leds[i+10] = leds[i+11] = teamRed;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
  for (int i=0; i<NUM_LEDS-10; i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = CRGB::Black;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = teamBlue;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = teamBlue;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = teamBlue; //teamRed
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = CRGB::Black; //teamBlue
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = teamGold;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = teamGold;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = teamGold;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = CRGB::Black;
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = teamRed;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = teamRed;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = teamRed;
  }
  FastLED.show();
  delay(75);
  if (mode != DISCONNECTED) return;
}

void on(int t) {
  for (int z=0; z<20; z++) {
  y = random(NUM_LEDS-3);
  for(int x=y; x<y+3; x++) {
    if (x >= NUM_LEDS) break;
    int color = random(3);
    switch (color) {
      case 0:
        leds[x] = teamBlue;
        break;
      case 1:
        leds[x] = teamGold;
        break;
      case 2:
        leds[x] = teamRed;
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
          leds[x] = CRGB::Black;
      }
      break;
  }
  for (int x=0; x<b; x++) {
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+4 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = red;
    if (z+9 >= NUM_LEDS) break;
    leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = white;
    if (z+14 >= NUM_LEDS) break;
    leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = blue;
    if (z+19 >= NUM_LEDS) break;
    leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+3 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = red;
    if (z+8 >= NUM_LEDS) break;
    leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = white;
    if (z+13 >= NUM_LEDS) break;
    leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = blue;
    if (z+18 >= NUM_LEDS) break;
    leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = CRGB::Black;
    if (z+19 >= NUM_LEDS) break;
    leds[z+19] = red;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+2 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = red;
    if (z+7 >= NUM_LEDS) break;
    leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = white;
    if (z+12 >= NUM_LEDS) break;
    leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = blue;
    if (z+17 >= NUM_LEDS) break;
    leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = CRGB::Black;
    if (z+19 >= NUM_LEDS) break;
    leds[z+18] = leds[z+19] = red;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+1 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = red;
    if (z+6 >= NUM_LEDS) break;
    leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = white;
    if (z+11 >= NUM_LEDS) break;
    leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = blue;
    if (z+16 >= NUM_LEDS) break;
    leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = CRGB::Black;
    if (z+19 >= NUM_LEDS) break;
    leds[z+17] = leds[z+18] = leds[z+19] = red;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z >= NUM_LEDS) break;
    leds[z] = red;
    if (z+5 >= NUM_LEDS) break;
    leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = white;
    if (z+10 >= NUM_LEDS) break;
    leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = blue;
    if (z+15 >= NUM_LEDS) break;
    leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = CRGB::Black;
    if (z+19 >= NUM_LEDS) break;
    leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = red;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+4 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = white;
    if (z+9 >= NUM_LEDS) break;
    leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = blue;
    if (z+14 >= NUM_LEDS) break;
    leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = CRGB::Black;
    if (z+19 >= NUM_LEDS) break;
    leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = red;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+3 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = white;
    if (z+8 >= NUM_LEDS) break;
    leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = blue;
    if (z+13 >= NUM_LEDS) break;
    leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = CRGB::Black;
    if (z+18 >= NUM_LEDS) break;
    leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = red;
    if (z+19 >= NUM_LEDS) break;
    leds[z+19] = white;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+2 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = white;
    if (z+7 >= NUM_LEDS) break;
    leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = blue;
    if (z+12 >= NUM_LEDS) break;
    leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = CRGB::Black;
    if (z+17 >= NUM_LEDS) break;
    leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = red;
    if (z+19 >= NUM_LEDS) break;
    leds[z+18] = leds[z+19] = white;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+1 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = white;
    if (z+6 >= NUM_LEDS) break;
    leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = blue;
    if (z+11 >= NUM_LEDS) break;
    leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = CRGB::Black;
    if (z+16 >= NUM_LEDS) break;
    leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = red;
    if (z+19 >= NUM_LEDS) break;
    leds[z+17] = leds[z+18] = leds[z+19] = white;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z >= NUM_LEDS) break;
    leds[z] = white;
    if (z+5 >= NUM_LEDS) break;
    leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = blue;
    if (z+10 >= NUM_LEDS) break;
    leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = CRGB::Black;
    if (z+15 >= NUM_LEDS) break;
    leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = red;
    if (z+19 >= NUM_LEDS) break;
    leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = white;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+4 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = blue;
    if (z+9 >= NUM_LEDS) break;
    leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = CRGB::Black;
    if (z+14 >= NUM_LEDS) break;
    leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = red;
    if (z+19 >= NUM_LEDS) break;
    leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = white;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+3 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = blue;
    if (z+8 >= NUM_LEDS) break;
    leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = CRGB::Black;
    if (z+13 >= NUM_LEDS) break;
    leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = red;
    if (z+18 >= NUM_LEDS) break;
    leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = white;
    if (z+19 >= NUM_LEDS) break;
    leds[z+19] = blue;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+2 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = blue;
    if (z+7 >= NUM_LEDS) break;
    leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = CRGB::Black;
    if (z+12 >= NUM_LEDS) break;
    leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = red;
    if (z+17 >= NUM_LEDS) break;
    leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = white;
    if (z+19 >= NUM_LEDS) break;
    leds[z+18] = leds[z+19] = blue;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+1 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = blue;
    if (z+6 >= NUM_LEDS) break;
    leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = CRGB::Black;
    if (z+11 >= NUM_LEDS) break;
    leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = red;
    if (z+16 >= NUM_LEDS) break;
    leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = white;
    if (z+19 >= NUM_LEDS) break;
    leds[z+17] = leds[z+18] = leds[z+19] = blue;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z >= NUM_LEDS) break;
    leds[z] = blue;
    if (z+5 >= NUM_LEDS) break;
    leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = CRGB::Black;
    if (z+10 >= NUM_LEDS) break;
    leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = red;
    if (z+15 >= NUM_LEDS) break;
    leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = white;
    if (z+19 >= NUM_LEDS) break;
    leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = blue;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+4 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = CRGB::Black;
    if (z+9 >= NUM_LEDS) break;
    leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = red;
    if (z+14 >= NUM_LEDS) break;
    leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = white;
    if (z+19 >= NUM_LEDS) break;
    leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = blue;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    if (z+3 >= NUM_LEDS) break;
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = CRGB::Black;
    if (z+8 >= NUM_LEDS) break;
    leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = red;
    if (z+13 >= NUM_LEDS) break;
    leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = white;
    if (z+18 >= NUM_LEDS) break;
    leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = blue;
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
    leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = red;
    if (z+12 >= NUM_LEDS) break;
    leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = white;
    if (z+17 >= NUM_LEDS) break;
    leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = blue;
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
    leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = red;
    if (z+11 >= NUM_LEDS) break;
    leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = white;
    if (z+16 >= NUM_LEDS) break;
    leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = blue;
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
    leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = red;
    if (z+10 >= NUM_LEDS) break;
    leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = white;
    if (z+15 >= NUM_LEDS) break;
    leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = blue;
    if (z+19 >= NUM_LEDS) break;
    leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  if (mode != FINISHED) return;
 }
}

void errorMode() {
  CRGB errorYellow = CRGB(255, 32, 0);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
}

void setState() {
 if ((charCommand[0] == '<') && (charCommand[2] == ':') && (charCommand[5] == ';'))
 {
  char robotState[] = {charCommand[3], charCommand[4], '\0'};
    if (robotState == "SE") // test enabled
    {
      mode = 1;
    }
    else if (robotState == "SD") // test disabled
    {
      mode = 2;
    }
    else if (robotState == "AE") // auto enabled
    {
      mode = 3;
    }
    else if (robotState == "AD") // auto disabled
    {
      mode = 4;
    }
    else if (robotState == "TE") // teleop enabled
    {
      mode = 5;
    }
    else if (robotState == "TD") // teleop disabled
    {
      mode = 6;
    }
    else if (robotState == "DC") // disconnected
    {
      mode = 7;
    }
    else if (robotState == "FN") // finished
    {
      mode = 8;
    }
    else if (robotState == "ES") // error/emergency stop
    {
      mode = 9;
    }
    else if (robotState == "OF") // lights off
    {
      mode = 10;
    }
    
  allianceColor = charCommand[1];
  char liftString[12];
  int j = 0;
  while (charCommand[j+6] != '>' && j<11)
  {
    liftString[j] = {charCommand[j+6]};
    j++;
  }
  liftString[j+1] = '\0';
  liftPosition = atof(liftString);
 }
}

void receiveEvent(int howMany)
{
  while (0 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
Serial.print(c);
    if (c == '<' || counter > 31)
    {
      counter = 0;
    }
    charCommand[counter++] = c;
  }
Serial.println();
  setState();
}

void loop() {
if (uninit) {
delay(5000);
uninit = false;
Serial.println("start");
}
/*
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
*/
disconnected2();
/*
for (int i=0;i<NUM_DRIVE_LIGHTS;i++) {
  if (allianceColor == 'R') {
    leds[i] = red;
  }
  else {
    leds[i] = blue;
  }
}
for (int i=NUM_DRIVE_LIGHTS;i<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS;i++) {
  leds[i] = white;
}
FastLED.show();
  delay(20);
*/
}

