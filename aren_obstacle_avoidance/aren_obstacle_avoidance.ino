/*
 * -- AREN Obstacle Avoidance Program
 * 
 * -- ABOUT
 *    Avoiding obstacles in front of the AREN using its front-facing ultrasonic sensor
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

// -- AREN & Ultrasound Object Creation
Aren Aren;

// -- Global Variables Declaration
int distance;                    // variable to store distance read from ultrasonic range finder module
bool isObstacleDetected = false; // logic state for when object detected is at the distance we set

// -- Program Setup
void setup()
{
  Serial.begin(9600);
  Aren.init(PIN_LL, PIN_RL, PIN_LF, PIN_RF, true, PIN_Buzzer, PIN_Trigger, PIN_Echo);
  Aren.sing(S_connection);
  Aren.home();
}

// -- Main Program loop
void loop()
{
  if (isObstacleDetected == true)
  {
    Aren.sing(S_surprise);
    Aren.jump(500);
    Aren.walk(3, 1000, BACKWARD);
    delay(500);
    Aren.turn(3, 1000, 1);
    delay(500);
    isObstacleDetected = false;
  }
  else
  {
    Aren.walk(1, 1000, FORWARD);
    checkForObstacles();
  }
}

//-- Functions
/*
  FUNCTION
  void checkForObstacles()
 
  Read the distance from the nearest object & flag whether an obstacle has been detected.
*/
void checkForObstacles()
{
  int distance = Aren.getDistance();
  Serial.println(distance);
  if (distance < 15)
  {
    isObstacleDetected = true;
  }
  else
  {
    isObstacleDetected = false;
  }
}