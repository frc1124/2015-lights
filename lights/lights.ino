#include "FastLED.h"
#include <stdlib.h>
#include <Wire.h>
#include <elapsedMillis.h>

char charCommand[32];
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
CRGB gray = CRGB(10, 10, 10);

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
  elapsedMillis timeElapsed = 0;
  for(int x=0; x<NUM_DRIVE_LIGHTS; x++) {
    leds[x] = orange; //set drive lights orange
  }
  //find number of lights needed to light up to lift position
  if (liftPosition > MAX_LIFT) liftPosition = MAX_LIFT;
  if (liftPosition < 0) liftPosition = 0;
  int percent = liftPosition / MAX_LIFT * 100;
  int lights = NUM_LIFT_LIGHTS * percent / 100;
  int lightsOn = NUM_LIFT_LIGHTS - lights;
  int topOffset = NUM_LEDS-lightsOn;
  for(int x=NUM_DRIVE_LIGHTS; x<topOffset; x++) {
    leds[x] = gray; //set lights white up to there
    if (x == topOffset-2){
    leds[x] = orange;
    }
    if (x == topOffset-1){
    leds[x] = orange;
    }
    if (x == topOffset-27){
    leds[x] = orange;
    }
    if (x == topOffset-28){
    leds[x] = orange;
    }
    if (x == topOffset-53){
    leds[x] = orange;
    }
    if (x == topOffset-54){
    leds[x] = orange;
    }
    if (x == topOffset-79){
    leds[x] = orange;
    }
    if (x == topOffset-80){
    leds[x] = orange;
    }
  }
  for(int x=topOffset; x<NUM_LEDS; x++) {
    leds[x] = CRGB::Black; //reset all lights above that to black
  }
/*  leds[NUM_DRIVE_LIGHTS+9] = orange;
  leds[NUM_DRIVE_LIGHTS+10] = orange;
  leds[NUM_DRIVE_LIGHTS+36] = orange;
  leds[NUM_DRIVE_LIGHTS+37] = orange;
  leds[NUM_DRIVE_LIGHTS+63] = orange;
  leds[NUM_DRIVE_LIGHTS+64] = orange;
  leds[NUM_LEDS-2] = orange; */
  leds[NUM_LEDS-1] = orange; //set top-most light to orange
  FastLED.show();
}

void testDisabled() {
  elapsedMillis timeElapsed = 0;
  //find number of lights needed to light up to lift position
  if (liftPosition > MAX_LIFT) liftPosition = MAX_LIFT;
  if (liftPosition < 0) liftPosition = 0;
  int percent = liftPosition / MAX_LIFT * 100;
  int lights = NUM_LIFT_LIGHTS * percent / 100;
  int lightsOn =  NUM_LIFT_LIGHTS - lights;
  int topOffset = NUM_LEDS-lightsOn;
  for(int x=NUM_DRIVE_LIGHTS; x<topOffset; x++) {
    leds[x] = gray;
    if (x == topOffset-1){
    leds[x] = orange;
    }
    if (x == topOffset-2){
    leds[x] = orange;
    }
    if (x == topOffset-27){
    leds[x] = orange;
    }
    if (x == topOffset-28){
    leds[x] = orange;
    }
    if (x == topOffset-53){
    leds[x] = orange;
    }
    if (x == topOffset-54){
    leds[x] = orange;
    }
    if (x == topOffset-79){
    leds[x] = orange;
    }
    if (x == topOffset-80){
    leds[x] = orange;
    }
  }
  for(int x=topOffset; x<NUM_LEDS; x++) {
    leds[x] = CRGB::Black; //set all lights above off
  }
/*  leds[NUM_DRIVE_LIGHTS] = orange;
  leds[NUM_DRIVE_LIGHTS+9] = orange;
  leds[NUM_DRIVE_LIGHTS+10] = orange;
  leds[NUM_DRIVE_LIGHTS+36] = orange;
  leds[NUM_DRIVE_LIGHTS+37] = orange;
  leds[NUM_DRIVE_LIGHTS+63] = orange;
  leds[NUM_DRIVE_LIGHTS+64] = orange;
  leds[NUM_LEDS-2] = orange; */
  leds[NUM_LEDS-1] = orange; //set top-most light to orange
  FastLED.show();
  while(timeElapsed < 101) {
  }
  timeElapsed = 0;
}

