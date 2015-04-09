 /*
 
 */

#include <Esplora.h>
#include <SabertoothSimplified.h>

SabertoothSimplified ST;
void setup()
{
  Serial1.begin(9600);       // initialize serial communication with sabertooth motor controller
  ST.drive(0);
  ST.turn(0);
  
}

void loop()
{
  
  ST.drive(map(Esplora.readJoystickX(), 0, 1024, -127, 127));
  
  ST.turn(map(Esplora.readJoystickY(), 0, 1024, -127, 127));
  
  delay(10);                                  // a short delay before moving again
}

