/*
 * -- AREN Servo Zeroing Program
 * 
 * -- ABOUT
 *    For moving servos to its zero position from any position it may be set.
 *    Use this program when recalibrating the servo horn positions.
 * 
 */

#include <Aren.h>

// -- Pin number constants specification
#define PIN_LL 2      // Left leg
#define PIN_RL 3      // Right leg
#define PIN_LF 4      // Left foot
#define PIN_RF 5      // Right foot
#define PIN_Buzzer 13 // Buzzer
#define PIN_Trigger 8 // Ultrasound Trigger
#define PIN_Echo 9    // Ultrasound Echo

// -- AREN Object Creation
Aren Aren;

// -- Program Setup
void setup()
{
  Aren.init(PIN_LL, PIN_RL, PIN_LF, PIN_RF, true, PIN_Buzzer, PIN_Trigger, PIN_Echo);
  Aren.home();
}

// -- Main Program Loop
void loop()
{
  Aren.home();
  delay(1000);
  Aren.setRestState(false);
}