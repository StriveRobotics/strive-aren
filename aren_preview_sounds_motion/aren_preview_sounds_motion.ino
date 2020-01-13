/*
 * -- AREN Debug tool - All Sounds & Motion
 * 
 * -- ABOUT
 *    For previewing all available sounds and movements starting with sounds and followed by movements.
 *    The program will zero the servos after every loop cycle.
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

// -- Global Variables Declaration
// For Debugging
unsigned short int loopCount = 0;    // This is the counter for the loop
unsigned short int endLoopCount = 2; // Change this value to set the number of times the program will run

/*
  CHANGING MODES
  Change the value of cycleType below based on what you want the program to cycle through.

  The available modes are as follows:
    SOUND = 1,
    MOTION = 2,
    BOTH = 3,
    ZERO SERVOS ONLY = <Any other number>
*/
unsigned short int cycleType = 1;

// -- Program Setup
void setup()
{
  // Open the serial port at stated data rate
  Serial.begin(9600);

  // Initialise the AREN
  Aren.init(PIN_LL, PIN_RL, PIN_LF, PIN_RF, true, PIN_Buzzer, PIN_Trigger, PIN_Echo);
  Aren.home();
  Aren.sing(S_mode3);
  Serial.println("AREN is ready");
}

// -- Main Program Loop
void loop()
{
  if (loopCount < endLoopCount)
  {
    // Run the program mode based on the cycleType variable above
    switch (cycleType) {
      case 1:
        cycleAllSounds();
        break;
      case 2:
        cycleAllMovements();
        break;
      case 3:
        cycleAllSounds();
        cycleAllMovements();
        break;
      default:
        break;
    }
    Aren.sing(S_sleeping);
    loopCount++;
  }

  Aren.home();
  delay(1000);
  Aren.setRestState(false);
}

// -- Functions
/*
  FUNCTION
  void cycleAllSounds()
 
  Will cycle through all the preprogrammed sounds available for use.
*/
void cycleAllSounds()
{
  Serial.println(F("Now cycling through all sounds..."));
  Serial.println(F("Now playing sound: Connection"));
  Aren.sing(S_connection);
  delay(2000);
  Serial.println(F("Now playing sound: Disconnection"));
  Aren.sing(S_disconnection);
  delay(2000);
  Serial.println(F("Now playing sound: Button Pushed"));
  Aren.sing(S_buttonPushed);
  delay(2000);
  Serial.println(F("Now playing sound: Mode 1"));
  Aren.sing(S_mode1);
  delay(2000);
  Serial.println(F("Now playing sound: Mode 2"));
  Aren.sing(S_mode2);
  delay(2000);
  Serial.println(F("Now playing sound: Mode 3"));
  Aren.sing(S_mode3);
  delay(2000);
  Serial.println(F("Now playing sound: Surprise"));
  Aren.sing(S_surprise);
  delay(2000);
  Serial.println(F("Now playing sound: OhOoh"));
  Aren.sing(S_OhOoh);
  delay(2000);
  Serial.println(F("Now playing sound: OhOoh2"));
  Aren.sing(S_OhOoh2);
  delay(2000);
  Serial.println(F("Now playing sound: Cuddly"));
  Aren.sing(S_cuddly);
  delay(2000);
  Serial.println(F("Now playing sound: Sleeping"));
  Aren.sing(S_sleeping);
  delay(2000);
  Serial.println(F("Now playing sound: Happy"));
  Aren.sing(S_happy);
  delay(2000);
  Serial.println(F("Now playing sound: Super Happy"));
  Aren.sing(S_superHappy);
  delay(2000);
  Serial.println(F("Now playing sound: Happy Short"));
  Aren.sing(S_happy_short);
  delay(2000);
  Serial.println(F("Now playing sound: Sad"));
  Aren.sing(S_sad);
  delay(2000);
  Serial.println(F("Now playing sound: Confused"));
  Aren.sing(S_confused);
  delay(2000);
  Serial.println(F("All sounds played!"));

  Serial.println(F("Pausing for 4 seconds..."));
  delay(4000);
}

