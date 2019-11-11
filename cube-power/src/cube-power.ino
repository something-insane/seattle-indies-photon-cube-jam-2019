#include "neopixel.h"
#include "Adafruit_MPR121.h"


#define BEEPER_PIN A5
#define BUZZER_PIN A4

SYSTEM_MODE(MANUAL);

#define CHARGING_PIN D7
#define CHARGED_PIN D6

int notes[] = 
{0,
/* C,  C#,   D,  D#,   E,   F,  F#,   G,  G#,   A,  A#,   B */
3817,3597,3401,3205,3030,2857,2703,2551,2404,2273,2146,2024,   // 3 (1-12)
1908,1805,1701,1608,1515,1433,1351,1276,1205,1136,1073,1012,   // 4 (13-24)
 956, 903, 852, 804, 759, 716, 676, 638, 602, 568, 536, 506,   // 5 (25-37)
 478, 451, 426, 402, 379, 358, 338, 319, 301, 284, 268, 253,   // 6 (38-50)
 239, 226, 213, 201, 190, 179, 169, 159, 151, 142, 134, 127 }; // 7 (51-62)
// setup() runs once, when the device is first turned on.

#define NOTE_FSHARP3 2703
#define NOTE_FSHARP4 1351
#define NOTE_C5 956
#define NOTE_CSHARP5 903
#define NOTE_D5  852
#define NOTE_DSHARP5 804
#define NOTE_E5 759
#define NOTE_F5 716
#define NOTE_FSHARP5 676
#define NOTE_G5  638
#define NOTE_GSHARP5 602
#define NOTE_A5 568
#define NOTE_ASHARP5 536
#define NOTE_B5 506
#define NOTE_C6 478
#define NOTE_G3  2551
#define NOTE_G4  1276
#define NOTE_C5  956
#define RELEASE  20
#define BPM      100

int correctMelody[] = {NOTE_C5,NOTE_E5,NOTE_G5,NOTE_E5,NOTE_C5,NOTE_F5,NOTE_A5,NOTE_F5,NOTE_C5};
int incorrectMelody[] = {NOTE_FSHARP3,NOTE_FSHARP4,NOTE_FSHARP5,0,NOTE_FSHARP3,NOTE_FSHARP4,NOTE_FSHARP5,0,};
int letGoMelody[] = {NOTE_E5,NOTE_G5,NOTE_D5,NOTE_E5,NOTE_C5};
int pickMeUpMelody[] = {NOTE_G5,NOTE_C5,NOTE_B5,NOTE_A5,NOTE_G5,NOTE_C6,NOTE_C5};

int correctMelodyDurations[] = {4,4,4,4,4,4,4,4,2};
int incorrectMelodyDurations[] = {4,4,2,4,4,4,2,4};
int letMeGoMelodyDurations[] = {2,2,2,2,1};
int pickMeUpMelodyDurations[] = {2,2,4,4,2,1,2};

#define PIXEL_PIN_A D2
#define PIXEL_COUNT_A 24
#define PIXEL_TYPE_A WS2812B
#define BRIGHTNESS_A 255 // 0 - 255
Adafruit_NeoPixel stripA(PIXEL_COUNT_A, PIXEL_PIN_A, PIXEL_TYPE_A);

#define PIXEL_PIN_B D3
#define PIXEL_COUNT_B 24
#define PIXEL_TYPE_B WS2812B
#define BRIGHTNESS_B 255 // 0 - 255
Adafruit_NeoPixel stripB(PIXEL_COUNT_B, PIXEL_PIN_B, PIXEL_TYPE_B);

Adafruit_MPR121 cap = Adafruit_MPR121();

uint16_t lastPixelA = 0;
uint16_t lastPixelB = 0;

uint16_t changeCounterA = 0;
uint16_t changeCounterB = 0;

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void touchSetup() {
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1)
    {
      #if defined(PARTICLE)
      Particle.process();
      #endif
    }

  }

  Serial.println("MPR121 found!");
}

// setup() runs once, when the device is first turned on.
void setup() {
  Serial.begin(9600);
  stripA.setBrightness(BRIGHTNESS_A);
  stripA.begin();
  stripA.show();

  stripB.setBrightness(BRIGHTNESS_B);
  stripB.begin();
  stripB.show();

  touchSetup();

  pinMode(CHARGING_PIN, INPUT_PULLDOWN);
  pinMode(CHARGED_PIN, INPUT_PULLDOWN);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BEEPER_PIN, OUTPUT);
  // analogWrite(BUZZER_PIN, 255);
  // delay(1000);
  // tone(BUZZER_PIN, 440, 0);
}

int redA = 0;
int greenA = 0;
int blueA = 0;
int redB = 0;
int greenB = 0;
int blueB = 0;

int redALast = 0;
int greenALast = 0;
int blueALast = 0;
int redBLast = 0;
int greenBLast = 0;
int blueBLast = 0;

