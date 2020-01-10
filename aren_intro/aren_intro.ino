/*
 * -- AREN Welcome
 * 
 * -- ABOUT
 *    An introductory program that will test whether your bot is working properly.
 *    This program will also bring you through some of the capabilities of the bot.
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
int loopCount = 0;
int loopLimit = 1;
int loopCount2 = 0;
int loopLimit2 = 10;

int distance;                    // variable to store distance read from ultrasonic range finder module
bool isObstacleDetected = false; // logic state for when object detected is at the distance we set

float tempo = 140.00;         // Change this value to change how fast the song plays. The notes will scale accordingly
const int songNoteCount = 33; // The total number of notes the current song has

// DO NOT CHANGE THE FOLLOWING SECTION
float wholeNote = 240.0 / tempo * 1000;
float halfNote = 120.0 / tempo * 1000;
float quarterNote = 60.0 / tempo * 1000;
float eigthNote = 30.0 / tempo * 1000;
float sixteenthNote = 15.0 / tempo * 1000;
float dottedQuarterNote = 90.0 / tempo * 1000;
float dottedEightNote = 45.0 / tempo * 1000;
float dottedSixteenthNote = 22.5 / tempo * 100;

struct notes
{
  float frequency;
  float noteType;
};

// -- Song Data
//    An array of type notes which has a frequency and note type variable.
notes songData[songNoteCount] = {
    {.frequency = note_D4, .noteType = quarterNote},
    {.frequency = note_E4, .noteType = quarterNote},

    {.frequency = note_G4, .noteType = quarterNote},
    {.frequency = note_G4, .noteType = quarterNote},
    {.frequency = note_G4, .noteType = quarterNote},
    {.frequency = note_G4, .noteType = eigthNote},
    {.frequency = note_G4, .noteType = eigthNote},

    {.frequency = note_G4, .noteType = eigthNote},
    {.frequency = note_G4, .noteType = eigthNote},
    {.frequency = note_G4, .noteType = quarterNote},
    {.frequency = note_D4, .noteType = quarterNote},
    {.frequency = note_E4, .noteType = quarterNote},

    {.frequency = note_G4, .noteType = quarterNote},
    {.frequency = note_G4, .noteType = quarterNote},
    {.frequency = note_G4, .noteType = quarterNote},
    {.frequency = note_G4, .noteType = eigthNote},
    {.frequency = note_G4, .noteType = eigthNote},

    {.frequency = note_G4, .noteType = eigthNote},
    {.frequency = note_G4, .noteType = eigthNote},
    {.frequency = note_G4, .noteType = quarterNote},
    {.frequency = note_D4, .noteType = quarterNote},
    {.frequency = note_E4, .noteType = quarterNote},

    {.frequency = note_G4, .noteType = quarterNote},
    {.frequency = note_G4, .noteType = quarterNote},
    {.frequency = note_G4, .noteType = quarterNote},
    {.frequency = note_G4, .noteType = eigthNote},
    {.frequency = note_G4, .noteType = eigthNote},

    {.frequency = note_G4, .noteType = eigthNote},
    {.frequency = note_G4, .noteType = eigthNote},
    {.frequency = note_G4, .noteType = quarterNote},
    {.frequency = note_G4, .noteType = quarterNote},
    {.frequency = note_G4, .noteType = quarterNote},

    {.frequency = note_F4, .noteType = wholeNote},
};

// -- Program Setup
void setup()
{
  // Open the serial port at stated data rate
  Serial.begin(9600);

  // Initialise the AREN
  Aren.init(PIN_LL, PIN_RL, PIN_LF, PIN_RF, true, PIN_Buzzer, PIN_Trigger, PIN_Echo);
  Aren.home();
  Aren.sing(S_mode3);
  Serial.println("AREN is Ready");
}

// -- Main Program Loop
void loop()
{
  if (loopCount < loopLimit)
  {
    delay(2000);
    Aren.sing(S_happy);
    Serial.println("Hello there! My name is AREN, nice to meet you :D");
    delay(3000);
    Serial.println("Here, let me show you what I can do.");
    delay(2500);

    Serial.println("I can walk...");
    Aren.walk(2, 1000, FORWARD);
    Aren.walk(2, 1000, BACKWARD);
    delay(2000);

    Serial.println("I can sing...");
    playSong();
    delay(2000);

    Serial.println("I can dance...");
    Aren.tiptoeSwing(4, 1000, MEDIUM);
    delay(2000);

    Serial.println("I can sense how far away you are in front of me...");
    while (loopCount2 < loopLimit2) {
      int distance = Aren.getDistance();
      Serial.println(distance);
      loopCount2++;
      delay(100);
    }
    delay(2000);

    Serial.println("And many more!");
    delay(2000);
    Serial.println("Let's have fun, and learn together!");
    delay(2000);
    loopCount++;
  }

  Aren.home();
  delay(1000);
  Aren.setRestState(false);
}

//-- Functions
/*
  FUNCTION
  void playSong()
 
  Play the song until it ends
*/
void playSong()
{
  for (size_t i = 0; i < songNoteCount; i++)
  {
    Aren._tone(songData[i].frequency, long(songData[i].noteType * 0.2), int(songData[i].noteType * 0.8));
  }
}