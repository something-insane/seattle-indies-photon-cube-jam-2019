#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Particle.h"
#include "Adafruit_MPR121.h"
#include "neopixel.h"

#define TEST_DISPLAY
#define TEST_TOUCH
#define TEST_NEOPIXEL
// #define TEST_BUZZER
// #define TEST_BEEPER

#define BEEPER_PIN TX
#define BUZZER_PIN WKP

SYSTEM_MODE(MANUAL);

#define CHARGING_PIN D7
#define CHARGED_PIN D6

#define PIXEL_PIN D2
#define SCL D1
#define SDA D0

#define PANEL_ONE   0
#define PANEL_TWO   1
#define PANEL_THREE 2
#define PANEL_FOUR  3
#define PANEL_FIVE  4
#define PANEL_SIX   5

#define PANEL_ONE_TOP_PIXEL       20
#define PANEL_ONE_RIGHT_PIXEL     18
#define PANEL_ONE_BOTTOM_PIXEL    16
#define PANEL_ONE_LEFT_PIXEL      21
#define PANEL_TWO_TOP_PIXEL       12
#define PANEL_TWO_RIGHT_PIXEL      7
#define PANEL_TWO_BOTTOM_PIXEL     6
#define PANEL_TWO_LEFT_PIXEL       1
#define PANEL_THREE_TOP_PIXEL     22
#define PANEL_THREE_RIGHT_PIXEL   11
#define PANEL_THREE_BOTTOM_PIXEL   5
#define PANEL_THREE_LEFT_PIXEL    23
#define PANEL_FOUR_TOP_PIXEL      17
#define PANEL_FOUR_RIGHT_PIXEL    10
#define PANEL_FOUR_BOTTOM_PIXEL   11
#define PANEL_FOUR_LEFT_PIXEL     14
#define PANEL_FIVE_TOP_PIXEL      19
#define PANEL_FIVE_RIGHT_PIXEL    24
#define PANEL_FIVE_BOTTOM_PIXEL    8
#define PANEL_FIVE_LEFT_PIXEL      9
#define PANEL_SIX_TOP_PIXEL       15
#define PANEL_SIX_RIGHT_PIXEL     13
#define PANEL_SIX_BOTTOM_PIXEL     0
#define PANEL_SIX_LEFT_PIXEL       3

#define STATE_LOADING              0
#define STATE_SHOW_PATTERN         1
#define STATE_GET_USER_INPUT       2
#define STATE_VICTORY              3
#define STATE_LOSER                4
#define STATE_USER_WAS_CORRECT     5

// current "level" or the number of panels that will light up for current "level"
int level;

// delay between showing the panels lighting up
int delayBetweenColors;

// potential randomness that messes with the delay being longer or shorter
int rndDelayRange;

// pattern to display to user per level each item representing a panel to light up
int pattern[10];

// represents which element in the pattern[] we are on and want to display next
int patternCount;

// represents the touch pin that represents the current correct corner to touch during gameplay
int currentCorrectPin;

// represents the guess number the user is currently on (each level has a number of guesses equal to the level number).
int numberOfGuesses;

// represents whether the user has just made a guess so we can give visual feedback and check for correct or incorrect
bool didGuessThisRound;

int gameState;

bool goingUp = true;

int glowyValue = 0;

int rate = 2;

void resetTheGlowyValues() {
  goingUp = true;
  glowyValue = 0;
  rate = 2;
}

void changeGameState(int newState) {
  // here we can do some cleanup / setup depending on the state we are about to enter
  switch (newState) {
    case STATE_LOADING:
      resetTheGlowyValues();

      break;
    case STATE_SHOW_PATTERN:
      // do nothing

      break;
    case STATE_GET_USER_INPUT:
      didGuessThisRound = false;

      break;
    case STATE_VICTORY:
      resetTheGlowyValues();

      break;
    case STATE_LOSER:
      resetTheGlowyValues();

      break;
  }

  gameState = newState;
}

