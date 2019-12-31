/*
 * AREN Bluetooth Test 3
 * Continues running until "stop" is received from the phone
 * Strive Robotics Dec 2019 GPL license
 */

#include <Aren.h>
#include <SoftwareSerial.h>

/* Pin number constants */
#define PIN_Buzzer 13
#define PIN_Trigger 8
#define PIN_Echo 9
#define PIN_NoiseSensor A6
#define PIN_YL 2 //servo[2]
#define PIN_YR 3 //servo[3]
#define PIN_RL 4 //servo[4]
#define PIN_RR 5 //servo[5]

/* Aren & BtSerial object */
Aren Aren;
SoftwareSerial BtSerial(0, 1); // RX, TX

/* Global Variables */
const uint8_t bufferLimit = 32;
char receivedData[bufferLimit];
char startMarker = '<';
char endMarker = '>';

boolean newDataFlag = false;
boolean rxInProgressFlag = false;

char ACTIONTYPE[8] = {'z'};
uint16_t SPEED = 1000;
uint16_t CYCLES = 1;
int8_t DIRECTION = 1;
uint8_t HEIGHT = 15;

/* Setup */
void setup()
{
  Serial.begin(9600);
  BtSerial.begin(9600);

  // Yl, YR, Rl, RR, load-calibration, noise sensor, buzzer, ultrasonic trigger, ultrasonic echo
  Aren.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, PIN_NoiseSensor, PIN_Buzzer, PIN_Trigger, PIN_Echo);
  Aren.home();
  Aren.sing(S_mode3);
  Serial.println("AREN is ready");
}

/* Main Loop */
void loop()
{
  receiveSerialData();
  handleSerialData();
}

/* Functions */

void receiveSerialData()
{
  char dataIn;
  static uint8_t index = 0;

  while (BtSerial.available() && newDataFlag == false)
  {
    dataIn = BtSerial.read();
    Serial.println("BtSerial data in: " + (String)dataIn);

    if (rxInProgressFlag == true)
    {
      if (dataIn != endMarker)
      {
        if (index >= bufferLimit)
        {
          index -= 1;
          Serial.println("Buffer overflow detected!");
        }
        receivedData[index] = dataIn;
        index++;
      }
      else
      {
        rxInProgressFlag = false;
        newDataFlag = true;
        receivedData[index] = "\0";
        index = 0;
        Serial.println("Data reading completed. Full read out: ");
        Serial.print(receivedData);
        Serial.println(" ");
      }
    }

    if (dataIn == startMarker)
    {
      rxInProgressFlag = true;
    }
  }
  // delay(50);
}

void handleSerialData()
{
  if (newDataFlag == true)
  {
    parseData();
    newDataFlag = false;
  }
  execute(ACTIONTYPE[0], SPEED, 1, DIRECTION, HEIGHT);
}

void parseData()
{
  char *strtokIndex; // this is used by strtok() as an index
  char directionInput[8] = {0};

  strtokIndex = strtok(receivedData, ","); // Get first part until first comma
  strcpy(ACTIONTYPE, strtokIndex);         // Copy the part until first comma to actionType
  Serial.println("Action type: " + (String)ACTIONTYPE);
  if (ACTIONTYPE[0] == 'z')
  {
    Serial.println("HOME CONDITION CALLED");
    Aren.home();
    delay(100);
    Aren.setRestState(false);
  }

  strtokIndex = strtok(NULL, ","); // this continues where the previous call left off
  SPEED = atol(strtokIndex);       // convert this next part to an integer and store to speed
  Serial.println("Speed: " + (String)SPEED);

  strtokIndex = strtok(NULL, ",");
  strcpy(directionInput, strtokIndex);
  Serial.println("Direction: " + (String)directionInput);
  if (directionInput[0] != ' ')
  {
    switch (directionInput[0])
    {
    case 'F':
      DIRECTION = FORWARD;
      break;
    case 'B':
      DIRECTION = BACKWARD;
      break;
    case 'L':
      DIRECTION = LEFT;
      break;
    case 'R':
      DIRECTION = RIGHT;
      break;
    default:
      Serial.println("Unexpected error, unknown direction input!");
      break;
    }
  }

  strtokIndex = strtok(NULL, ",");
  HEIGHT = atol(strtokIndex);
  Serial.println("Height: " + (String)HEIGHT);
}

void execute(char actionType, uint16_t speed, uint16_t cycles, int8_t direction, uint8_t height)
{
  if (actionType == 'z')
  {
    return;
  }

  Serial.println("Execute data parameter actionType: " + (String)actionType);
  Serial.println("Execute data parameter speed: " + (String)speed);
  Serial.println("Execute data parameter cycle: " + (String)cycles);
  Serial.println("Execute data parameter direction: " + (String)direction);
  Serial.println("Execute data parameter height: " + (String)height);

  switch (actionType)
  {
  case '0':
    // Move Forward/Back
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.walk(cycles, speed, direction);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case '1':
    // Turn Left/Right
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.turn(cycles, speed, direction);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case '2':
    // Jump
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.jump(speed);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case '3':
    // Bend
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.bend(cycles, speed, direction);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case '4':
    // Shake Leg
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.shakeLeg(cycles, speed, direction);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case '5':
    // Up & Down
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.updown(cycles, speed, height);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case '6':
    // Swing
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.swing(cycles, speed, height);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case '7':
    // Tip-toe Swing
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.tiptoeSwing(cycles, speed, height);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case '8':
    // Jitter
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.jitter(cycles, speed, height);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case '9':
    // Ascending Turn
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.ascendingTurn(cycles, speed, height);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case 'a':
    // Wave
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.wave(cycles, speed, height, direction);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case 'b':
    // Tip-toe Wave
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.tiptoeWave(cycles, speed, height, direction);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case 'c':
    // Flapping
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.flapping(cycles, speed, height, direction);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case 'd':
    // Shuffle
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.shuffle(cycles, speed, direction);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case 'e':
    // Kick
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.kick(cycles, speed, direction);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case 'f':
    // Diagonal Lean Back Right
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.diagonalLeanBackRight(cycles, speed);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case 'g':
    // Tap
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.tap(cycles, speed, direction);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  case 'h':
    // Foot Wave
    Serial.println("Executing move movement of type: " + (String)actionType);
    Aren.footWave(cycles, speed, direction);
    Serial.println("Finished executing movement of type: " + (String)actionType);
    break;

  default:
    Serial.println("Unexpected error, cannot execute: Direction not recognised!");
    break;
  }
}