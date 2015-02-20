#include "FastLED.h"
#define NUM_LEDS 175
#define DATA_PIN 5
#define CLOCK_PIN 13
#define MAX_LIFT 59
#define NUM_LIFT_LIGHTS 75
#define NUM_DRIVE_LIGHTS 25
CRGB leds[NUM_LEDS];
CRGB allianceColor = CRGB::Blue;
byte alliancecolor = 0;
float liftPosition = 59.0;
int a, b, v, w, y, z;
byte c;

#define TEST_ENABLED 1
#define TEST_DISABLED 2
#define AUTO_ENABLED 3
#define AUTO_DISABLED 4
#define TELEOP_ENABLED 5
#define TELEOP_DISABLED 6
#define DISCONNECTED 7
#define FINISHED 8
#define ERROR_MODE 9

byte mode = TELEOP_DISABLED;
byte frame = 24;


void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void testEnabled() {
  for(int x=0; x<NUM_DRIVE_LIGHTS; x++) {
    leds[x] = CRGB::Orange;
  }
  int percent = liftPosition / MAX_LIFT * 100;
  int lights = NUM_LIFT_LIGHTS * percent / 100;
  int lightsOn = NUM_LIFT_LIGHTS - lights;
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn; x++) {
    leds[x] = CRGB::White;
  }
  leds[NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn] = CRGB::Orange;
  for(int x=NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS+1; x<NUM_LEDS-lightsOn-1; x++) {
    leds[x] = CRGB::White;
  }
  leds[NUM_LEDS-lightsOn-1] = CRGB::Orange;
  FastLED.show();
}

void testDisabled() {
  if (frame > 254) c = 1;
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
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn; x++) {
    leds[x] = CRGB::White;
  }
  leds[NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn] = CRGB::Orange;
  for(int x=NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS+1; x<NUM_DRIVE_LIGHTS+2*NUM_LIFT_LIGHTS-lightsOn-1; x++) {
    leds[x] = CRGB::White;
  }
  leds[NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS+NUM_LIFT_LIGHTS-lightsOn-1] = CRGB::Orange;
  FastLED.show();
  delay(80);
}

void autoDisabled() {
  if (frame > 254) c = 1;
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
    if (alliancecolor == 1) {
      leds[x] = CRGB(0, 0, frame);
    }
    else {
      leds[x] = CRGB(frame, 0, 0);
    }
  }
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS; x+=2) {
      leds[x] = CRGB::White;
    }
    for(int x=NUM_DRIVE_LIGHTS+1; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS; x+=2) {
      leds[x] = CRGB::Yellow;
    }
    for(int x=NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS; x<NUM_LEDS; x+=2) {
      leds[x] = CRGB::White;
    }
    for(int x=NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS+1; x<NUM_LEDS; x+=2) {
      leds[x] = CRGB::Yellow;
    }
  FastLED.show();
  delay(80);
}

void autoEnabled() {
  for(int x=0; x<NUM_DRIVE_LIGHTS; x++) {
    leds[x] = allianceColor;
  }
  int percent = liftPosition / MAX_LIFT * 100;
  int lights = NUM_LIFT_LIGHTS * percent / 100;
  int lightsOn = NUM_LIFT_LIGHTS - lights;
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn; x++) {
    leds[x] = CRGB::White;
  }
  leds[NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn] = CRGB::Red;
  for(int x=NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS+1; x<NUM_LEDS-lightsOn-1; x++) {
    leds[x] = CRGB::White;
  }
  leds[NUM_LEDS-lightsOn-1] = CRGB::Red;
  FastLED.show();
}

void teleopEnabled() {
  for(int x=0; x<NUM_DRIVE_LIGHTS; x++) {
    leds[x] = allianceColor;
  }
  int percent = liftPosition / MAX_LIFT * 100;
  int lights = NUM_LIFT_LIGHTS * percent / 100;
  int lightsOn = NUM_LIFT_LIGHTS - lights;
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn; x++) {
    leds[x] = CRGB::White;
  }
  leds[NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS-lightsOn] = CRGB::Blue;
  for(int x=NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS+1; x<NUM_LEDS-lightsOn-1; x++) {
    leds[x] = CRGB::White;
  }
  leds[NUM_LEDS-lightsOn-1] = CRGB::Blue;
  FastLED.show();
}