void initCurrentLevel() {
  didGuessThisRound = false;
  patternCount = 0;

  int panels[] = { PANEL_ONE, PANEL_TWO, PANEL_THREE, PANEL_FOUR, PANEL_FIVE, PANEL_SIX };

  size_t panelSize = sizeof(panels) / sizeof(int);
  int count = static_cast<int>(panelSize);

  for (int i = 0; i < count; i += 1) {
    int rndIndex = PANEL_ONE + (rand() % static_cast<int>(PANEL_SIX - PANEL_ONE + 1));

    pattern[i] = panels[rndIndex];
  }

  currentCorrectPin = pattern[0];

  changeGameState(STATE_SHOW_PATTERN);
}

void initCube() {
  changeGameState(STATE_LOADING);
  level = 1;
  delayBetweenColors = 50;
  rndDelayRange = 1;
  patternCount = 0;
  numberOfGuesses = 0;
}

/**
 * Display
 */
#ifdef TEST_DISPLAY
#define OLED_CLK    A0
#define OLED_MOSI   A1
#define OLED_RESET  A2
#define OLED_DC     A3
#define OLED_CS     A4
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void displaySetup() {
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done

  display.display(); // show splashscreen
}
#endif

/**
 * Touch Board
 */
#ifdef TEST_TOUCH
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
#endif

/**
 * NeoPixels
 */
#ifdef TEST_NEOPIXEL

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D2
#define PIXEL_COUNT 24
#define PIXEL_TYPE WS2812B
#define BRIGHTNESS 255 // 0 - 255

int cubePanelLights[6][4] = {
  {PANEL_ONE_TOP_PIXEL, PANEL_ONE_RIGHT_PIXEL, PANEL_ONE_BOTTOM_PIXEL, PANEL_ONE_LEFT_PIXEL},
  {PANEL_TWO_TOP_PIXEL, PANEL_TWO_RIGHT_PIXEL, PANEL_TWO_BOTTOM_PIXEL, PANEL_TWO_LEFT_PIXEL},
  {PANEL_THREE_TOP_PIXEL, PANEL_THREE_RIGHT_PIXEL, PANEL_THREE_BOTTOM_PIXEL, PANEL_THREE_LEFT_PIXEL},
  {PANEL_FOUR_TOP_PIXEL, PANEL_FOUR_RIGHT_PIXEL, PANEL_FOUR_BOTTOM_PIXEL, PANEL_FOUR_LEFT_PIXEL},
  {PANEL_FIVE_TOP_PIXEL, PANEL_FIVE_RIGHT_PIXEL, PANEL_FIVE_BOTTOM_PIXEL, PANEL_FIVE_LEFT_PIXEL},
  {PANEL_SIX_TOP_PIXEL, PANEL_SIX_RIGHT_PIXEL, PANEL_SIX_BOTTOM_PIXEL, PANEL_SIX_LEFT_PIXEL},
};

int gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void neoPixelSetup() {
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();
}

void lightAllPanelsWithColor(uint32_t panelColor) {
  // 6 is the number of panels
  for (int i = 0; i < 6; i += 1) {
    // 4 is the number of lights per panel
    for (int j = 0; j < 4; j += 1) {
      strip.setPixelColor(cubePanelLights[i][j], panelColor);
    }
  }
}

#endif

#ifdef TEST_BEEPER
void beeperSetup() {
  pinMode(BEEPER_PIN, OUTPUT);
}
#endif

#ifdef TEST_BUZZER
void buzzerSetup() {
  pinMode(BUZZER_PIN, OUTPUT);
}
#endif

