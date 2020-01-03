/*
 * -- AREN Source File
 * -- Derived from the open source project, OttoDIY available at https://www.ottodiy.com
 * 
 */

// -- Check whether to include the Arduino Library
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif

// -- Include headers
#include <Aren.h>

void Aren::init(int LL, int RL, int LF, int RF, bool load_calibration, int Buzzer, int USTrigger, int USEcho)
{
  servo_pins[0] = LL;
  servo_pins[1] = RL;
  servo_pins[2] = LF;
  servo_pins[3] = RF;

  attachServos();
  isResting = false;

  if (load_calibration)
  {
    for (int i = 0; i < 4; i++)
    {
      int servo_trim = EEPROM.read(i);
      if (servo_trim > 128)
        servo_trim -= 256;
      servo[i].SetTrim(servo_trim);
    }
  }

  for (int i = 0; i < 4; i++)
  {
    servo_position[i] = 90;
  }

  // US sensor init with the pins:
  Aren::us.init(USTrigger, USEcho);

  // Buzzer pins:
  pinBuzzer = Buzzer;
  pinMode(Buzzer, OUTPUT);
}

///////////////////////////////////////////////////////////////////
//-- ATTACH & DETACH FUNCTIONS ----------------------------------//
///////////////////////////////////////////////////////////////////

void Aren::attachServos()
{
  servo[0].attach(servo_pins[0]);
  servo[1].attach(servo_pins[1]);
  servo[2].attach(servo_pins[2]);
  servo[3].attach(servo_pins[3]);
}

void Aren::detachServos()
{
  servo[0].detach();
  servo[1].detach();
  servo[2].detach();
  servo[3].detach();
}

///////////////////////////////////////////////////////////////////
//-- OSCILLATORS TRIMS ------------------------------------------//
///////////////////////////////////////////////////////////////////

void Aren::setTrims(int YL, int YR, int RL, int RR)
{
  servo[0].SetTrim(YL);
  servo[1].SetTrim(YR);
  servo[2].SetTrim(RL);
  servo[3].SetTrim(RR);
}

void Aren::saveTrimsOnEEPROM()
{
  for (int i = 0; i < 4; i++)
  {
    EEPROM.write(i, servo[i].getTrim());
  }
}

///////////////////////////////////////////////////////////////////
//-- BASIC MOTION FUNCTIONS -------------------------------------//
///////////////////////////////////////////////////////////////////

void Aren::_moveServos(int time, int servo_target[])
{
  attachServos();
  if (getRestState() == true)
  {
    setRestState(false);
  }

  if (time > 10)
  {
    for (int i = 0; i < 4; i++)
    {
      increment[i] = ((servo_target[i]) - servo_position[i]) / (time / 10.0);
    }
    final_time = millis() + time;

    for (int iteration = 1; millis() < final_time; iteration++)
    {
      partial_time = millis() + 10;
      for (int i = 0; i < 4; i++)
      {
        servo[i].SetPosition(servo_position[i] + (iteration * increment[i]));
      }
      while (millis() < partial_time)
      {
      }; //pause
    }
  }
  else
  {
    for (int i = 0; i < 4; i++)
    {
      servo[i].SetPosition(servo_target[i]);
    }
  }
  for (int i = 0; i < 4; i++)
  {
    servo_position[i] = servo_target[i];
  }
}

void Aren::oscillateServos(int A[4], int O[4], int T, double phase_diff[4], float cycle = 1)
{
  for (int i = 0; i < 4; i++)
  {
    servo[i].SetO(O[i]);
    servo[i].SetA(A[i]);
    servo[i].SetT(T);
    servo[i].SetPh(phase_diff[i]);
  }

  double ref = millis();
  for (double x = ref; x <= T * cycle + ref; x = millis())
  {
    for (int i = 0; i < 4; i++)
    {
      servo[i].refresh();
    }
  }
}

void Aren::_execute(int A[4], int O[4], int T, double phase_diff[4], float steps = 1.0)
{
  attachServos();
  if (getRestState() == true)
  {
    setRestState(false);
  }

  int cycles = (int)steps;

  //-- Execute complete cycles
  if (cycles >= 1)
  {
    for (int i = 0; i < cycles; i++)
    {
      oscillateServos(A, O, T, phase_diff);
    }
  }
  //-- Execute the final not complete cycle
  oscillateServos(A, O, T, phase_diff, (float)steps - cycles);
}

