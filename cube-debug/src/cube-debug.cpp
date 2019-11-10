/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

<<<<<<< HEAD
<<<<<<< HEAD
#line 1 "c:/Users/me/Dropbox/2019-games/particle/seattle-indies-photon-cube-jam-2019/cube-debug/src/cube-debug.ino"

#include <math.h>
=======
#line 1 "/Users/matthewmccord/Workspace/photon_iot/seattle-indies-photon-cube-jam-2019/cube-debug/src/cube-debug.ino"
>>>>>>> 731424d142190ec06f28eebceeba14a6ae3b79f2
=======
#line 1 "/Users/matthewmccord/Workspace/photon_iot/seattle-indies-photon-cube-jam-2019/cube-debug/src/cube-debug.ino"
>>>>>>> 731424d142190ec06f28eebceeba14a6ae3b79f2
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Particle.h"
#include "Adafruit_MPR121.h"
#include "neopixel.h"

// #define TEST_DISPLAY
void displaySetup();
void touchSetup();
void neoPixelSetup();
void beeperSetup();
void buzzerSetup();
void setup();
void printStatus(uint16_t currtouched);
void printBaselineData(uint16_t currtouched);
void printFilteredData(uint16_t currtouched);
void loop();
<<<<<<< HEAD
<<<<<<< HEAD
#line 9 "c:/Users/me/Dropbox/2019-games/particle/seattle-indies-photon-cube-jam-2019/cube-debug/src/cube-debug.ino"
#define TEST_DISPLAY
=======
#line 8 "/Users/matthewmccord/Workspace/photon_iot/seattle-indies-photon-cube-jam-2019/cube-debug/src/cube-debug.ino"
>>>>>>> 731424d142190ec06f28eebceeba14a6ae3b79f2
=======
#line 8 "/Users/matthewmccord/Workspace/photon_iot/seattle-indies-photon-cube-jam-2019/cube-debug/src/cube-debug.ino"
>>>>>>> 731424d142190ec06f28eebceeba14a6ae3b79f2
#define TEST_TOUCH
#define TEST_NEOPIXEL
#define TEST_BUZZER
// #define TEST_BEEPER

#define BEEPER_PIN TX
#define BUZZER_PIN WKP

SYSTEM_MODE(MANUAL);

#define CHARGING_PIN D7
#define CHARGED_PIN D6

#define PIXEL_PIN D2
// #define SCL D1
// #define SDA D0

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

int led_gamma[] = {
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
typedef struct {
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);

hsv rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
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
  Serial.begin(9600);
  delay(1000);
  Serial.println("External Hardware Test");
  delay(1000);
  Serial.println("External Hardware Test");
  delay(1000);
  Serial.println("External Hardware Test");
  delay(1000);
  Serial.println("External Hardware Test");
  delay(1000);
  Serial.println("External Hardware Test");
  #ifdef TEST_DISPLAY
  displaySetup();
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

  pinMode(CHARGING_PIN, INPUT_PULLDOWN);
}

#ifdef TEST_DISPLAY
#ifdef TEST_TOUCH
unsigned long nextTime = 0;
void printStatus(uint16_t currtouched) {
  int textSize = 5 + 1 + 5 + 1 + 5;
  int gapSize = (128 - (textSize * 6)) / 5;
  for (int i = 0; i < 6; i++) {
    display.setCursor((i * (gapSize+textSize)), 0);
    display.print(currtouched & (1 << i) ? "***" : " - ");
  }
  for (int i = 0; i < 6; i++) {
    display.setCursor((i * (gapSize+textSize)), 32);
    display.print(currtouched & (1 << (i + 6)) ? "***" : " - ");
  }
}

void printBaselineData(uint16_t currtouched) {
  int textSize = 5 + 1 + 5 + 1 + 5;
  int gapSize = (128 - (textSize * 6)) / 5;
  for (int i = 0; i < 6; i++) {
    display.setCursor(i * (gapSize+textSize), 0 + 8);
    display.print(cap.baselineData(i));
  }
  for (int i = 0; i < 6; i++) {
    display.setCursor(i * (gapSize+textSize), 32 + 8);
    display.print(cap.baselineData(i + 6));
  }
}

void printFilteredData(uint16_t currtouched) {
  int textSize = 5 + 1 + 5 + 1 + 5;
  int gapSize = (128 - (textSize * 6)) / 5;
  for (int i = 0; i < 6; i++) {
    display.setCursor(i * (gapSize+textSize), 0 + 8 + 10);
    display.print(cap.filteredData(i));
  }
  for (int i = 0; i < 6; i++) {
    display.setCursor(i * (gapSize+textSize), 32 + 8 + 10);
    display.print(cap.filteredData(i + 6));
  }
}
#endif
#endif

bool goingUp = true;
int value = 0;
int rate = 2;

void loop() {

  #ifdef TEST_TOUCH
  // cap.setThresholds(touch_threshold, release_threshold);
  // Get the currently touched pads
  currtouched = cap.touched();
  #endif

  if (goingUp) {
    value += rate;
    if (value > 255) {
      value = 255;
      goingUp = false;
    }
  } else {
    value -= rate;
    if (value < 0) {
      value = 0;
      goingUp = true;
    }
  }

  #ifdef TEST_BEEPER
  tone(BEEPER_PIN, (double)value / 255.0 * 5000, 0);
  // pinMode(TX, OUTPUT);
  // noTone(TX);
  #endif

  #ifdef TEST_BUZZER
  tone(BUZZER_PIN, (double)value / 255.0 * 5000, 0);
  // pinMode(TX, OUTPUT);
  // noTone(TX);
  #endif

  #ifdef TEST_NEOPIXEL
<<<<<<< HEAD
<<<<<<< HEAD
  for (int i = 0; i < PIXEL_COUNT - 3; i++) {
    rgb color = hsv2rgb(hsv {
      (rand() % 36000)/ 100.0, // hue 0..1
      1, // saturation 0..1
      1 // value 0..1
    });
    strip.setPixelColor(i, strip.Color(color.r * 255, color.g * 255, color.b * 255));
=======
=======
>>>>>>> 731424d142190ec06f28eebceeba14a6ae3b79f2

  strip.setPixelColor(0, strip.Color(value, 00000, 00000));
  strip.setPixelColor(1, strip.Color(00000, value, 00000));
  strip.setPixelColor(2, strip.Color(00000, 00000, value));
  for (int i = 3; i < PIXEL_COUNT - 3; i++)
    strip.setPixelColor(i, strip.Color(value, value, value));
  if (PIXEL_COUNT > 6) {

    strip.setPixelColor(PIXEL_COUNT - 1, strip.Color(value, 00000, 00000));
    strip.setPixelColor(PIXEL_COUNT - 2, strip.Color(00000, value, 00000));
    strip.setPixelColor(PIXEL_COUNT - 3, strip.Color(00000, 00000, value));
>>>>>>> 731424d142190ec06f28eebceeba14a6ae3b79f2
  }
  strip.show();
  #endif

  #ifdef TEST_DISPLAY
  #ifdef TEST_TOUCH
  if (millis() > nextTime) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    printStatus(currtouched);
    printFilteredData(currtouched);
    printBaselineData(currtouched);
    nextTime = millis() + 100;
    display.display();
  }


  for (uint8_t i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released");
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
  #endif
  #endif

  // put a delay so it isn't overwhelming
  delay(50);
}