void pulseDriveTrain(byte driveColor) {
  elapsedMillis timeElapsed = 0;
  //Change parameters of function to take a char so testDisabled can also use it
  if (frame > 220 && driveColor != 2) c = 1; //if brightness is too high, make it step down
  if (frame < 25) c = 0; //if brightness is too low, make it step up
  if (frame > 134 && driveColor == 2) c = 2; //if brightness is too high and is testDisabled, make it step down
  for(int x=0; x<NUM_DRIVE_LIGHTS; x++) {
    switch (c) {
      case 0:
        frame = frame + 1; //step up
        break;
      case 1:
        frame = frame - 1; //step down
        break;
      case 2:
        frame = frame - 1; //step down
        break; 
      default:
        frame++; //just go up because we don't know what to do
        //Change to frame = 0
      break;
    }
    //check allianceColor to set drive train to pulse in the right color
    //Change this so it pulse whatever char color the parameter gets
    switch (driveColor) {
    case 0:
      leds[x] = CRGB(0, 0, frame);
      break;
    case 1:
      leds[x] = CRGB(frame, 0, 0);
      break;
    case 2:
      leds[x] = CRGB(frame+86, frame, 0);
      break;
    default:
      leds[x] = CRGB::Black; //if things break, just turn them off
      break;
    }
  }
}

void autoDisabled() {
  elapsedMillis timeElapsed = 0;
  //set every other lift light to white
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_LEDS; x+=2) {
      leds[x] = white;
  }
  //set other lift lights to yellow
  for(int x=NUM_DRIVE_LIGHTS+1; x<NUM_LEDS; x+=2) {
      leds[x] = yellow;
  }
  leds[NUM_LEDS-1] = red; //set top light to red???
  FastLED.show();
  while(timeElapsed < 151) { //delay pulse to create breathing effect
  }
  timeElapsed = 0;
}

void autoEnabled() {
  CRGB AC;
  elapsedMillis timeElapsed = 0; 
  //set drive train color
  switch (allianceColor){
    case 'B':
      AC = blue;
      break;
    case 'R':
      AC = red;
      break;
    default:
      AC = blue;
      break;
  }
  for(int x=0; x<NUM_DRIVE_LIGHTS; x++) {
    switch (allianceColor) {
    case 'B':
      leds[x] = blue;
      break;
    case 'R':
      leds[x] = red;
      break;
    default:
      leds[x] = CRGB::Black; //off means something broke
      break;
    }
  }
  //find number of lights needed to light up to lift position
  if (liftPosition > MAX_LIFT) liftPosition = MAX_LIFT;
  if (liftPosition < 0) liftPosition = 0;
  int percent = liftPosition / MAX_LIFT * 100;
  int lights = NUM_LIFT_LIGHTS * percent / 100;
  int lightsOn = NUM_LIFT_LIGHTS - lights;
  int topOffset = NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn;
  //set lights up to that point to white
  for(int x=NUM_DRIVE_LIGHTS; x<topOffset; x++) {
    leds[x] = yellow;
    if (x == topOffset-2){
    leds[x] = AC;
    }
    if (x == topOffset-1){
    leds[x] = AC;
    }
    if (x == topOffset-27){
    leds[x] = AC;
    }
    if (x == topOffset-28){
    leds[x] = AC;
    }
    if (x == topOffset-53){
    leds[x] = AC;
    }
    if (x == topOffset-54){
    leds[x] = AC;
    }
    if (x == topOffset-79){
    leds[x] = AC;
    }
    if (x == topOffset-80){
    leds[x] = AC;
    }
  }
  //reset all others to black
  for(int x=topOffset; x<NUM_LEDS; x++) {
    leds[x] = CRGB::Black;
  }
/*  leds[NUM_DRIVE_LIGHTS+9] = red;
  leds[NUM_DRIVE_LIGHTS+10] = red;
  leds[NUM_DRIVE_LIGHTS+36] = red;
  leds[NUM_DRIVE_LIGHTS+37] = red;
  leds[NUM_DRIVE_LIGHTS+63] = red;
  leds[NUM_DRIVE_LIGHTS+64] = red;
  leds[NUM_LEDS-2] = red; */
  leds[NUM_LEDS-1] = red; //set top-most light to red
  FastLED.show();
}