int redTargetA = 255;
int greenTargetA = 255;
int blueTargetA = 255;

int redTargetB = 255;
int greenTargetB = 255;
int blueTargetB = 255;

bool wasDone = false;
bool waitingForRelease = false;

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // #if defined(PARTICLE)
  // Particle.process();
  // #endif
  checkPower();
  // tone(BEEPER_PIN, 440, 0);
  bool anyTouched = false;

  // the actual game
  currtouched = cap.touched();
  redA = greenA = blueA = 0;
  redB = greenB = blueB = 0;
  if (currtouched & _BV(0)) {
    redA += 255;
    anyTouched = true;
  }
  
  if (currtouched & _BV(1)) {
    greenA += 255;
    anyTouched = true;
  }

  if (currtouched & _BV(2)) {
    blueA += 255;
    anyTouched = true;
  }

  if (currtouched & _BV(3)) {
    redA -= 128;
    blueA -= 128;
    greenA -= 128;
    anyTouched = true;
  }

  if (currtouched & _BV(6)) {
    redB += 255;
    anyTouched = true;
  }
  
  if (currtouched & _BV(7)) {
    greenB += 255;
    anyTouched = true;
  }

  if (currtouched & _BV(8)) {
    blueB += 255;
    anyTouched = true;
  }

  if (currtouched & _BV(9)) {
    redB -= 128;
    blueB -= 128;
    greenB -= 128;
    anyTouched = true;
  }

  bool aChanged =
    redA != redALast
    || greenA != greenALast
    || blueA != blueALast;

  bool bChanged =
    redB != redBLast
    || greenB != greenBLast
    || blueB != blueBLast;

  if (aChanged) {
    changeCounterA = 0;
    // onA(stripA.Color(0, 0, 0));
  }

  if (bChanged){
    changeCounterB = 0;
    // onB(stripB.Color(0, 0, 0));
  }
  
  chaseAStepwiseSingle(stripA.Color(redA, greenA, blueA), false);

  chaseBStepwiseSingle(stripB.Color(redB, greenB, blueB), false);
  changeCounterA++;
  changeCounterB++;
  
  lasttouched = currtouched;

  redALast = redA;
  greenALast = greenA;
  blueALast = blueA;
  redBLast = redB;
  greenBLast = greenB;
  blueBLast = blueB;

  if (waitingForRelease) {
    if (!anyTouched
        && changeCounterA > PIXEL_COUNT_A
        && changeCounterB > PIXEL_COUNT_B) {
      waitingForRelease = false;
      doneReleased();
    } else {
      doneWaitingOnRelease();
      delay(25);
      return;
    }
  } else {
    handleDoneStates();
  }


  Serial.println("loop");
  // chaseBoth();

  delay(25);
}

static void handleDoneStates() {
  bool done
    = changeCounterA > PIXEL_COUNT_A+4
    && changeCounterB > PIXEL_COUNT_B+4
    && !(redA == 0 && greenA == 0 && blueA == 0)
    && redA == redB
    && greenA == greenB
    && blueA == blueB;

  if (done && !wasDone) {
    successJingle();
    waitingForRelease = true;
    wasDone = true;
    return;
  }
}

static void doneWaitingOnRelease() {
  // changeCounterA = 0;
  // changeCounterB = 0;
}

static void doneReleased() {
  handsOffJingle();
  wasDone = false;
  waitingForRelease = false;
  changeCounterA = 0;
  changeCounterB = 0;
}

static void chaseBoth() {
  chaseA(stripA.Color(255, 255, 255)); // Red

  chaseB(stripB.Color(255, 255, 255)); // Red
}

static void successJingle() {
  // onA(stripA.Color(0, 0, 0));
  // onB(stripB.Color(0, 0, 0));
  //tone(BEEPER_PIN, 261, 50);
  //delay(100);
  // onA(stripA.Color(255, 255, 255));
  // onB(stripB.Color(255, 255, 255));
  //tone(BEEPER_PIN, 440, 150);
  // onA(stripA.Color(0, 0, 0));
  // onB(stripB.Color(0, 0, 0));
  correctCombo();
  tone(BUZZER_PIN, 440, 150);
}

static void chargingJingle() {
 // tone(BEEPER_PIN, 261, 1000);
  letMeGo();
}

static void pickedUpJingle() {
//  tone(BEEPER_PIN, 220, 1000);
  pickMeUp();

}

static void handsOffJingle() {
  // tone(BEEPER_PIN, 261, 50);
  //   delay(100);
  // tone(BEEPER_PIN, 440, 50);
  //   delay(100);
  // tone(BEEPER_PIN, 440, 50);
  incorrectCombo();
  tone(BUZZER_PIN, 440, 150);

}