///////////////////////////////////////////////////////////////////
//-- HOME = Aren at rest position -------------------------------//
///////////////////////////////////////////////////////////////////

void Aren::home()
{
  if (isResting == false)
  { //Go to rest position only if necessary

    int homes[4] = {90, 90, 90, 90}; //All the servos at rest position
    _moveServos(500, homes);         //Move the servos in half a second

    detachServos();
    isResting = true;
  }
}

bool Aren::getRestState()
{
  return isResting;
}

void Aren::setRestState(bool state)
{
  isResting = state;
}

///////////////////////////////////////////////////////////////////
//-- SENSORS FUNCTIONS  -----------------------------------------//
///////////////////////////////////////////////////////////////////

//---------------------------------------------------------
//-- getDistance: return ultrasonic sensor measure
//---------------------------------------------------------
float Aren::getDistance()
{
  return us.read();
}

///////////////////////////////////////////////////////////////////
//-- SOUNDS -----------------------------------------------------//
///////////////////////////////////////////////////////////////////

void Aren::_tone(float noteFrequency, long noteDuration, int silentDuration)
{
  if (silentDuration == 0)
  {
    silentDuration = 1;
  }

  tone(Aren::pinBuzzer, noteFrequency, noteDuration);
  delay(noteDuration); //milliseconds to microseconds
  //noTone(PIN_Buzzer);
  delay(silentDuration);
}

void Aren::bendTones(float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration)
{

  //Examples:
  //  bendTones (880, 2093, 1.02, 18, 1);
  //  bendTones (note_A5, note_C7, 1.02, 18, 0);

  if (silentDuration == 0)
  {
    silentDuration = 1;
  }

  if (initFrequency < finalFrequency)
  {
    for (int i = initFrequency; i < finalFrequency; i = i * prop)
    {
      _tone(i, noteDuration, silentDuration);
    }
  }
  else
  {
    for (int i = initFrequency; i > finalFrequency; i = i / prop)
    {
      _tone(i, noteDuration, silentDuration);
    }
  }
}

void Aren::sing(int musicName)
{
  switch (musicName)
  {

  case 19:
    _tone(note_E5, 50, 0);
    break;

  case S_connection:
    _tone(note_E5, 50, 30);
    _tone(note_E6, 55, 25);
    _tone(note_A6, 60, 10);
    break;

  case S_disconnection:
    _tone(note_E5, 50, 30);
    _tone(note_A6, 55, 25);
    _tone(note_E6, 50, 10);
    break;

  case S_buttonPushed:
    bendTones(note_E6, note_G6, 1.03, 20, 2);
    delay(30);
    bendTones(note_E6, note_D7, 1.04, 10, 2);
    break;

  case S_mode1:
    bendTones(note_E6, note_A6, 1.02, 30, 10); //1318.51 to 1760
    break;

  case S_mode2:
    bendTones(note_G6, note_D7, 1.03, 30, 10); //1567.98 to 2349.32
    break;

  case S_mode3:
    _tone(note_E6, 50, 100); //D6
    _tone(note_G6, 50, 80);  //E6
    _tone(note_D7, 300, 0);  //G6
    break;

  case S_surprise:
    bendTones(800, 2150, 1.02, 10, 1);
    bendTones(2149, 800, 1.03, 7, 1);
    break;

  case S_OhOoh:
    bendTones(880, 2000, 1.04, 8, 3); //A5 = 880
    delay(200);

    for (int i = 880; i < 2000; i = i * 1.04)
    {
      _tone(note_B5, 5, 10);
    }
    break;

  case S_OhOoh2:
    bendTones(1880, 3000, 1.03, 8, 3);
    delay(200);

    for (int i = 1880; i < 3000; i = i * 1.03)
    {
      _tone(note_C6, 10, 10);
    }
    break;

  case S_cuddly:
    bendTones(700, 900, 1.03, 16, 4);
    bendTones(899, 650, 1.01, 18, 7);
    break;

  case S_sleeping:
    bendTones(100, 500, 1.04, 10, 10);
    delay(500);
    bendTones(400, 100, 1.04, 10, 1);
    break;

  case S_happy:
    bendTones(1500, 2500, 1.05, 20, 8);
    bendTones(2499, 1500, 1.05, 25, 8);
    break;

  case S_superHappy:
    bendTones(2000, 6000, 1.05, 8, 3);
    delay(50);
    bendTones(5999, 2000, 1.05, 13, 2);
    break;

  case S_happy_short:
    bendTones(1500, 2000, 1.05, 15, 8);
    delay(100);
    bendTones(1900, 2500, 1.05, 10, 8);
    break;

  case S_sad:
    bendTones(880, 669, 1.02, 20, 200);
    break;

  case S_confused:
    bendTones(1000, 1700, 1.03, 8, 2);
    bendTones(1699, 500, 1.04, 8, 3);
    bendTones(1000, 1700, 1.05, 9, 10);
    break;
  }
}

