/*
 * -- AREN Bluetooth Connection & Control Program
 * 
 * -- ABOUT
 *    For controlling AREN via Bluetooth through the basic mobile application provided
 * 
 */

#include <Aren.h>
#include <SoftwareSerial.h>

// -- Pin number constants specification
#define PIN_LL 2      // Left leg
#define PIN_RL 3      // Right leg
#define PIN_LF 4      // Left foot
#define PIN_RF 5      // Right foot
#define PIN_Buzzer 13 // Buzzer
#define PIN_Trigger 8 // Ultrasound Trigger
#define PIN_Echo 9    // Ultrasound Echo

// -- AREN and Bluetooth Serial Object Creation
Aren Aren;
SoftwareSerial BtSerial(0, 1); // BtSerial(RX: Receive Pin, TX: Transmit Pin)

// -- Global Variables Declaration
const uint8_t bufferLimit = 32; // Declaring the maximum number of characters the buffer will hold
char receivedData[bufferLimit]; // Where the transmitted data will be stored (Has a max size of 32 characters)
char startMarker = '<';         // Marker to denote start of data transmission from application
char endMarker = '>';           // Marker to denote end of data transmission from application

boolean newDataFlag = false;      // Flag to signal to the program that there is a new command to be executed
boolean rxInProgressFlag = false; // Flag to signal to the program loop that data is being transmitted from the phone and written

char ACTIONTYPE[8] = {'z'}; // To store the type of action for execution. Has default value of character 'z' which represents "Stop, nothing to execute"
uint16_t SPEED = 1000;      // To store the speed of action. Has a default value of 1000
uint16_t CYCLES = 1;        // To store the number of cycles for a action. Has a default value of 1
int8_t DIRECTION = 1;       // To store the direction parameter. Has a default value of 1
uint8_t HEIGHT = 15;        // To store the height parameter. Has a default value of 15 degrees

// -- Program Setup
void setup()
{
  // Open the respective Serial ports at stated data rate
  Serial.begin(9600);
  BtSerial.begin(9600);

  // Initialise the AREN
  Aren.init(PIN_LL, PIN_RL, PIN_LF, PIN_RF, true, PIN_Buzzer, PIN_Trigger, PIN_Echo);
  Aren.home();
  Aren.sing(S_mode3);
  Serial.println("AREN is ready");
}

// -- Main Program Loop
void loop()
{
  receiveSerialData();
  handleSerialData();
}

// -- Functions

/*
  FUNCTION
  void receiveSerialData()
 
  For handling the receiving of serial data from the bluetooth module.
  Looks for the start marker '<' and will start the saving of the data following the marker into receivedData array.
  Stops saving to the receivedData array once it receives the end marker '>'.
  Will also change the newDataFlag to true to get the program to handle the data.
*/
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
}

/*
  FUNCTION
  void handleSerialData()
 
  If there is new data, which is flagged by the newDataFlag, this will call the parseData() function to parse the data. After parsing the data, it will set the newDataFlag back to false to say that it has finished parsing.
  This function then calls the execute function to excute based on the parsed data. By default, the action type is 'z' which represents "Stop, do nothing".
*/
void handleSerialData()
{
  if (newDataFlag == true)
  {
    parseData();
    newDataFlag = false;
  }
  execute(ACTIONTYPE[0], SPEED, CYCLES, DIRECTION, HEIGHT);
}

/*
  FUNCTION
  void parseData()
 
  Will parse the four parameters transmitted by the application to the Bluetooth module and save it the corresponding global variables
  They are the "Action Type", "Duration/Speed", "Direction of Movement" (if applicable), and "Height of Movement" (if applicable)
  
  Note that the method atol() for converting a C-type string to a long integer should return 0 should there be no valid conversion
*/
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
      Serial.println("Unexpected error: The direction input specified is not valid!");
      break;
    }
  }

  strtokIndex = strtok(NULL, ",");
  HEIGHT = atol(strtokIndex);
  Serial.println("Height: " + (String)HEIGHT);
}

/*
  FUNCTION
  void execute(5 parameters)

  Will execute a valid action based on the received data.

  Parameters (Name/Type):
    actionType  character
    speed       unsigned 16 bit integer
    cycles      unsigned 16 bit integer
    direction   signed 8 bit integer
    height      unsigned 8 bit integer
*/
void execute(char actionType, uint16_t speed, uint16_t cycles, int8_t direction, uint8_t height)
{
  if (actionType == 'z')
  {
    // 'z' means no action to be taken
    // Therefore, don't do anything and jump out of the function
    return;
  }

  // This is for debugging and troubleshooting
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
    Serial.println("Unexpected error: Action Type is not valid!");
    break;
  }
}