/*
  FUNCTION
  void cycleAllMovements()
 
  Will cycle through all the preprogrammed movements available for use.
*/
void cycleAllMovements()
{
  Serial.println(F("Now cycling through all movements..."));

  Serial.println(F("Current movement: Tap Left"));
  Aren.tap(5, 1000, LEFT);
  delay(2000);
  Serial.println(F("Current movement: Tap Right"));
  Aren.tap(5, 1000, RIGHT);
  delay(2000);

  Serial.println(F("Current movement: Foot wave left"));
  Aren.footWave(5, 1000, LEFT);
  delay(2000);
  Serial.println(F("Current movement: Foot wave right"));
  Aren.footWave(5, 1000, RIGHT);
  delay(2000);

  Serial.println(F("Current movement: Lean Back Right"));
  Aren.diagonalLeanBackRight(10, 1000);
  delay(2000);
  Aren.diagonalLeanBackRight(10, 1000);
  delay(2000);

  Serial.println(F("Current movement: Kick Right"));
  Aren.kick(3, 1000, RIGHT);
  delay(2000);
  Serial.println(F("Current movement: Kick Right"));
  Aren.kick(3, 1000, LEFT);
  delay(2000);

  Serial.println(F("Current movement: Shuffle Left"));
  Aren.shuffle(5, 1000, LEFT);
  delay(2000);
  Serial.println(F("Current movement: Shuffle Right"));
  Aren.shuffle(5, 1000, RIGHT);
  delay(2000);

  Serial.println(F("Current movement: Walk forward"));
  Aren.walk(4, 1000, FORWARD);
  delay(2000);
  Serial.println(F("Current movement: Walk backward"));
  Aren.walk(4, 1000, BACKWARD);
  delay(2000);

  Serial.println(F("Current movement: Jump"));
  Aren.jump(1000);
  delay(2000);

  Serial.println(F("Current movement: Turn Left"));
  Aren.turn(4, 1000, LEFT);
  delay(2000);
  Serial.println(F("Current movement: Turn Right"));
  Aren.turn(4, 1000, RIGHT);
  delay(2000);

  Serial.println(F("Current movement: Walk Back"));
  Aren.walk(8, 1000, BACKWARD);
  delay(2000);

  Serial.println(F("Current movement: Bend Left"));
  Aren.bend(4, 1000, LEFT);
  delay(2000);
  Serial.println(F("Current movement: Bend Right"));
  Aren.bend(4, 1000, RIGHT);
  delay(2000);

  Serial.println(F("Current movement: Shake Left Leg"));
  Aren.shakeLeg(4, 1000, LEFT);
  delay(2000);
  Serial.println(F("Current movement: Shake Right Leg"));
  Aren.shakeLeg(4, 1000, RIGHT);
  delay(2000);

  Serial.println(F("Current movement: Up & Down"));
  Aren.updown(4, 1000, LARGE);
  delay(2000);

  Serial.println(F("Current movement: Swing"));
  Aren.swing(4, 1000, 40);
  delay(2000);
  Serial.println(F("Current movement: Tip Toe Swing"));
  Aren.tiptoeSwing(4, 1000, 40);
  delay(2000);

  Serial.println(F("Current movement: Jitter"));
  Aren.jitter(4, 1000, 15);
  delay(2000);

  Serial.println(F("Current movement: Ascending Turn"));
  Aren.ascendingTurn(4, 1000, 10);
  delay(2000);

  Serial.println(F("Current movement: Wave Left"));
  Aren.wave(4, 1000, 30, LEFT);
  delay(2000);
  Serial.println(F("Current movement: Wave Right"));
  Aren.wave(4, 1000, 30, RIGHT);
  delay(2000);

  Serial.println(F("Current movement: Tiptoe Wave Left"));
  Aren.tiptoeWave(4, 1000, 35, LEFT);
  delay(2000);
  Serial.println(F("Current movement: Tiptoe Wave Right"));
  Aren.tiptoeWave(4, 1000, 35, RIGHT);
  delay(2000);

  Serial.println(F("Current movement: Flapping Forward"));
  Aren.flapping(4, 1000, 20, FORWARD);
  delay(2000);
  Serial.println(F("Current movement: Flapping Backward"));
  Aren.flapping(4, 1000, 20, BACKWARD);

  Serial.println(F("All movements cycled through!"));

  Serial.println(F("Pausing for 4 seconds..."));
  delay(4000);
}