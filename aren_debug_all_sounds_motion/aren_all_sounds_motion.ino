#include <Aren.h>

Aren Aren;

#define PIN_YL 2 //servo[2]
#define PIN_YR 3 //servo[3]
#define PIN_RL 4 //servo[4]
#define PIN_RR 5 //servo[5]

// For debugging
unsigned short int loopCount = 0;
unsigned short int endLoopCount = 2;
unsigned short int cycleType = 1; // SOUND = 0, MOTION = 1, BOTH = 2, Home() = any other num

void cycleAllSounds() {
    Serial.println("Now cycling through all sounds...");
    Serial.println("Now playing sound: Connection");
    Aren.sing(S_connection);
    delay(2000);
    Serial.println("Now playing sound: Disconnection");
    Aren.sing(S_disconnection);
    delay(2000);
    Serial.println("Now playing sound: Button Pushed");
    Aren.sing(S_buttonPushed);
    delay(2000);
    Serial.println("Now playing sound: Mode 1");
    Aren.sing(S_mode1);
    delay(2000);
    Serial.println("Now playing sound: Mode 2");
    Aren.sing(S_mode2);
    delay(2000);
    Serial.println("Now playing sound: Mode 3");
    Aren.sing(S_mode3);
    delay(2000);
    Serial.println("Now playing sound: Surprise");
    Aren.sing(S_surprise);
    delay(2000);
    Serial.println("Now playing sound: OhOoh");
    Aren.sing(S_OhOoh);
    delay(2000);
    Serial.println("Now playing sound: OhOoh2");
    Aren.sing(S_OhOoh2);
    delay(2000);
    Serial.println("Now playing sound: Cuddly");
    Aren.sing(S_cuddly);
    delay(2000);
    Serial.println("Now playing sound: Sleeping");
    Aren.sing(S_sleeping);
    delay(2000);
    Serial.println("Now playing sound: Happy");
    Aren.sing(S_happy);
    delay(2000);
    Serial.println("Now playing sound: Super Happy");
    Aren.sing(S_superHappy);
    delay(2000);
    Serial.println("Now playing sound: Happy Short");
    Aren.sing(S_happy_short);
    delay(2000);
    Serial.println("Now playing sound: Sad");
    Aren.sing(S_sad);
    delay(2000);
    Serial.println("Now playing sound: Confused");
    Aren.sing(S_confused);
    delay(2000);
    Serial.println("All sounds played!");

    Serial.println("Pausing for 4 seconds...");
    delay(4000);
}

void cycleAllMovements() {
  Serial.println("Now cycling through all movements...");

  // EXPERIMENTAL - IN-DEV
  Serial.println("Current movement: Tap Left");
  Aren.tap(5, 1000, LEFT);
  delay(2000);
  Serial.println("Current movement: Tap Right");
  Aren.tap(5, 1000, RIGHT);
  delay(2000);

  Serial.println("Current movement: Foot wave left");
  Aren.footWave(5, 1000, LEFT);
  delay(2000);
  Serial.println("Current movement: Foot wave right");
  Aren.footWave(5, 1000, RIGHT);
  delay(2000);

  // EXPERIMENTAL - TESTED
  Serial.println("Current movement: Lean Back Right");
  Aren.diagonalLeanBackRight(10, 1000);
  delay(2000);
  Aren.diagonalLeanBackRight(10, 1000);
  delay(2000);
  
  Serial.println("Current movement: Kick Right");
  Aren.kick(3, 1000, RIGHT);
  delay(2000);
  Serial.println("Current movement: Kick Right");
  Aren.kick(3, 1000, LEFT);
  delay(2000);

  Serial.println("Current movement: Shuffle Left");
  Aren.shuffle(5, 1000, LEFT);
  delay(2000);
  Serial.println("Current movement: Shuffle Right");
  Aren.shuffle(5, 1000, RIGHT);
  delay(2000);

  // FINAL
  Serial.println("Current movement: Walk forward");
  Aren.walk(4, 1000, FORWARD);
  delay(2000);
  Serial.println("Current movement: Walk backward");
  Aren.walk(4, 1000, BACKWARD);
  delay(2000);
  Serial.println("Current movement: Jump");
  Aren.jump(1000);
  delay(2000);
  Serial.println("Current movement: Turn Left");
  Aren.turn(4, 1000, LEFT);
  delay(2000);
  Serial.println("Current movement: Turn Right");
  Aren.turn(4, 1000, RIGHT);
  delay(2000);
  Serial.println("Current movement: Walk Back");
  Aren.walk(8, 1000, BACKWARD);
  delay(2000);

  Serial.println("Current movement: Bend Left");
  Aren.bend(4, 1000, LEFT);
  delay(2000);
  Serial.println("Current movement: Bend Right");
  Aren.bend(4, 1000, RIGHT);
  delay(2000);
  Serial.println("Current movement: Shake Left Leg");
  Aren.shakeLeg(4, 1000, LEFT);
  delay(2000);
  Serial.println("Current movement: Shake Right Leg");
  Aren.shakeLeg(4, 1000, RIGHT);
  delay(2000);

  Serial.println("Current movement: Up & Down");
  Aren.updown(4, 1000, LARGE);
  delay(2000);
  Serial.println("Current movement: Swing");
  Aren.swing(4, 1000, 40);
  delay(2000);
  Serial.println("Current movement: Tip Toe Swing");
  Aren.tiptoeSwing(4, 1000, 40);
  delay(2000);
  Serial.println("Current movement: Jitter");
  Aren.jitter(4, 1000, 15);
  delay(2000);
  Serial.println("Current movement: Ascending Turn");
  Aren.ascendingTurn(4, 1000, 10);
  delay(2000);
  Serial.println("Current movement: Wave Left");
  Aren.wave(4, 1000, 30, LEFT);
  delay(2000);
  Serial.println("Current movement: Wave Right");
  Aren.wave(4, 1000, 30, RIGHT);
  delay(2000);
  Serial.println("Current movement: Tiptoe Wave Left");
  Aren.tiptoeWave(4, 1000, 35, LEFT);
  delay(2000);
  Serial.println("Current movement: Tiptoe Wave Right");
  Aren.tiptoeWave(4, 1000, 35, RIGHT);
  delay(2000);
  Serial.println("Current movement: Flapping Forward");
  Aren.flapping(4, 1000, 20, FORWARD);
  delay(2000);
  Serial.println("Current movement: Flapping Backward");
  Aren.flapping(4, 1000, 20, BACKWARD);
  Serial.println("All movements cycled through!");

  Serial.println("Pausing for 4 seconds...");
  delay(4000);
}

void setup() {
  Serial.begin(19200);
  Aren.init(PIN_YL,PIN_YR,PIN_RL,PIN_RR,true,PIN_NoiseSensor, 13, 8, 9);
  // Yl, YR, Rl, RR, load-calibration, noise sensor, buzzer, ultrasonic trigger, ultrasonic echo
  Aren.home();

  pinMode(PIN_Echo, INPUT);
  pinMode(PIN_Trigger, OUTPUT);
}

void loop() {

  while (loopCount != endLoopCount) {
    
    switch (cycleType)
    {
    case 0:
      cycleAllSounds();
      break;

    case 1:
      cycleAllMovements();
      break;
    
    case 2:
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