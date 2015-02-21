// DO NOT USE THIS VERSION
// THIS WAS A TEST I ACCIDENTALLY UPLOADED
// LITERALLY FISCHLER
// "8/10 TOO MUCH COMMENT" - IGN

#include <stdlib.h>
#include <Wire.h>

char* charCommand;
char* cmdString;
char* robotState;
char robotAlliance;
double liftValue;

void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);

}

void receiveEvent(int howMany)
{
  while (1 <= Wire.available())
  {
    char incomingChar = Wire.read();
    Serial.print(incomingChar);
  }
}
/*    while (charCommand[-1] != '>')
    {
      int i;
      for (i = 0; i++;)
        {
          charCommand[i] = incomingChar;
        }
      int commandIndex = i;
    }
  cmdString = charCommand;
  char charCommand;
  char robotState[] = {cmdString[3], cmdString[4]}; 
  char robotAlliance = cmdString[1];
  int j;
  char liftString[1];
  for (j = 0; j++;)
  {
    if (cmdString[6+j] != '>')
    {
     liftString[j] = cmdString[6+j];
    }
  }
  liftValue = strtod(liftString, NULL); */

void loop()
{
//  Serial.println(cmdString);
//  Serial.print("Robot State");
//  Serial.println(robotState);
//  Serial.print("Robot Alliance");
//  Serial.println(robotAlliance);
//  Serial.print("Lift Value");
//  Serial.println(liftValue);
  
}
