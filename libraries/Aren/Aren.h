/*
 * -- AREN Header File
 * -- Based on the open source project, Otto available at https://www.ottodiy.com
 * 
 */

#ifndef Aren_h
#define Aren_h

#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>
#include <US.h>
#include "Sounds.h"


//-- Constants
// Forward/Backward/Left/Right for use in specifying direction
#define FORWARD 1
#define BACKWARD -1
#define LEFT 1
#define RIGHT -1

class Aren {
  public:

    // Initialisation
    void init(int YL, int YR, int RL, int RR, bool load_calibration, int NoiseSensor, int Buzzer, int USTrigger, int USEcho);

    // Attach & detach functions
    void attachServos();
    void detachServos();

    // Oscillator Trims
    void setTrims(int YL, int YR, int RL, int RR);
    void saveTrimsOnEEPROM();

    // Motion Functions 
    void _moveServos(int time, int  servo_target[]);
    void oscillateServos(int A[4], int O[4], int T, double phase_diff[4], float cycle);

    // HOME = At rest position
    void home();
    bool getRestState();
    void setRestState(bool state);
    
    // Sensor functions
    float getDistance(); //US sensor
    int getNoise();      //Noise Sensor

    // Sounds
    void _tone(float noteFrequency, long noteDuration, int silentDuration);
    void bendTones(float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration);
    void sing(int musicName);

    // Predetermined movement sequences 
    void walk(float steps, int T, int dir);
    void walkDebug();
    void jump(int T);
    void turn(float steps, int T, int dir);
    void bend(int steps, int T, int dir);
    void shakeLeg(int steps, int T, int dir);

    void updown(float steps, int T, int h);
    void swing(float steps, int T, int h);
    void tiptoeSwing(float steps, int T, int h);
    void jitter(float steps, int T, int h);
    void ascendingTurn(float steps, int T, int h);

    void wave(float steps, int T, int h, int dir);
    void tiptoeWave(float steps, int T, int h, int dir);
    void flapping(float steps, int T, int h, int dir);

    void shuffle(float steps, int T, int dir);
    void kick(float steps, int T, int dir);
    void diagonalLeanBackRight(float steps, int T);
    void tap(float steps, int T, int dir);
    void footWave(float steps, int T, int dir);
 
  private:
    Oscillator servo[4];
    US us;

    int servo_pins[4];
    int servo_trim[4];
    int servo_position[4];

    int pinBuzzer;
    int pinNoiseSensor;
    
    unsigned long final_time;
    unsigned long partial_time;
    float increment[4];

    bool isResting;

    void _execute(int A[4], int O[4], int T, double phase_diff[4], float steps);
};

#endif