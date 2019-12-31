#include <Aren.h>

Aren Aren;

#define PIN_YL 2 //servo[2]
#define PIN_YR 3 //servo[3]
#define PIN_RL 4 //servo[4]
#define PIN_RR 5 //servo[5]

int loopCount; // For debugging

float tempo = 140.00;
const int songNoteCount = 25;

float wholeNote = 240.0/tempo * 1000;
float halfNote = 120.0/tempo * 1000;
float quarterNote = 60.0/tempo * 1000;
float eigthNote = 30.0/tempo * 1000;
float sixteenthNote = 15.0/tempo * 1000;
float dottedQuarterNote = 90.0/tempo * 1000;
float dottedEightNote = 45.0/tempo * 1000;
float dottedSixteenthNote = 22.5/tempo * 100;

struct notes {
  float frequency;
  float noteType;
};

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


void setup() {
  Serial.begin(198);
  // Yl, YR, Rl, RR, load-calibration, noise sensor, buzzer, ultrasonic trigger, ultrasonic echo
  Aren.init(PIN_YL,PIN_YR,PIN_RL,PIN_RR,true,PIN_NoiseSensor, 13, 8, 9);
  Aren.home();

  pinMode(PIN_Echo, INPUT);
  pinMode(PIN_Trigger, OUTPUT);
}

void loop() {

  while (loopCount != 5) {
    for (size_t i = 0; i < songNoteCount; i++)
    {
      Aren._tone(songData1[i].frequency, long(songData1[i].noteType * 0.2), int(songData1[i].noteType * 0.8));

      // tone(PIN_Buzzer, songData1[i].frequency, long(songData1[i].noteType * 0.2));
    }
    loopCount++;
  }

  Aren.home();
  delay(4000);
}