void teleopEnabled() {
  CRGB AC;
  elapsedMillis timeElapsed = 0;
  switch (allianceColor){
    case 'B':
      AC = blue;
      break;
    case 'R':
      AC = red;
      break;
    default:
      AC = blue;
      break;
  }
  for(int x=0; x<NUM_DRIVE_LIGHTS; x++) {
    switch (allianceColor) {
    case 'B':
      leds[x] = blue;
      break;
    case 'R':
      leds[x] = red;
      break;
    default:
      leds[x] = CRGB::Black; //turn off things when they break
      break;
    }
  }
  //find number of lights needed to light up to lift position
  if (liftPosition > MAX_LIFT) liftPosition = MAX_LIFT;
  if (liftPosition < 0) liftPosition = 0;
  int percent = liftPosition / MAX_LIFT * 100;
  int lights = NUM_LIFT_LIGHTS * percent / 100;
  int lightsOn = NUM_LIFT_LIGHTS - lights;
  int topOffset = NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn;
  for(int x=NUM_DRIVE_LIGHTS; x<topOffset; x++) {
    leds[x] = gray;
    if (x == topOffset-2){
    leds[x] = AC;
    }
    if (x == topOffset-1){
    leds[x] = AC;
    }
    if (x == topOffset-27){
    leds[x] = AC;
    }
    if (x == topOffset-28){
    leds[x] = AC;
    }
    if (x == topOffset-53){
    leds[x] = AC;
    }
    if (x == topOffset-54){
    leds[x] = AC;
    }
    if (x == topOffset-79){
    leds[x] = AC;
    }
    if (x == topOffset-80){
    leds[x] = AC;
    }
  }
  //reset all others to black
  for(int x=topOffset; x<NUM_LEDS; x++) {
    leds[x] = CRGB::Black;
  }
/*  leds[NUM_DRIVE_LIGHTS+9] = blue;
  leds[NUM_DRIVE_LIGHTS+10] = blue;
  leds[NUM_DRIVE_LIGHTS+36] = blue;
  leds[NUM_DRIVE_LIGHTS+37] = blue;
  leds[NUM_DRIVE_LIGHTS+63] = blue;
  leds[NUM_DRIVE_LIGHTS+64] = blue;
  leds[NUM_LEDS-2] = blue; */
  leds[NUM_LEDS-1] = blue; //set top-most light to blue
  FastLED.show();
}

void teleopDisabled() {
  elapsedMillis timeElapsed = 0;
  //set every other lift light to white
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_LEDS; x+=2) {
      leds[x] = green;
    }
  //set other lights to green
  for(int x=NUM_DRIVE_LIGHTS+1; x<NUM_LEDS; x+=2) {
      leds[x] = green;
  }
  leds[NUM_LEDS-1] = blue; //set top-most light to blue
  FastLED.show();
  while(timeElapsed < 151){ //delay pulse to create breathing effect
  }
  timeElapsed = 0;
}

void disconnected2() {
  elapsedMillis timeElapsed = 0;
  // Define the basic light pattern
  CRGB pattern[12];
  pattern[0] = pattern[1] = pattern[2] = teamRed;
  pattern[3] = CRGB::Black;
  pattern[4] = pattern[5] = pattern[6] = teamGold;
  pattern[7] = CRGB::Black;
  pattern[8] = pattern[9] = pattern[10] = teamBlue;
  pattern[11] = CRGB::Black;

  // Cycle through the starting point on the pattern
  // This creates the full cycle of "moving" lights
  for (int start=0;start<12;start++) {
    // Fill in the lights for the drive
    for (int x=0;x<NUM_DRIVE_LIGHTS;x++) {
      // Find the current pattern color using both starting pattern offset and the current LED
      int y = (start+x)%12;
      leds[x] = pattern[y];
    }

    // Fill in the lights for the lift
    for (int x=NUM_LEDS-1;x>=NUM_DRIVE_LIGHTS;x--) {
      // Find the current pattern color using both starting pattern offset and the current LED
      int y = 11-((start+x)%12);
      leds[x] = pattern[y];
    }

    // Show the lights
    FastLED.show();
    while(timeElapsed < 51){
    }
  timeElapsed = 0;
  }
}