void setup() {
  // Serial.begin(9600);
  // delay(1000);
  // Serial.println("External Hardware Test");
  // delay(1000);
  // Serial.println("External Hardware Test");
  // delay(1000);
  // Serial.println("External Hardware Test");
  // delay(1000);
  // Serial.println("External Hardware Test");
  // delay(1000);
  // Serial.println("External Hardware Test");
  #ifdef TEST_DISPLAY
  // displaySetup();
  #endif

  #ifdef TEST_TOUCH
  touchSetup();
  #endif

  #ifdef TEST_NEOPIXEL
  neoPixelSetup();
  #endif

  #ifdef TEST_BEEPER
  beeperSetup();
  #endif

  #ifdef TEST_BUZZER
  buzzerSetup();
  #endif

  initCube();
}// THIS NEEDS DISPLAY


// #ifdef TEST_TOUCH
// #endif

// #ifdef TEST_DISPLAY
// THIS NEEDS DISPLAY
// unsigned long nextTime = 0;
// void printStatus(uint16_t currtouched) {
//   int textSize = 5 + 1 + 5 + 1 + 5;
//   int gapSize = (128 - (textSize * 6)) / 5;
//   for (int i = 0; i < 6; i++) {
//     display.setCursor((i * (gapSize+textSize)), 0);
//     display.print(currtouched & (1 << i) ? "***" : " - ");
//   }
//   for (int i = 0; i < 6; i++) {
//     display.setCursor((i * (gapSize+textSize)), 32);
//     display.print(currtouched & (1 << (i + 6)) ? "***" : " - ");
//   }
// }

// THIS NEEDS DISPLAY
// void printBaselineData(uint16_t currtouched) {
//   int textSize = 5 + 1 + 5 + 1 + 5;
//   int gapSize = (128 - (textSize * 6)) / 5;
//   for (int i = 0; i < 6; i++) {
//     display.setCursor(i * (gapSize+textSize), 0 + 8);
//     display.print(cap.baselineData(i));
//   }
//   for (int i = 0; i < 6; i++) {
//     display.setCursor(i * (gapSize+textSize), 32 + 8);
//     display.print(cap.baselineData(i + 6));
//   }
// }

// THIS NEEDS DISPLAY
// void printFilteredData(uint16_t currtouched) {
//   int textSize = 5 + 1 + 5 + 1 + 5;
//   int gapSize = (128 - (textSize * 6)) / 5;
//   for (int i = 0; i < 6; i++) {
//     display.setCursor(i * (gapSize+textSize), 0 + 8 + 10);
//     display.print(cap.filteredData(i));
//   }
//   for (int i = 0; i < 6; i++) {
//     display.setCursor(i * (gapSize+textSize), 32 + 8 + 10);
//     display.print(cap.filteredData(i + 6));
//   }
// }
// #endif

void neoPixelLoop() {
  // strip.setPixelColor(pixel_number, strip.Color(R, G, B));

  switch (gameState) {
    case STATE_LOADING:
      // do the fun loading light patterns

      // here we just light up all cube panels blue for "loading"
      lightAllPanelsWithColor(strip.Color(0, 0, 255));

      break;
    case STATE_SHOW_PATTERN:
      {
        // display the current levels pattern on the cube to user
        int currentPanel = pattern[patternCount];

        // TURN PANEL ON
        // 4 is the number of lights per panel
        for (int i = 0; i < 4; i += 1) {
          strip.setPixelColor(cubePanelLights[currentPanel][i], strip.Color(255, 255, 255));
        }

        strip.show();

        delay(2000);

        // TURN PANEL OFF
        for (int i = 0; i < 4; i += 1) {
          strip.setPixelColor(cubePanelLights[currentPanel][i], strip.Color(0, 0, 0));
        }

        strip.show();
        patternCount += 1;

        changeGameState(STATE_GET_USER_INPUT);
      }
      break;
    case STATE_GET_USER_INPUT:
      // give feedback when a corner is pressed
      if (didGuessThisRound == true) {
        // light up the corner that was touched
      }

      break;
    case STATE_VICTORY:
      // show some kind of cool "you did it correctly" pattern?

      // here we just light up all cube panels green for success
      lightAllPanelsWithColor(strip.Color(0, 255, 0));

      break;
    case STATE_LOSER:
      // wow what a jerk, our player failed

      // here we just light up all cube panels red for failure
      lightAllPanelsWithColor(strip.Color(255, 0, 0));

      break;
  }
}

