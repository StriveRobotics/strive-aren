/*
 * -- AREN Music Program
 * 
 * -- ABOUT
 *    By default, it plays the "Happy Birthday" song using the Buzzer
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
int loopCount;     // For storing the current number of playbacks
int loopLimit = 5; // Change this value to change the number of times the song plays

float tempo = 140.00;         // Change this value to change how fast the song plays. The notes will scale accordingly
const int songNoteCount = 25; // The total number of notes the current song has

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
notes songData1[songNoteCount] = {
    {.frequency = note_C5, .noteType = dottedQuarterNote},
    {.frequency = note_C5, .noteType = quarterNote},
    {.frequency = note_D5, .noteType = quarterNote},
    {.frequency = note_C5, .noteType = quarterNote},
    {.frequency = note_F5, .noteType = quarterNote},
    {.frequency = note_E5, .noteType = quarterNote},

    {.frequency = note_silent, .noteType = dottedQuarterNote},
    {.frequency = note_C5, .noteType = quarterNote},
    {.frequency = note_D5, .noteType = quarterNote},
    {.frequency = note_C5, .noteType = quarterNote},
    {.frequency = note_G5, .noteType = quarterNote},
    {.frequency = note_F5, .noteType = quarterNote},

    {.frequency = note_C5, .noteType = dottedQuarterNote},
    {.frequency = note_C5, .noteType = quarterNote},
    {.frequency = note_C6, .noteType = quarterNote},
    {.frequency = note_A5, .noteType = quarterNote},
    {.frequency = note_F5, .noteType = quarterNote},
    {.frequency = note_E5, .noteType = quarterNote},
    {.frequency = note_D5, .noteType = quarterNote},

    {.frequency = note_Bb5, .noteType = dottedQuarterNote},
    {.frequency = note_Bb5, .noteType = quarterNote},
    {.frequency = note_A5, .noteType = quarterNote},
    {.frequency = note_F5, .noteType = quarterNote},
    {.frequency = note_G5, .noteType = quarterNote},
    {.frequency = note_F5, .noteType = wholeNote},
};

// -- Program Setup
void setup()
{
  Serial.begin(9600);
  Aren.init(PIN_LL, PIN_RL, PIN_LF, PIN_RF, true, PIN_Buzzer, PIN_Trigger, PIN_Echo);
  Aren.home();
  Aren.sing(S_mode3);
  Serial.println("AREN is Ready");
}

// -- Main Program loop
void loop()
{

  while (loopCount != loopLimit)
  {
    for (size_t i = 0; i < songNoteCount; i++)
    {
      Aren._tone(songData1[i].frequency, long(songData1[i].noteType * 0.2), int(songData1[i].noteType * 0.8));
      // tone(PIN_Buzzer, songData1[i].frequency, long(songData1[i].noteType * 0.2));
    }
    loopCount++;
  }

  Aren.home();
  delay(4000);
  Aren.setRestState(false);
}