void disconnected() {
  elapsedMillis timeElapsed = 0;
  //warning: this function is at this point kinda pointless because of disconnected2
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
    leds[i+4] = teamGold;
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = teamGold;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = teamGold;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = CRGB::Black;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = teamRed;
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = teamRed;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = teamRed;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = CRGB::Black;
  }
  FastLED.show();
  if (mode != DISCONNECTED) return;
  while(timeElapsed < 76){
  }
  timeElapsed = 0;
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
    leds[i+4] = teamGold;
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = teamGold;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = CRGB::Black;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = teamRed;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = teamRed;
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = teamRed;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = CRGB::Black;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = teamBlue;
  }
  FastLED.show();
  while(timeElapsed < 76){
  }
  timeElapsed = 0;
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
    leds[i+4] = teamGold;
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = CRGB::Black;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = teamRed;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = teamRed;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = teamRed;
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = CRGB::Black;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = teamBlue;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = teamBlue;
  }
  FastLED.show();
  while(timeElapsed < 76){
  }
  timeElapsed = 0;
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
    leds[i+4] = CRGB::Black;
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = teamRed;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = teamRed;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = teamRed;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = CRGB::Black;
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = teamBlue;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = teamBlue;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = teamBlue;
  }
  FastLED.show();
  while(timeElapsed < 76){
  }
  timeElapsed = 0;
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
    leds[i+4] = teamRed;
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = teamRed;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = teamRed;
    if (i+7 >= NUM_LEDS) break;
    leds[i+7] = CRGB::Black;
    if (i+8 >= NUM_LEDS) break;
    leds[i+8] = teamBlue;
    if (i+9 >= NUM_LEDS) break;
    leds[i+9] = teamBlue;
    if (i+10 >= NUM_LEDS) break;
    leds[i+10] = teamBlue;
    if (i+11 >= NUM_LEDS) break;
    leds[i+11] = CRGB::Black;
  }
  FastLED.show();
  while(timeElapsed < 76){
  }
  timeElapsed = 0;
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
    leds[i+4] = teamRed;
    if (i+5 >= NUM_LEDS) break;
    leds[i+5] = teamRed;
    if (i+6 >= NUM_LEDS) break;
    leds[i+6] = CRGB::Black;
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
  while(timeElapsed < 76){
  }
  timeElapsed = 0;
  if (mode != DISCONNECTED) return;
  for (int i=0; i<NUM_LEDS-10; i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = teamGold;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = CRGB::Black;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = teamRed;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = teamRed;
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = teamRed;
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
  while(timeElapsed < 76){
  }
  timeElapsed = 0;
  if (mode != DISCONNECTED) return;
  for (int i=0; i<NUM_LEDS-10; i+=12) {
    if (i >= NUM_LEDS) break;
    leds[i] = CRGB::Black;
    if (i+1 >= NUM_LEDS) break;
    leds[i+1] = teamRed;
    if (i+2 >= NUM_LEDS) break;
    leds[i+2] = teamRed;
    if (i+3 >= NUM_LEDS) break;
    leds[i+3] = teamRed;
    if (i+4 >= NUM_LEDS) break;
    leds[i+4] = CRGB::Black;
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
  while(timeElapsed < 76){
  }
  timeElapsed = 0;
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
  while(timeElapsed < 76){
  }
  timeElapsed = 0;
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
  while(timeElapsed < 76){
  }
  timeElapsed = 0;
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
  while(timeElapsed < 76){
  }
  timeElapsed = 0;
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
  while(timeElapsed < 76){
  }
  timeElapsed = 0;
  if (mode != DISCONNECTED) return;
}

void on(int t) {
  elapsedMillis timeElapsed = 0;
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
  while(timeElapsed < t+1){
  }
  timeElapsed = 0;
}

void off(int t) {
  elapsedMillis timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x++) {
    leds[x]= CRGB::Black;
  }
  FastLED.show();
  while(timeElapsed < t+1){
  }
  timeElapsed = 0;
}

