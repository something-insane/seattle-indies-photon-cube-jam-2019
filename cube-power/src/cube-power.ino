#include "neopixel.h"
#include "Adafruit_MPR121.h"


#define BEEPER_PIN TX
#define BUZZER_PIN WKP

SYSTEM_MODE(MANUAL);

#define CHARGING_PIN D7
#define CHARGED_PIN D6


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
  pinMode(BEEPER_PIN, OUTPUT);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // #if defined(PARTICLE)
  // Particle.process();
  // #endif
  checkPower();

  // the actual game
  currtouched = cap.touched();
  int redA = 0;
  int greenA = 0;
  int blueA = 0;
  if (currtouched & _BV(0)) {
    redA = 255;
  }
  
  if (currtouched & _BV(1)) {
    greenA = 255;
  }

  if (currtouched & _BV(2)) {
    blueA = 255;
  }

  if (currtouched & _BV(3)) {
    redA = greenA = blueA = 255;
  }

  int redB = 0;
  int greenB = 0;
  int blueB = 0;
  if (currtouched & _BV(6)) {
    redB = 255;
  }
  
  if (currtouched & _BV(7)) {
    greenB = 255;
  }

  if (currtouched & _BV(8)) {
    blueB = 255;
  }

  if (currtouched & _BV(9)) {
    redB = greenB = blueB = 255;
  }

  bool setA =
    (currtouched & 0b111111)
    == (lasttouched & 0b111111)
    || !(currtouched & 0b111111);

  if (setA) {
    onA(stripA.Color(redA, greenA, blueA));
  }

  bool setB =
    (currtouched & 0b111111000000)
    == (lasttouched & 0b111111000000)
    || !(currtouched & 0b111111000000);

  if (setB) {
    onB(stripB.Color(redB, greenB, blueB));
  }

  if (!setA) {
    chaseA(stripA.Color(redA, greenA, blueA));
  }

  if (!setB) {
    chaseB(stripB.Color(redB, greenB, blueB));
  }


  Serial.println("loop");
  // chaseBoth();
  lasttouched = currtouched;
}

static void chaseBoth() {
  chaseA(stripA.Color(255, 0, 0)); // Red
  chaseA(stripA.Color(0, 255, 0)); // Green
  chaseA(stripA.Color(0, 0, 255)); // Blue
  chaseA(stripA.Color(0, 0, 0)); // Off

  chaseB(stripB.Color(255, 0, 0)); // Red
  chaseB(stripB.Color(0, 255, 0)); // Green
  chaseB(stripB.Color(0, 0, 255)); // Blue
  chaseB(stripB.Color(0, 0, 0)); // Off
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
      tone(BEEPER_PIN, 261, 1000);
    }

    if (isCharged) {
      Serial.println("charged");
      tone(BEEPER_PIN, 220, 1000);
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
    tone(BEEPER_PIN, 220, 1000);
  }

  wasCharging = isCharging;
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