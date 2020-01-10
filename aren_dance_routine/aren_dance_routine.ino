/*
 * -- AREN Dance Routine 1
 * 
 * -- ABOUT
 *    AREN will dance along to the following song: Seven Nation Army - Glitch Mob Remix
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

// -- Global Variable Declaration
unsigned int loopCount = 0;    // This is the counter for the loop
unsigned int endLoopCount = 2; // Change this value to set the number of times the program will run

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
  while (loopCount != endLoopCount)
  {
    Aren.sing(S_mode3); // 00:00 Sync with the song by playing the song on the media device of your choice.
    delay(500);

    moveToSong();

    delay(5000);
    Aren.home();              // Reset all the servos to their zero positions
    Aren.setRestState(false); // Set the rest state to false in order to be able to call the .home() function again

    loopCount++; // Increment the counter
  }
}

// -- Functions
/*
  FUNCTION
  void moveToSong()
 
  This houses the synced movement sequence for the dance routine
*/
void moveToSong()
{
  Aren.tap(8, 400, LEFT);
  Aren.tap(8, 400, RIGHT);
  Aren.tap(8, 400, LEFT);
  Aren.tap(9, 400, RIGHT); //00:16

  Aren.jump(500); //00:23
  Aren.jump(500);
  Aren.jump(500);
  Aren.jump(500);
  Aren.jump(500);
  Aren.jump(500);
  Aren.jump(500);
  Aren.jump(500);

  Aren.jump(250); //00:32
  Aren.jump(250);
  Aren.jump(250);
  Aren.jump(250);
  Aren.jump(250);
  Aren.jump(250);
  Aren.jump(250);
  Aren.jump(250);

  Aren.jump(250);
  Aren.jump(250);
  Aren.jump(250);
  Aren.jump(250);
  Aren.jump(250);
  Aren.jump(250);
  Aren.jump(250);
  Aren.jump(250);

  Aren.flapping(8, 1000, 20, BACKWARD); //00:40
  Aren.flapping(7, 1000, 20, FORWARD);  //00:48
  delay(1000);
  Aren.swing(8, 1000, 20);       //00:56
  Aren.tiptoeSwing(8, 1000, 30); //01:04

  Aren.tiptoeWave(2, 1000, 30, LEFT);
  Aren.tiptoeWave(2, 1000, 30, RIGHT); //01:08
  Aren.walk(4, 1000, FORWARD);
  Aren.walk(3, 1000, BACKWARD); //1:14
  delay(300);
  Aren.jump(300); //01:15
  Aren.walk(4, 1000, FORWARD);
  Aren.walk(4, 1000, BACKWARD); //01:21
  Aren.diagonalLeanBackRight(3, 800);
  Aren.diagonalLeanBackRight(1, 2000); //01:29

  Aren.tap(7, 400, LEFT);
  Aren.tap(8, 400, RIGHT); //01:32
  Aren.tap(8, 400, LEFT);  //01:39
  Aren.tap(9, 400, RIGHT); //01:44

  Aren.tiptoeSwing(10, 500, 30);
  Aren.kick(1, 900, LEFT); //01:49
  delay(100);
  Aren.kick(1, 900, RIGHT);
  delay(100);
  Aren.tiptoeSwing(8, 500, 30); //01:56
  Aren.updown(3, 1000, 30);     //02:01

  Aren.walk(4, 1000, FORWARD);  //2:04
  Aren.walk(4, 1000, BACKWARD); //02:08
  Aren.updown(1, 1000, 30);     //02:09
  Aren.walk(2, 1000, FORWARD);
  Aren.walk(2, 1000, FORWARD);
  Aren.tiptoeWave(2, 1000, 30, LEFT);
  Aren.tiptoeWave(2, 1000, 30, RIGHT);

  Aren.updown(4, 1000, 30); //02:20

  Aren.shuffle(6, 1000, LEFT);
  Aren.footWave(1, 1000, LEFT);
  Aren.footWave(1, 1000, RIGHT);
  Aren.shuffle(7, 1000, RIGHT); //02:36

  Aren.tiptoeWave(4, 1000, 30, LEFT);
  Aren.jump(500);
  Aren.jump(500);
  Aren.tiptoeWave(2, 1000, 30, RIGHT);
  Aren.jump(300); //02:44
  Aren.tiptoeWave(2, 1000, 30, LEFT);
  Aren.kick(1, 600, RIGHT);
  Aren.jitter(2, 350, 30);
  Aren.kick(1, 600, LEFT);
  Aren.jitter(2, 350, 30);

  Aren.jump(1000); //02:53
  Aren.jump(400);
  Aren.diagonalLeanBackRight(1, 1000);
  // Until 2:56
}