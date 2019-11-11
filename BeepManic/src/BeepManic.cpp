/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "application.h"
#line 1 "d:/Git/Git/TheCube/seattle-indies-photon-cube-jam-2019/BeepManic/src/BeepManic.ino"
/*
 * Project BeepManic
 * Description:
 * Author:
 * Date:
 */

void setup();
void loop();
#line 8 "d:/Git/Git/TheCube/seattle-indies-photon-cube-jam-2019/BeepManic/src/BeepManic.ino"
#define BEEPER_PIN A5
#define BUZZER_PIN A4

SYSTEM_MODE(MANUAL);

int notes[] = 
{0,
/* C,  C#,   D,  D#,   E,   F,  F#,   G,  G#,   A,  A#,   B */
3817,3597,3401,3205,3030,2857,2703,2551,2404,2273,2146,2024,   // 3 (1-12)
1908,1805,1701,1608,1515,1433,1351,1276,1205,1136,1073,1012,   // 4 (13-24)
 956, 903, 852, 804, 759, 716, 676, 638, 602, 568, 536, 506,   // 5 (25-37)
 478, 451, 426, 402, 379, 358, 338, 319, 301, 284, 268, 253,   // 6 (38-50)
 239, 226, 213, 201, 190, 179, 169, 159, 151, 142, 134, 127 }; // 7 (51-62)
// setup() runs once, when the device is first turned on.

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


int melody[] = {NOTE_E5,NOTE_E5,0,NOTE_E5,0,NOTE_C5,NOTE_E5,0,NOTE_G5,0,0,NOTE_G4};


int correctMelody[] = {NOTE_C5,NOTE_D5,NOTE_E5,NOTE_F5,NOTE_G5,NOTE_A5,NOTE_B5,NOTE_C6};
int incorrectMelody[] = {NOTE_E5,NOTE_E5,0,NOTE_E5,0,NOTE_C5,NOTE_E5,0,NOTE_G5,0,0,NOTE_G4};
int loadingMelodyBlue [] = {NOTE_E5,NOTE_E5,0,NOTE_E5,0,NOTE_C5,NOTE_E5,0,NOTE_G5,0,0,NOTE_G4};
int loadingMelodyRed [] = {NOTE_E5,NOTE_E5,0,NOTE_E5,0,NOTE_C5,NOTE_E5,0,NOTE_G5,0,0,NOTE_G4};
int loadingMelodyPink [] = {NOTE_E5,NOTE_E5,0,NOTE_E5,0,NOTE_C5,NOTE_E5,0,NOTE_G5,0,0,NOTE_G4};
int loadingMelodyOrange [] = {NOTE_E5,NOTE_E5,0,NOTE_E5,0,NOTE_C5,NOTE_E5,0,NOTE_G5,0,0,NOTE_G4};

int noteDurations[] = {4,4,4,4,4,4,4,4,4,2,4,4};
int correctMelodyDurations[] = {8,8,8,8,8,8,2,4};

void setup() {
  // Put initialization like pinMode and begin functions here.

  pinMode(BEEPER_PIN, OUTPUT);
  // pinMode(BUZZER_PIN, OUTPUT);
  // tone(BEEPER_PIN, 261, 0);
 
//  for (int thisNote = 0; thisNote < 12; thisNote++) {

//     // to calculate the note duration, take one second
//     // divided by the note type.
//     // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
//     int noteDuration = 60*1000/BPM/noteDurations[thisNote];
//     tone(BEEPER_PIN, (melody[thisNote]!=0)?(500000/melody[thisNote]):0,noteDuration-RELEASE);

//     // blocking delay needed because tone() does not block
//     delay(noteDuration);
//   }
// }

  for (int thisNote = 0; thisNote < (sizeof(correctMelody)/sizeof(int)); thisNote++) {

  int noteDuration = 60*1000/BPM/correctMelodyDurations[thisNote];
    tone(BEEPER_PIN, (correctMelody[thisNote]!=0)?(500000/correctMelody[thisNote]):0,noteDuration-RELEASE);
    delay(noteDuration);
  }
  

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  // soundOne();
  // tone(BUZZER_PIN, 261, 0);
  // tone(BEEPER_PIN, 261, 0);
}