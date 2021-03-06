#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif

#include <US.h>

void US::init(int pinTrigger, int pinEcho)
{
  _pinTrigger = pinTrigger;
  _pinEcho = pinEcho;
  pinMode(_pinTrigger, OUTPUT);
  pinMode(_pinEcho, INPUT);
}

long US::TP_init()
{
  digitalWrite(_pinTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(_pinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(_pinTrigger, LOW);
  long microseconds = pulseIn(_pinEcho, HIGH, 40000); //40000
  return microseconds;
}

float US::read() {
  long microseconds = US::TP_init();
  long distance = microseconds/29/2;
  if (distance == 0){
    distance = 999;
  }
  return (float)distance;
}