//////////////////////////////////////////////////////////////////
//-- MOTION SEQUENCES ------------------------------------------//
//////////////////////////////////////////////////////////////////

void Aren::walkDebug()
{
  // {left hip angle, right hip angle, left foot angle, right foot angle}

  /*
  int motion1[4] = {90, 90, 90, 90};
  int motion2[4] = {125, 125, 125, 125};
  int motion3[4] = {45, 45, 45, 45};
  _moveServos(800, motion1);
  _moveServos(800, motion3);
  _moveServos(1600, motion2);
 */

  int motion4[4] = {125, 125, 100, 105};
  int motion5[4] = {55, 55, 100, 105};
  int motion6[4] = {55, 55, 75, 80};
  int motion7[4] = {125, 125, 75, 80};
  _moveServos(1000, motion4);
  _moveServos(500, motion5);
  _moveServos(1000, motion6);
  _moveServos(500, motion7);

  // int motion1[4] = {125, 125, 125, 125};
  // _moveServos(500, motion1);
}

//---------------------------------------------------------
//-- Aren motion: Walking  (forward or backward)
//--  Parameters:
//--    steps:  Number of steps
//--    T : Period
//--    Dir: Direction: FORWARD / BACKWARD
//---------------------------------------------------------
void Aren::walk(float steps, int T, int dir)
{

  // Oscillator parameters for walking
  // Both hip servos are in phase. Both feet servos are in phase
  // However, hip and feet are 90 degrees out of phase
  //-- -90 : Walk forward
  //--  90 : Walk backward

  // Offsets alues for
  int A[4] = {30, 30, 20, 20};
  int O[4] = {0, 0, 4, -4};
  double phase_diff[4] = {0, 0, DEG2RAD(dir * -90), DEG2RAD(dir * -90)}; // change dir to 1 to more forward and -1 to move backwards

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Aren movement: Jump
//--  Parameters:
//--    T: Period
//---------------------------------------------------------
void Aren::jump(int T)
{

  int up[] = {90, 90, 150, 30};
  _moveServos(T, up);
  int down[] = {90, 90, 90, 90};
  _moveServos(T, down);
}

//---------------------------------------------------------
//-- Aren motion: Turning (left or right)
//--  Parameters:
//--   * Steps: Number of steps
//--   * T: Period
//--   * Dir: Direction: LEFT / RIGHT
//---------------------------------------------------------
void Aren::turn(float steps, int T, int dir)
{

  //-- Same coordination than for walking (see Aren::walk)
  //-- The Amplitudes of the hip's oscillators are not igual
  //-- When the right hip servo amplitude is higher, the steps taken by
  //--   the right leg are bigger than the left. So, the robot describes an
  //--   left arc
  int A[4] = {30, 30, 20, 20};
  int O[4] = {0, 0, 4, -4};
  double phase_diff[4] = {0, 0, DEG2RAD(-90), DEG2RAD(-90)};

  if (dir == LEFT)
  {
    A[0] = 30; //-- Left hip servo
    A[1] = 10; //-- Right hip servo
  }
  else
  {
    A[0] = 10;
    A[1] = 30;
  }

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Aren motion: Lateral bend
//--  Parameters:
//--    steps: Number of bends
//--    T: Period of one bend
//--    dir: RIGHT=Right bend LEFT=Left bend
//---------------------------------------------------------
void Aren::bend(int steps, int T, int dir)
{

  //Parameters of all the movements. Default: Left bend
  int bend1[4] = {90, 90, 62, 35};
  int bend2[4] = {90, 90, 62, 105};
  int homes[4] = {90, 90, 90, 90};

  //Time of one bend, constrained in order to avoid movements too fast.
  //T=max(T, 600);

  //Changes in the parameters if right direction is chosen
  if (dir == -1)
  {
    bend1[2] = 180 - 35;
    bend1[3] = 180 - 60; //Not 65. Aren is unbalanced
    bend2[2] = 180 - 105;
    bend2[3] = 180 - 60;
  }

  //Time of the bend movement. Fixed parameter to avoid falls
  int T2 = 800;

  //Bend movement
  for (int i = 0; i < steps; i++)
  {
    _moveServos(T2 / 2, bend1);
    _moveServos(T2 / 2, bend2);
    delay(T * 0.8);
    _moveServos(500, homes);
  }
}

//---------------------------------------------------------
//-- Aren motion: Shake a leg
//--  Parameters:
//--    steps: Number of shakes
//--    T: Period of one shake
//--    dir: RIGHT=Right leg LEFT=Left leg
//---------------------------------------------------------
void Aren::shakeLeg(int steps, int T, int dir)
{

  //This variable change the amount of shakes
  int numberLegMoves = 2;

  //Parameters of all the movements. Default: Right leg
  int shake_leg1[4] = {90, 90, 58, 35};
  int shake_leg2[4] = {90, 90, 58, 120};
  int shake_leg3[4] = {90, 90, 58, 60};
  int homes[4] = {90, 90, 90, 90};

  //Changes in the parameters if left leg is chosen
  if (dir == -1)
  {
    shake_leg1[2] = 180 - 35;
    shake_leg1[3] = 180 - 58;
    shake_leg2[2] = 180 - 120;
    shake_leg2[3] = 180 - 58;
    shake_leg3[2] = 180 - 60;
    shake_leg3[3] = 180 - 58;
  }

  //Time of the bend movement. Fixed parameter to avoid falls
  int T2 = 1000;
  //Time of one shake, constrained in order to avoid movements too fast.
  T = T - T2;
  T = max(T, 200 * numberLegMoves);

  for (int j = 0; j < steps; j++)
  {
    //Bend movement
    _moveServos(T2 / 2, shake_leg1);
    _moveServos(T2 / 2, shake_leg2);

    //Shake movement
    for (int i = 0; i < numberLegMoves; i++)
    {
      _moveServos(T / (2 * numberLegMoves), shake_leg3);
      _moveServos(T / (2 * numberLegMoves), shake_leg2);
    }
    _moveServos(500, homes); //Return to home position
  }

  delay(T);
}

//---------------------------------------------------------
//-- Aren movement: up & down
//--  Parameters:
//--    * steps: Number of jumps
//--    * T: Period
//--    * h: Jump height: SMALL / MEDIUM / BIG
//--              (or a number in degrees 0 - 90)
//---------------------------------------------------------
void Aren::updown(float steps, int T, int h)
{

  //-- Both feet are 180 degrees out of phase
  //-- Feet amplitude and offset are the same
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  int A[4] = {0, 0, h, h};
  int O[4] = {0, 0, h, -h};
  double phase_diff[4] = {0, 0, DEG2RAD(-90), DEG2RAD(90)};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Aren movement: swinging side to side
//--  Parameters:
//--     steps: Number of steps
//--     T : Period
//--     h : Amount of swing (from 0 to 50 aprox)
//---------------------------------------------------------
void Aren::swing(float steps, int T, int h)
{

  //-- Both feets are in phase. The offset is half the amplitude
  //-- It causes the robot to swing from side to side
  int A[4] = {0, 0, h, h};
  int O[4] = {0, 0, h / 2, -h / 2};
  double phase_diff[4] = {0, 0, DEG2RAD(0), DEG2RAD(0)};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Aren movement: swinging side to side without touching the floor with the heel
//--  Parameters:
//--     steps: Number of steps
//--     T : Period
//--     h : Amount of swing (from 0 to 50 aprox)
//---------------------------------------------------------
void Aren::tiptoeSwing(float steps, int T, int h)
{

  //-- Both feets are in phase. The offset is not half the amplitude in order to tiptoe
  //-- It causes the robot to swing from side to side
  int A[4] = {0, 0, h, h};
  int O[4] = {0, 0, h, -h};
  double phase_diff[4] = {0, 0, 0, 0};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Aren motion: Jitter
//--  Parameters:
//--    steps: Number of jitters
//--    T: Period of one jitter
//--    h: height (Values between 5 - 25)
//---------------------------------------------------------
void Aren::jitter(float steps, int T, int h)
{

  //-- Both feet are 180 degrees out of phase
  //-- Feet amplitude and offset are the same
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  //-- h is constrained to avoid hit the feets
  h = min(25, h);
  int A[4] = {h, h, 0, 0};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {DEG2RAD(-90), DEG2RAD(90), 0, 0};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Aren motion: Ascending & turn (Jitter while up&down)
//--  Parameters:
//--    steps: Number of bends
//--    T: Period of one bend
//--    h: height (Values between 5 - 15)
//---------------------------------------------------------
void Aren::ascendingTurn(float steps, int T, int h)
{

  //-- Both feet and legs are 180 degrees out of phase
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  //-- h is constrained to avoid hit the feets
  h = min(13, h);
  int A[4] = {h, h, h, h};
  int O[4] = {0, 0, h + 4, -h + 4};
  double phase_diff[4] = {DEG2RAD(-90), DEG2RAD(90), DEG2RAD(-90), DEG2RAD(90)};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Aren motion: Wave
//--  Parameters:
//--    Steps: Number of steps
//--    T: Period
//--    h: Height. Typical values between 15 and 40
//--    dir: Direction: LEFT / RIGHT
//---------------------------------------------------------
void Aren::wave(float steps, int T, int h, int dir)
{

  //-- This motion is similar to that of the caterpillar robots: A travelling
  //-- wave moving from one side to another
  //-- The two Aren's feet are equivalent to a minimal configuration. It is known
  //-- that 2 servos can move like a worm if they are 120 degrees out of phase
  //-- In the example of Aren, the two feet are mirrored so that we have:
  //--    180 - 120 = 60 degrees. The actual phase difference given to the oscillators
  //--  is 60 degrees.
  //--  Both amplitudes are equal. The offset is half the amplitud plus a little bit of
  //-   offset so that the robot tiptoe lightly

  int A[4] = {0, 0, h, h};
  int O[4] = {0, 0, h / 2 + 2, -h / 2 - 2};
  int phi = -dir * 90;
  double phase_diff[4] = {0, 0, DEG2RAD(phi), DEG2RAD(-60 * dir + phi)};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//----------------------------------------------------------
//-- Aren motion: Tiptoe Wave. A mixture between wave and tiptoe swing
//--   Parameters:
//--     steps: Number of steps
//--     T: Period
//--     h: height (Values between 20 - 50)
//--     dir:  Direction: LEFT / RIGHT
//-----------------------------------------------------------
void Aren::tiptoeWave(float steps, int T, int h, int dir)
{

  int A[4] = {25, 25, h, h};
  int O[4] = {0, 0, h / 2 + 4, -h / 2 - 4};
  double phase_diff[4] = {90, 90, DEG2RAD(0), DEG2RAD(-60 * dir)};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Aren motion: Flapping
//--  Parameters:
//--    steps: Number of steps
//--    T: Period
//--    h: height (Values between 10 - 30)
//--    dir: direction: FORWARD, BACKWARD
//---------------------------------------------------------
void Aren::flapping(float steps, int T, int h, int dir)
{

  int A[4] = {12, 12, h, h};
  int O[4] = {0, 0, h - 10, -h + 10};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(-90 * dir), DEG2RAD(90 * dir)};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//--  Aren motion: Shuffle
//--  Parameters:
//--    steps: Number of steps
//--    T: Time Period
//--    dir: Direction (LEFT or RIGHT)
//---------------------------------------------------------
void Aren::shuffle(float steps, int T, int dir)
{
  int A[4] = {15, 15, 30, 30};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90 * dir), DEG2RAD(-90 * dir)};

  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//--  Aren motion: Kick
//--  Parameters:
//--    steps: Number of steps
//--    T: Time Period
//--    dir: Direction (LEFT or RIGHT)
//---------------------------------------------------------
void Aren::kick(float steps, int T, int dir)
{
  // Default: Left bend
  int bend1[4] = {90, 90, 180, 100};
  int bend2[4] = {30, 90, 90, 100};
  int bend3[4] = {30, 90, 40, 100};
  int home[4] = {90, 90, 90, 90};

  //Changes in the parameters if right direction is chosen
  if (dir == RIGHT)
  {
    int newBend1[4] = {90, 90, 80, 0};
    int newBend2[4] = {90, 150, 80, 90};
    int newBend3[4] = {90, 150, 80, 140};
    memcpy(bend1, newBend1, sizeof(newBend1));
    memcpy(bend2, newBend2, sizeof(newBend2));
    memcpy(bend3, newBend3, sizeof(newBend3));
  }

  for (int i = 0; i < steps; i++)
  {
    _moveServos(300, bend1);
    _moveServos(75, bend2);
    _moveServos(75, bend3);
    delay(500);
    _moveServos(500, home);
  }
}

//---------------------------------------------------------
//--  Aren motion: Diagonal Lean Right
//--  Parameters:
//--    steps: Number of steps
//--    T: Time Period
//---------------------------------------------------------
void Aren::diagonalLeanBackRight(float steps, int T)
{
  // Default: Lean towards right
  int A[4] = {10, 10, 15, 15};
  int O[4] = {40, 10, 15, 15};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(0), DEG2RAD(0)};

  _execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//--  Aren motion: Tap
//--  Parameters:
//--    steps: Number of steps
//--    T: Time Period
//--    dir: Direction (LEFT or RIGHT)
//---------------------------------------------------------
void Aren::tap(float steps, int T, int dir)
{
  // Default: Tap right foot
  int A[4] = {90, 90, 90, 90};
  int O[4] = {90, 90, 100, 130};

  if (dir == LEFT)
  {
    int a[4] = {90, 90, 90, 90};
    int o[4] = {90, 90, 50, 100};
    memcpy(A, a, sizeof(a));
    memcpy(O, o, sizeof(o));
  }

  for (int i = 0; i < steps; i++)
  {
    _moveServos(T * 0.6, A);
    _moveServos(T * 0.4, O);
    delay(T * 0.2);
  }
}

//---------------------------------------------------------
//--  Aren motion: Foot Wave
//--  Parameters:
//--    steps: Number of steps
//--    T: Time Period
//--    dir: Direction (LEFT or RIGHT)
//---------------------------------------------------------
void Aren::footWave(float steps, int T, int dir)
{
  // Default: Right foot
  int m1[4] = {50, 60, 90, 90};
  int m2[4] = {80, 60, 90, 90};
  int m3[4] = {40, 60, 90, 90};
  int m4[4] = {80, 60, 90, 90};
  int m5[4] = {40, 60, 90, 90};
  int m6[4] = {80, 60, 90, 90};

  if (dir == LEFT)
  {
    int newm1[4] = {120, 130, 90, 90};
    int newm2[4] = {120, 100, 90, 90};
    int newm3[4] = {120, 140, 90, 90};
    int newm4[4] = {120, 80, 90, 90};
    int newm5[4] = {120, 140, 90, 90};
    int newm6[4] = {120, 100, 90, 90};
    memcpy(m1, newm1, sizeof(newm1));
    memcpy(m2, newm2, sizeof(newm2));
    memcpy(m3, newm3, sizeof(newm3));
    memcpy(m4, newm4, sizeof(newm4));
    memcpy(m5, newm5, sizeof(newm5));
    memcpy(m6, newm6, sizeof(newm6));
  }

  for (int i = 0; i < steps; i++)
  {
    // 100, 200, 100, 200, 100, 200
    // 100, 200, 100, 200, 100, 200
    // Total: 1300
    // 100 = 0.077
    // 200 = 0.154
    _moveServos(T * 0.077, m1);
    delay(T * 0.154);
    _moveServos(T * 0.077, m2);
    delay(T * 0.154);
    _moveServos(T * 0.077, m3);
    delay(T * 0.154);
    _moveServos(T * 0.077, m4);
    delay(T * 0.154);
    _moveServos(T * 0.077, m5);
    delay(T * 0.154);
    _moveServos(T * 0.077, m6);
    delay(T * 0.154);
  }
}
