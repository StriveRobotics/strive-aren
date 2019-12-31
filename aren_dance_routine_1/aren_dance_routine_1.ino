/*
 * DANCE ROUTINE 1
 * Song: Seven Nation Army - Glitch Mob Remix
 * 
 * NOTES
 * Progress
 * - Currently checking through global top 50 (spotify), trending (YouTube), Billboard Hot 100, and trending (genius).
 * 
 * Songs
 * - Seven Nation Army - Remix by Glitch Mob
 * - Memories - Maroon 5 (Late 2019)
 * - Old Town Road - Remix by Lil Nas X (Early 2019)
 * - Billie Jean - Michael Jackson
 * 
 */



#include <Aren.h>

Aren Aren;

#define PIN_YL 2 //servo[2]
#define PIN_YR 3 //servo[3]
#define PIN_RL 4 //servo[4]
#define PIN_RR 5 //servo[5]

int loopCount; // For debugging

void setup() {
  Serial.begin(19200);
  // Yl, YR, Rl, RR, load-calibration, noise sensor, buzzer, ultrasonic trigger, ultrasonic echo
  Aren.init(PIN_YL,PIN_YR,PIN_RL,PIN_RR,true,PIN_NoiseSensor, 13, 8, 9);
  Aren.home();

  pinMode(PIN_Echo, INPUT);
  pinMode(PIN_Trigger, OUTPUT);
}

void loop() {
  // DEBUG LOOP
  // while (loopCount != 1) {


    Aren.sing(S_mode3); //00:00
    delay(500);

    Aren.tap(8, 400, LEFT);
    Aren.tap(8, 400, RIGHT);
    Aren.tap(8, 400, LEFT);
    Aren.tap(9, 400, RIGHT); //00:16

    for (size_t i = 0; i < 8; i++) Aren.jump(500); //00:23
    for (size_t j = 0; j < 16; j++) Aren.jump(250); //00:32

    Aren.flapping(8, 1000, 20, BACKWARD); //00:40
    Aren.flapping(7, 1000, 20, FORWARD); //00:48
    delay(1000);
    Aren.swing(8, 1000, 20); //00:56
    Aren.tiptoeSwing(8, 1000, 30); //01:04
    
    Aren.tiptoeWave(2, 1000, 30, LEFT);
    Aren.tiptoeWave(2, 1000, 30, RIGHT); //01:08
    Aren.walk(4, 1000, FORWARD);
    Aren.walk(3, 1000, BACKWARD); //1:14
    delay(300);
    Aren.jump(300); //01:15
    Aren.walk(4, 1000, FORWARD);
    Aren.walk(4, 1000, BACKWARD); //01:21
    Aren.diagonalLeanBackRight(3, 800);
    Aren.diagonalLeanBackRight(1, 2000); //01:29
    
    Aren.tap(7, 400, LEFT);
    Aren.tap(8, 400, RIGHT); //01:32
    Aren.tap(8, 400, LEFT); //01:39
    Aren.tap(9, 400, RIGHT); //01:44
    
    Aren.tiptoeSwing(10, 500, 30);
    Aren.kick(1, 900, LEFT); //01:49
    delay(100);
    Aren.kick(1, 900, RIGHT);
    delay(100);
    Aren.tiptoeSwing(8, 500, 30); //01:56
    Aren.updown(3, 1000, 30); //02:01
    
    Aren.walk(4, 1000, FORWARD); //2:04
    Aren.walk(4, 1000, BACKWARD); //02:08
    Aren.updown(1, 1000, 30); //02:09
    Aren.walk(2, 1000, FORWARD);
    Aren.walk(2, 1000, FORWARD);
    Aren.tiptoeWave(2, 1000, 30, LEFT);
    Aren.tiptoeWave(2, 1000, 30, RIGHT);
    
    Aren.updown(4, 1000, 30); //02:20

    Aren.shuffle(6, 1000, LEFT);
    Aren.footWave(1, 1000, LEFT);
    Aren.footWave(1, 1000, RIGHT);
    Aren.shuffle(7, 1000, RIGHT); //02:36

    Aren.tiptoeWave(4, 1000, 30, LEFT);
    Aren.jump(500);
    Aren.jump(500);
    Aren.tiptoeWave(2, 1000, 30, RIGHT);
    Aren.jump(300); //02:44
    Aren.tiptoeWave(2, 1000, 30, LEFT);
    Aren.kick(1, 600, RIGHT);
    Aren.jitter(2, 350, 30);
    Aren.kick(1, 600, LEFT);
    Aren.jitter(2, 350, 30);

    Aren.jump(1000); //02:53
    Aren.jump(400);
    Aren.diagonalLeanBackRight(1, 1000);


// DEBUG LOOP CONTINUED
    // Until 2:56
    // delay(5000);
    // Aren.sing(S_disconnection);
    // delay(5000);
    // loopCount++;
  //}

// DEBUG for reseting to home every 4s (Used in servo zeroing when assembling)
  // Aren.home();
  // delay(4000);
  // Aren.setRestState(false);

}