void teleopDisabled() {
  if (frame > 254) c = 1;
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
    if (alliancecolor == 1) {
      leds[x] = CRGB(0, 0, frame);
    }
    else {
      leds[x] = CRGB(frame, 0, 0);
    }
  }
  for(int x=NUM_DRIVE_LIGHTS; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS; x+=2) {
      leds[x] = CRGB::White;
    }
    for(int x=NUM_DRIVE_LIGHTS+1; x<NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS; x+=2) {
      leds[x] = CRGB::Green;
    }
    for(int x=NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS+1; x<NUM_LEDS-1; x+=2) {
      leds[x] = CRGB::White;
    }
    for(int x=NUM_DRIVE_LIGHTS+NUM_LIFT_LIGHTS+2; x<NUM_LEDS-1; x+=2) {
      leds[x] = CRGB::Green;
    }
  FastLED.show();
  delay(80);
}

void disconnected() { 
  // Turn the LED on, then pause
  CRGB blue  = CRGB(13, 91, 223);
  CRGB gold = CRGB(215, 180, 36);
  CRGB red = CRGB(128, 3, 5);
  for (int i=0;i<NUM_LEDS-12;i+=12) {
    leds[i] = blue;
    leds[i+1] = blue;
    leds[i+2] = blue;
    leds[i+3] = CRGB::Black;
    leds[i+4] = gold; //Red
    leds[i+5] = gold;
    leds[i+6] = gold;
    leds[i+7] = CRGB::Black;
    leds[i+8] = red; //Blue
    leds[i+9] = red;
    leds[i+10] = red;
    leds[i+11] = CRGB::Black;
  }
  FastLED.show();
  delay(75);
  for (int i=0;i<NUM_LEDS-12;i+=12) {
    leds[i] = blue;
    leds[i+1] = blue;
    leds[i+2] = CRGB::Black;
    leds[i+3] = gold;
    leds[i+4] = gold; //Red
    leds[i+5] = gold;
    leds[i+6] = CRGB::Black;
    leds[i+7] = red;
    leds[i+8] = red; //Blue
    leds[i+9] = red;
    leds[i+10] = CRGB::Black;
    leds[i+11] = blue;
  }
  FastLED.show();
  delay(75);
  for (int i=0;i<NUM_LEDS-12;i+=12) {
    leds[i] = blue;
    leds[i+1] = CRGB::Black;
    leds[i+2] = gold;
    leds[i+3] = gold;
    leds[i+4] = gold; //Red
    leds[i+5] = CRGB::Black;
    leds[i+6] = red;
    leds[i+7] = red;
    leds[i+8] = red; //Blue
    leds[i+9] = CRGB::Black;
    leds[i+10] = blue;
    leds[i+11] = blue;
  }
  FastLED.show();
  delay(75);
  for (int i=0;i<NUM_LEDS-12;i+=12) {
    leds[i] = CRGB::Black;
    leds[i+1] = gold;
    leds[i+2] = gold;
    leds[i+3] = gold;
    leds[i+4] = CRGB::Black; //Red
    leds[i+5] = red;
    leds[i+6] = red;
    leds[i+7] = red;
    leds[i+8] = CRGB::Black; //Blue
    leds[i+9] = blue;
    leds[i+10] = blue;
    leds[i+11] = blue;
  }
  FastLED.show();
  delay(75);
  for (int i=0;i<NUM_LEDS-12;i+=12) {
    leds[i] = gold;
    leds[i+1] = gold;
    leds[i+2] = gold;
    leds[i+3] = CRGB::Black;
    leds[i+4] = red; //Red
    leds[i+5] = red;
    leds[i+6] = red;
    leds[i+7] = CRGB::Black;
    leds[i+8] = blue; //Blue
    leds[i+9] = blue;
    leds[i+10] = blue;
    leds[i+11] = CRGB::Black;
  }
  FastLED.show();
  delay(75);
  // Now turn the LED off, then pause
  for (int i=0;i<NUM_LEDS-12;i+=12) {
    leds[i] = gold;
    leds[i+1] = gold;
    leds[i+2] = CRGB::Black;
    leds[i+3] = red;
    leds[i+4] = red; //Red
    leds[i+5] = red;
    leds[i+6] = CRGB::Black; //Blue
    leds[i+7] = blue;
    leds[i+8] = blue;
    leds[i+9] = blue;
    leds[i+10] = CRGB::Black;
    leds[i+11] = gold;
  }
  FastLED.show();
  delay(75);
  for (int i=0; i<NUM_LEDS-12; i+=12) {
    leds[i] = gold;
    leds[i+1] = CRGB::Black;
    leds[i+2] = red;
    leds[i+3] = red; //Red
    leds[i+4] = red; //Blue
    leds[i+5] = CRGB::Black;
    leds[i+6] = blue;
    leds[i+7] = blue;
    leds[i+8] = blue;
    leds[i+9] = CRGB::Black;
    leds[i+10] = gold;
    leds[i+11] = gold;
  }
  FastLED.show();
  delay(75);
  for (int i=0; i<NUM_LEDS-12; i+=12) {
    leds[i] = CRGB::Black;
    leds[i+1] = red;
    leds[i+2] = red;
    leds[i+3] = red; //Red
    leds[i+4] = CRGB::Black; //Blue
    leds[i+5] = blue;
    leds[i+6] = blue;
    leds[i+7] = blue;
    leds[i+8] = CRGB::Black;
    leds[i+9] = gold;
    leds[i+10] = gold;
    leds[i+11] = gold;
  }
  FastLED.show();
  delay(75);
  for (int i=0; i<NUM_LEDS-12; i+=12) {
    leds[i] = leds[i+1] = leds[i+2] = red;
    leds[i+3] = CRGB::Black;
    leds[i+4] = leds[i+5] = leds[i+6] = blue;
    leds[i+7] = CRGB::Black;
    leds[i+8] = leds[i+9] = leds[i+10] = gold;
    leds[i+11] = CRGB::Black;
  }
  FastLED.show();
  delay(75);
  for (int i=0; i<NUM_LEDS-12; i+=12) {
    leds[i] = leds[i+1] = red;
    leds[i+2] = CRGB::Black;
    leds[i+3] = leds[i+4] = leds[i+5] = blue;
    leds[i+6] = CRGB::Black;
    leds[i+7] = leds[i+8] = leds[i+9] = gold;
    leds[i+10] = CRGB::Black;
    leds[i+11] = red;
  }
  FastLED.show();
  delay(75);
  for (int i=0; i<NUM_LEDS-12; i+=12) {
    leds[i] = red;
    leds[i+1] = CRGB::Black;
    leds[i+2] = leds[i+3] = leds[i+4] = blue;
    leds[i+5] = CRGB::Black;
    leds[i+6] = leds[i+7] = leds[i+8] = gold;
    leds[i+9] = CRGB::Black;
    leds[i+10] = leds[i+11] = red;
  }
  FastLED.show();
  delay(75);
  for (int i=0; i<NUM_LEDS-12; i+=12) {
    leds[i] = CRGB::Black;
    leds[i+1] = blue;
    leds[i+2] = blue;
    leds[i+3] = blue; //Red
    leds[i+4] = CRGB::Black; //Blue
    leds[i+5] = gold;
    leds[i+6] = gold;
    leds[i+7] = gold;
    leds[i+8] = CRGB::Black;
    leds[i+9] = red;
    leds[i+10] = red;
    leds[i+11] = red;
  }
  FastLED.show();
  delay(75);
}