void finished() {
  elapsedMillis timeElapsed = 0;
  a = 4;
  switch (a) {
    case 0:
      //Hi, Fischler
      b = 19; //set red white blue chase time
      on(75); //H
      off(75);
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(75); //i
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(300); //,
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
      off(500); //
      if (mode != FINISHED) return;
      
      on(75); //F
      off(75);
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(75); //i
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(75); //s
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(300); //c
      off(75);
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(75); //h
      off(75);
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(75); //l
      off(75);
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(75); //e
      off(300);
      if (mode != FINISHED) return;
      
      on(75); //r
      off(75);
      on(300);
      off(75);
      on(75);
      off(500); //
      if (mode != FINISHED) return;
      break;
    case 1:
      //Ubernerds
      b = 22; //set red white blue chase time
      on(75); //U
      off(75);
      on(75);
      off(75);
      on(300);
      off(300);
      if (mode != FINISHED) return;
  
      on(300); //b
      off(75);
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //e
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //r
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
      
      on(300); //n
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //e
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //r
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(300); //d
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //s
      off(75);
      on(75);
      off(75);
      on(75);
      off(500); //
      if (mode != FINISHED) return;
      break;
    case 2:
      //Uberbots
      b = 22; //set red white blue chase time
      on(75); //U
      off(75);
      on(75);
      off(75);
      on(300);
      off(300);
      if (mode != FINISHED) return;
  
      on(300); //b
      off(75);
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //e
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //r
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(300); //b
      off(75);
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(300); //o
      off(75);
      on(300);
      off(75);
      on(300);
      off(300);
      if (mode != FINISHED) return;
  
      on(300); //t
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //s
      off(75);
      on(75);
      off(75);
      on(75);
      off(500); //
      if (mode != FINISHED) return;
      break;
    case 3:
      //Niezreicki's rule
      b = 16; //set red white blue chase time
      on(300); //N
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //i
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //e
      off(300);
      if (mode != FINISHED) return;
  
      on(300); //z
      off(75);
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //r
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //e
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //i
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(300); //c
      off(75);
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(300); //k
      off(75);
      on(75);
      off(75);
      on(300);
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //i
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //'
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
  
      on(75); //s
      off(75);
      on(75);
      off(75);
      on(75);
      off(500); //
      if (mode != FINISHED) return;
  
      on(75); //r
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //u
      off(75);
      on(75);
      off(75);
      on(300);
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //l
      off(75);
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      if (mode != FINISHED) return;
  
      on(75); //e
      off(500); //
      if (mode != FINISHED) return;
      break;
    default:
    b = 30; //set red white blue chase time
      for (int x=0; x<NUM_LEDS; x++) {
          leds[x] = CRGB::Black;
      }
      break;
  }
  //Fix red white blue chase to take up less space
  //As in fewer for loops would be nice
  for (int x=0; x<b; x++) {
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
  for(int z=0; z<NUM_LEDS; z+=20  ) {
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
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  if (mode != FINISHED) return;
 }
}

void errorMode() {
  elapsedMillis timeElapsed = 0;
  //To test, press space bar at driver's station
  CRGB errorYellow = CRGB(255, 32, 0);
  //Also make this take up less code
  //Pattern arrays, yay!
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = errorYellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = red;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  while(timeElapsed < 51){
  }
  timeElapsed = 0;
}

void setState() {
 if ((charCommand[0] == '<') && (charCommand[2] == ':') && (charCommand[5] == ';')) //try  && (charCommand[counter-1] == '>')
 {
  char robotState[] = {charCommand[3], charCommand[4]};
    if (robotState[0] == 'S' && robotState[1] == 'E') // test enabled
    {
      mode = 1;
    }
    else if (robotState[0] == 'S' && robotState[1] == 'D') // test disabled
    {
      mode = 2;
    }
    else if (robotState[0] == 'A' && robotState[1] == 'E') // auto enabled
    {
      mode = 3;
    }
    else if (robotState[0] == 'A' && robotState[1] == 'D') // auto disabled
    {
      mode = 4;
    }
    else if (robotState[0] == 'T' && robotState[1] == 'E') // teleop enabled
    {
      mode = 5;
    }
    else if (robotState[0] == 'T' && robotState[1] == 'D') // teleop disabled
    {
      mode = 6;
    }
    else if (robotState[0] == 'D' && robotState[1] == 'C') // disconnected
    {
      mode = 7;
    }
    else if (robotState[0] == 'F' && robotState[1] == 'N') // finished
    {
      mode = 8;
    }
    else if (robotState[0] == 'E' && robotState[1] == 'S') // error/emergency stop
    {
      mode = 9;
    }
    else if (robotState[0] == 'O' && robotState[1] == 'F') // lights off
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
  switch (mode) {
  case TEST_ENABLED:
    //Drive: solid orange, Lift: white up to lift position with top-most light orange
    testEnabled();
    break;
  case TEST_DISABLED:
    //Drive: yellow pulse, Lift: White up to lift position with top-most light orange
    pulseDriveTrain(2);
    testDisabled();
    break;
  case AUTO_ENABLED:
    //Drive: alliance color, Lift: white with top-most red
    autoEnabled();
    break;
  case AUTO_DISABLED:
    //Drive: alliance color pulse, Lift: white and yellow alternating
    if (allianceColor == 'B') pulseDriveTrain(0);
    if (allianceColor == 'R') pulseDriveTrain(1);
    autoDisabled();
    break;
  case TELEOP_ENABLED:
    //Drive: alliance color, Lift: white on lift up to lift position with top-most blue
    teleopEnabled();
    break;
  case TELEOP_DISABLED:
    //Drive: alliance color pulse, Lift: white and green alternating
    if (allianceColor == 'B') pulseDriveTrain(0);
    if (allianceColor == 'R') pulseDriveTrain(1);
    teleopDisabled();
    break;
  case DISCONNECTED:
    //Drive and Lift: red, yellow, blue chase
    disconnected2();
    break;
  case FINISHED:
    finished();
    break;
  case ERROR_MODE:
    //Drive and Lift: red and yellow flash
    errorMode();
    break;
  case OFF:
    //Drive and Lift: all lights off, duh
    off(0);
    break;
  }
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
  while(timeElapsed < 21){
  }
  timeElapsed = 0;
  */
}

