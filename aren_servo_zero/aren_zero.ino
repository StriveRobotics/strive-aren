#include <Aren.h>

Aren Aren;

#define PIN_YL 2 //servo[2]
#define PIN_YR 3 //servo[3]
#define PIN_RL 4 //servo[4]
#define PIN_RR 5 //servo[5]

void setup() {
  Serial.begin(19200);
  Aren.init(PIN_YL,PIN_YR,PIN_RL,PIN_RR,true,PIN_NoiseSensor, 13, 8, 9);
  Aren.home();
}

void loop() {
  Aren.home();
  delay(1000);
  Aren.setRestState(false);
}