bool wasCharging = true;
// bool wasCharged = false;
static void checkPower() {
  bool isCharging = digitalRead(CHARGING_PIN);
  bool isCharged = digitalRead(CHARGED_PIN);

  // if (!wasCharging && isCharging) {
  //   wasCharged = false;
  // }

  if (isCharging) {
    if (!wasCharging) {
      chargingJingle();
    }

    if (isCharged) {
      Serial.println("charged");
      tone(BEEPER_PIN, 220, 100);
      delay(100);
      tone(BEEPER_PIN, 220, 100);
    } else {
      Serial.println("charging");
    }

    stripA.setBrightness(10);
    stripB.setBrightness(10);
    chaseBoth();
    System.sleep(SLEEP_MODE_DEEP, 3);
  }

  if (wasCharging && !isCharging) {
    Serial.println("unplugged");
    stripA.setBrightness(255);
    stripB.setBrightness(255);
    pickedUpJingle();
  }

  wasCharging = isCharging;
}

void correctCombo (){
 
 for (int thisNote = 0; thisNote < (sizeof(correctMelody)/sizeof(int)); thisNote++) {

  int noteDuration = 60*1000/BPM/correctMelodyDurations[thisNote];
    tone(BEEPER_PIN, (correctMelody[thisNote]!=0)?(500000/correctMelody[thisNote]):0,noteDuration-RELEASE);
    delay(noteDuration);
  }
  
  
}

void incorrectCombo () {

 for (int thisNote = 0; thisNote < (sizeof(incorrectMelody)/sizeof(int)); thisNote++) {

  int noteDuration = 60*1000/BPM/incorrectMelodyDurations[thisNote];
    tone(BEEPER_PIN, (incorrectMelody[thisNote]!=0)?(500000/incorrectMelody[thisNote]):0,noteDuration-RELEASE);
    delay(noteDuration);
  }
  

}

void letMeGo(){

 for (int thisNote = 0; thisNote < (sizeof(letGoMelody)/sizeof(int)); thisNote++) {

  int noteDuration = 60*1000/BPM/letMeGoMelodyDurations[thisNote];
    tone(BEEPER_PIN, (letGoMelody[thisNote]!=0)?(500000/letGoMelody[thisNote]):0,noteDuration-RELEASE);
    delay(noteDuration);
  }

}

void pickMeUp(){

 for (int thisNote = 0; thisNote < (sizeof(pickMeUpMelody)/sizeof(int)); thisNote++) {

  int noteDuration = 60*1000/BPM/pickMeUpMelodyDurations[thisNote];
    tone(BEEPER_PIN, (pickMeUpMelody[thisNote]!=0)?(500000/pickMeUpMelody[thisNote]):0,noteDuration-RELEASE);
    delay(noteDuration);
  }

}

// static void chaseA(uint32_t c) {
//   for(uint16_t i = 0; i < stripA.numPixels(); i++) {
//       stripA.setPixelColor(i  , c);
//   }
//   stripA.show();
//   delay(100);
// }

// static void chaseB(uint32_t c) {
//   for(uint16_t i = 0; i < stripB.numPixels() + 4; i++) {
//     stripB.setPixelColor(i  , c);
//   }
//   stripB.show();
//   delay(100);
// }
 
static void chaseA(uint32_t c) {
  for(uint16_t i = 0; i < stripA.numPixels() + 4; i++) {
      stripA.setPixelColor(i  , c); // Draw new pixel
      stripA.setPixelColor(i-4, 0); // Erase pixel a few steps back
      stripA.show();
      delay(25);
  }
}
 
static void chaseB(uint32_t c) {
  for(uint16_t i = 0; i < stripB.numPixels() + 4; i++) {
      stripB.setPixelColor(i  , c); // Draw new pixel
      stripB.setPixelColor(i-4, 0); // Erase pixel a few steps back
      stripB.show();
      delay(25);
  }
}
 
static void chaseAStepwiseSingle(uint32_t c, bool erase) {
  lastPixelA = (lastPixelA + 1) % (stripB.numPixels() + 4);
  stripA.setPixelColor(lastPixelA  , c); // Draw new pixel
  if (erase) stripA.setPixelColor(lastPixelA-4, 0); // Erase pixel a few steps back
  stripA.show();
}
 
static void chaseBStepwiseSingle(uint32_t c, bool erase) {
  lastPixelB = (lastPixelB + 1) % (stripB.numPixels() + 4);
  stripB.setPixelColor(lastPixelB  , c); // Draw new pixel
  if (erase) stripB.setPixelColor(lastPixelB-4, 0); // Erase pixel a few steps back
  stripB.show();
}
 
static void onA(uint32_t c) {
  for(uint16_t i = 0; i < stripA.numPixels(); i++) {
      stripA.setPixelColor(i  , c); // Draw new pixel
  }
  stripA.show();
}
 
static void onB(uint32_t c) {
  for(uint16_t i = 0; i < stripB.numPixels(); i++) {
      stripB.setPixelColor(i  , c); // Draw new pixel
  }
  stripB.show();
}