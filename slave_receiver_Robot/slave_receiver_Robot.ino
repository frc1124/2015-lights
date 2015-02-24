// robot to arduino command interpretation slave :)

#include <stdlib.h>
#include <Wire.h>

char* charCommand;
char* cmdString;
char* robotState;
char* liftString;
char robotAlliance;
double liftValue;
int counter = 0;
int mode;

void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop()
{
  delay(100);
 if ((charCommand[0] == '<') && (charCommand[2] == ':') && (charCommand[5] == ';') && (charCommand[counter] == '>'))
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
    
  char robotAlliance = charCommand[1];
  int j;
  for (j = 0; j++; j < (counter - 6))
  {
    liftString[j] = {charCommand[j+6]};
  }
  liftString[j+1] = '\0';
  liftValue = atof(liftString);
  memset(liftString, 0, sizeof liftString);
 }
}


void receiveEvent(int howMany)
{
  while ((1 <= Wire.available()) && (charCommand[counter] != '>')) 
  {
    char incomingChar = Wire.read();
    if (incomingChar == '<')
    {
      counter = 0;
    }
    charCommand[counter] = incomingChar;
    counter++;
  }
}
