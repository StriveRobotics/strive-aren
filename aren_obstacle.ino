#include <Aren.h>

Aren Aren;
US us;

#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot


//-- Global Variables -------------------------------------------//
int distance; // variable to store distance read from ultrasonic range finder module
bool obstacleDetected = false; // logic state for when object detected is at the distance we set

//-- Setup ------------------------------------------------------//
void setup() {
  Aren.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, PIN_NoiseSensor, 13, 8, 9);
  Aren.sing(S_connection);
  Aren.home();
  delay(500);

  us.init(8, 9);

  Serial.begin(19200);

  pinMode(PIN_Echo, INPUT);
  pinMode(PIN_Trigger, OUTPUT);
}

//-- Principal Loop ---------------------------------------------//
void loop() {

  int val = us.read();

  Serial.println(val);

  if (obstacleDetected) {
    Aren.sing(S_surprise); // sound a surprise
    Aren.jump(500);
    Aren.sing(S_cuddly);

    for (int i = 0; i < 3; i++) Aren.walk(1, 1300, BACKWARD);

    delay(500);// small 1/2 second delay

    for (int i = 0; i < 3; i++) { //repeat three times 
        Aren.turn(1, 1000, 1); // the walk left command
        delay(500);// small 1/2 second delay to allow Otto to settle
    }
  } else { // if nothing in front then walk forward
    Aren.walk(1, 1000, FORWARD); //Aren walk straight
    obstacleDetector(); // call the function to check the ultrasonic range finder for an object closer than 15cm
  }
}  

//-- Function to read distance sensor & to actualize obstacleDetected variable
void obstacleDetector() {
  int distance = Aren.getDistance(); // get the distance from the ultrasonic range finder
  Serial.println(distance);
  if (distance < 15) obstacleDetected = true; // check to see if this distance is closer than 15cm, true if it is
  else obstacleDetected = false;// false if it is not
}