void on(int t) {
  CRGB blue = CRGB(13, 91, 223);
  CRGB gold = CRGB(255, 255, 36);
  CRGB red = CRGB(128, 3, 5);
  for (int z=0; z<20; z++) {
  y = random(NUM_LEDS-3);
  for(int x=y; x<y+3; x++) {
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
      
      on(75);
      off(75);
      on(75);
      off(300);
      
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
      
      on(75);
      off(75);
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      
      on(75);
      off(75);
      on(75);
      off(300);
      
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      
      on(300);
      off(75);
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
      
      on(75);
      off(300);
      
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(500);
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
  
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
  
      on(75);
      off(300);
  
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
      
      on(300);
      off(75);
      on(75);
      off(300);
  
      on(75);
      off(300);
  
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
  
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
  
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(500);
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
  
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
  
      on(75);
      off(300);
  
      on(75);
      off(75);
      on(300);
      off(75);
      on(75);
      off(300);
  
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
  
      on(300);
      off(75);
      on(300);
      off(75);
      on(300);
      off(300);
  
      on(300); 
      off(300);
  
      on(75);
      off(75);
      on(75);
      off(75);
      on(75);
      off(500);
      break;
    case 3:
      //Niezreicki's rule
      b = 16;
      on(300);
      off(75);
      on(75);
      off(300);
  
      on(75);
      off(75);
      on(75);
      off(300);
  
      on(75);
      off(300);
  
      on(300);
      off(75);
      on(300);
      off(75);
      on(75);
      off(75);
      on(75);
      off(300);
  
  on(75);
  off(75);
  on(300);
  off(75);
  on(75);
  off(300);
  
  on(75);
  off(300);
  
  on(75);
  off(75);
  on(75);
  off(300);
  
  on(300);
  off(75);
  on(75);
  off(75);
  on(300);
  off(75);
  on(75);
  off(300);
  
  on(300);
  off(75);
  on(75);
  off(75);
  on(300);
  off(300);
  
  on(75);
  off(75);
  on(75);
  off(300);
  
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
  
  on(75);
  off(75);
  on(75);
  off(75);
  on(75);
  off(500);
  
  on(75);
  off(75);
  on(300);
  off(75);
  on(75);
  off(300);
  
  on(75);
  off(75);
  on(75);
  off(75);
  on(300);
  off(300);
  
  on(75);
  off(75);
  on(300);
  off(75);
  on(75);
  off(75);
  on(75);
  off(300);
  
  on(75);
  off(500);
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
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = CRGB::Red;
    leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = CRGB::White;
    leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = CRGB::Blue;
    leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = CRGB::Red;
    leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = CRGB::White;
    leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = CRGB::Blue;
    leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = CRGB::Black;
    leds[z+19] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = leds[z+1] = leds[z+2] = CRGB::Red;
    leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = CRGB::White;
    leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = CRGB::Blue;
    leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = CRGB::Black;
    leds[z+18] = leds[z+19] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = leds[z+1] = CRGB::Red;
    leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = CRGB::White;
    leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = CRGB::Blue;
    leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = CRGB::Black;
    leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = CRGB::Red;
    leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = CRGB::White;
    leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = CRGB::Blue;
    leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = CRGB::Black;
    leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = CRGB::White;
    leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = CRGB::Blue;
    leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = CRGB::Black;
    leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = CRGB::White;
    leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = CRGB::Blue;
    leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = CRGB::Black;
    leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = CRGB::Red;
    leds[z+19] = CRGB::White;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = leds[z+1] = leds[z+2] = CRGB::White;
    leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = CRGB::Blue;
    leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = CRGB::Black;
    leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = CRGB::Red;
    leds[z+18] = leds[z+19] = CRGB::White;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = leds[z+1] = CRGB::White;
    leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = CRGB::Blue;
    leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = CRGB::Black;
    leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = CRGB::Red;
    leds[z+17] = leds[z+18] = leds[z+19] = CRGB::White;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = CRGB::White;
    leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = CRGB::Blue;
    leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = CRGB::Black;
    leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = CRGB::Red;
    leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::White;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = CRGB::Blue;
    leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = CRGB::Black;
    leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = CRGB::Red;
    leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::White;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = CRGB::Blue;
    leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = CRGB::Black;
    leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = CRGB::Red;
    leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = CRGB::White;
    leds[z+19] = CRGB::Blue;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = leds[z+1] = leds[z+2] = CRGB::Blue;
    leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = CRGB::Black;
    leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = CRGB::Red;
    leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = CRGB::White;
    leds[z+18] = leds[z+19] = CRGB::Blue;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = leds[z+1] = CRGB::Blue;
    leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = CRGB::Black;
    leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = CRGB::Red;
    leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = CRGB::White;
    leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Blue;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = CRGB::Blue;
    leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = CRGB::Black;
    leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = CRGB::Red;
    leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = CRGB::White;
    leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Blue;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = CRGB::Black;
    leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = CRGB::Red;
    leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = CRGB::White;
    leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Blue;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = leds[z+1] = leds[z+2] = leds[z+3] = CRGB::Black;
    leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = leds[z+8] = CRGB::Red;
    leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = leds[z+13] = CRGB::White;
    leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = leds[z+18] = CRGB::Blue;
    leds[z+19] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = leds[z+1] = leds[z+2] = CRGB::Black;
    leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = leds[z+7] = CRGB::Red;
    leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = leds[z+12] = CRGB::White;
    leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = leds[z+17] = CRGB::Blue;
    leds[z+18] = leds[z+19] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = leds[z+1] = CRGB::Black;
    leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = leds[z+6] = CRGB::Red;
    leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = leds[z+11] = CRGB::White;
    leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = leds[z+16] = CRGB::Blue;
    leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int z=0; z<NUM_LEDS-20; z+=20) {
    leds[z] = CRGB::Black;
    leds[z+1] = leds[z+2] = leds[z+3] = leds[z+4] = leds[z+5] = CRGB::Red;
    leds[z+6] = leds[z+7] = leds[z+8] = leds[z+9] = leds[z+10] = CRGB::White;
    leds[z+11] = leds[z+12] = leds[z+13] = leds[z+14] = leds[z+15] = CRGB::Blue;
    leds[z+16] = leds[z+17] = leds[z+18] = leds[z+19] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
 }
}

void errorMode() {
  CRGB yellow = CRGB(255, 32, 0);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = yellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = yellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = yellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = yellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = yellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = yellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = yellow;
  }
  for(int x=1; x<NUM_LEDS; x+=2) {
    leds[x] = CRGB::Red;
  }
  FastLED.show();
  delay(50);
  for(int x=0; x<NUM_LEDS; x+=2) {
    leds[x] = yellow;
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
    leds[x] = CRGB::Red;
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
    leds[x] = CRGB::Red;
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
    leds[x] = CRGB::Red;
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
    leds[x] = CRGB::Red;
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
    disconnected();
    break;
  case FINISHED:
    finished();
    break;
  case ERROR_MODE:
    errorMode();
    break;
  }
}