void handleTouchedPin(int pinNumber) {
  didGuessThisRound = true;

  if (pinNumber == currentCorrectPin) {
    numberOfGuesses += 1;

    if (numberOfGuesses == level) {
      changeGameState(STATE_VICTORY);

      return;
    }

    currentCorrectPin = pattern[numberOfGuesses - 1];

    return;
  }

  // if we get here the incorrect pin number was pressed
  changeGameState(STATE_LOSER);
}

void handleReleasedPin(int pinNumber) {
  //
}

void touchSensorsLoop() {
  switch (gameState) {
    case STATE_LOADING:
      // do nothing

      break;
    case STATE_SHOW_PATTERN:
      // do nothing

      break;
    case STATE_GET_USER_INPUT:
      // get user input when touch sensors pressed and check for correct pattern input from user.
      for (uint8_t i=0; i<12; i++) {
        // it if *is* touched and *wasnt* touched before, alert!
        if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
          Serial.print(i); Serial.println(" touched");

          handleTouchedPin(i);
        }

      // if it *was* touched and now *isnt*, alert!
        if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
          Serial.print(i); Serial.println(" released");

          handleReleasedPin(i);
        }
      }

      // reset our state
      lasttouched = currtouched;

      // comment out this line for detailed data from the sensor!
      return;

      // debugging info, what
      Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
      Serial.print("Filt: ");
      for (uint8_t i=0; i<12; i++) {
        Serial.print(cap.filteredData(i)); Serial.print("\t");
      }
      Serial.println();
      Serial.print("Base: ");
      for (uint8_t i=0; i<12; i++) {
        Serial.print(cap.baselineData(i)); Serial.print("\t");
      }
      Serial.println();

      break;
    case STATE_VICTORY:
      // do nothing

      break;
  }
}

void incrementTheGlowyValues() {
  if (goingUp) {
    glowyValue += rate;

    if (glowyValue > 255) {
      glowyValue = 255;

      goingUp = false;
    }
  } else {
    glowyValue -= rate;

    if (glowyValue < 0) {
      glowyValue = 0;

      goingUp = true;
    }
  }
}

void loop() {
  #ifdef TEST_TOUCH
    touchSensorsLoop();
  #endif

  switch (gameState) {
    case STATE_LOADING:
      incrementTheGlowyValues();

      break;
    case STATE_SHOW_PATTERN:
      // do nothing

      break;
    case STATE_GET_USER_INPUT:
      // do nothing

      break;
    case STATE_VICTORY:
      incrementTheGlowyValues();

      break;
    case STATE_LOSER:
      incrementTheGlowyValues();

      break;
  }

  #ifdef TEST_BEEPER
  tone(BEEPER_PIN, static_cast<double>(glowyValue) / 255.0 * 5000, 0);
  // pinMode(TX, OUTPUT);
  // noTone(TX);
  #endif

  #ifdef TEST_BUZZER
  tone(BUZZER_PIN, static_cast<double>(glowyValue) / 255.0 * 5000, 0);
  // pinMode(TX, OUTPUT);
  // noTone(TX);
  #endif

  #ifdef TEST_NEOPIXEL
    neoPixelLoop();
  #endif

  // #ifdef TEST_DISPLAY
  // #ifdef TEST_TOUCH
  // if (millis() > nextTime) {
  //   display.clearDisplay();
  //   display.setTextSize(1);
  //   display.setTextColor(WHITE);
  //   display.setCursor(0,0);
  //   printStatus(currtouched);
  //   printFilteredData(currtouched);
  //   printBaselineData(currtouched);
  //   nextTime = millis() + 100;
  //   display.display();
  // }
  // #endif
  // #endif

  // put a delay so it isn't overwhelming
  delay(50);
}
