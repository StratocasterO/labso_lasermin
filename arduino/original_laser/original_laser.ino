#include <VL53L0X.h>
#include <Wire.h>
#include <pitches.h>

VL53L0X sensor;

// Connections setup
#define OUTPUT 9
#define CONT 8
#define MODE A0
#define PITCH A1
#define SECOND 2
#define THIRD 3
#define FOURTH 4
#define FIFTH 5
#define SIXTH 6
#define SEVENTH 7

// Constants
#define MIN 6.0
#define MAX 45.0
#define MIN_NOTE 220.0
#define MAX_NOTE 880.0

// Note arrays
int notesC[] = { NOTE_C3,  
                 NOTE_CS3,
                 NOTE_D3,
                 NOTE_DS3,
                 NOTE_E3,
                 NOTE_F3,
                 NOTE_FS3,
                 NOTE_G3,
                 NOTE_GS3,
                 NOTE_A3,
                 NOTE_AS3,
                 NOTE_B3,
                 NOTE_C4,
                 NOTE_CS4,
                 NOTE_D4,
                 NOTE_DS4,
                 NOTE_E4,
                 NOTE_F4,
                 NOTE_FS4,
                 NOTE_G4,
                 NOTE_GS4,
                 NOTE_A4,
                 NOTE_AS4,
                 NOTE_B4,
                 NOTE_C5 };
int notesD[] = { NOTE_C3,
                 NOTE_D3,
                 NOTE_E3,
                 NOTE_F3,
                 NOTE_G3,
                 NOTE_A3,
                 NOTE_B3,
                 NOTE_C4,
                 NOTE_D4,
                 NOTE_E4,
                 NOTE_F4,
                 NOTE_G4,
                 NOTE_A4,
                 NOTE_B4,
                 NOTE_C5 };
int notesP[] = { NOTE_C3,
                 NOTE_D3,
                 NOTE_E3,
                 NOTE_G3,
                 NOTE_A3,
                 NOTE_C4,
                 NOTE_D4,
                 NOTE_E4,
                 NOTE_G4,
                 NOTE_A4,
                 NOTE_C5 };

// Variables
float distNew, dist = 20;
int note;
float pitch;
float pot = 1.0;
int vol, wav, fil, dro, rev;

unsigned long previousMillis = 0;
const long interval = 10;

void setup() {
  Serial.begin(115200);

  // sensor initialization
  Wire.begin();
  sensor.init();
  sensor.setTimeout(500);
  sensor.startContinuous();

  // define mode of pins
  pinMode(SECOND, INPUT_PULLUP);
  pinMode(THIRD, INPUT_PULLUP);
  pinMode(FOURTH, INPUT_PULLUP);
  pinMode(FIFTH, INPUT_PULLUP);
  pinMode(SIXTH, INPUT_PULLUP);
  pinMode(SEVENTH, INPUT_PULLUP);
  pinMode(CONT, INPUT_PULLUP);

  // initialisation without smothing function to eliminate starting transient
  pot = analogRead(PITCH);
  pitch = 0.5 + (1.5 * (pot / 1024.0));
}

void loop() {
  distNew = 0.1 * sensor.readRangeContinuousMillimeters();
  dist = dist * 0.6 + float(distNew) * 0.4;  // with smoothing
  Serial.println(dist);

  if (dist >= MIN && dist <= MAX) {
    if (digitalRead(CONT) == HIGH) {  // continuous
      // note = map(dist, MIN, MAX, 55, 880);
      note = MIN_NOTE + ((MAX_NOTE - MIN_NOTE) * ((dist - MIN ) / (MAX - MIN)));
      tone(OUTPUT, note);
    } else {                                // discrete
      if (analogRead(MODE) > 900) {         // diatonic
        if (digitalRead(SECOND) == HIGH) {  // change notes of the array
          notesD[1] = NOTE_D3;
          notesD[8] = NOTE_D4;
        } else {
          notesD[1] = NOTE_CS3;
          notesD[8] = NOTE_CS4;
        }
        if (digitalRead(THIRD) == HIGH) {
          notesD[2] = NOTE_E3;
          notesD[9] = NOTE_E4;
        } else {
          notesD[2] = NOTE_DS3;
          notesD[9] = NOTE_DS4;
        }
        if (digitalRead(FOURTH) == HIGH) {
          notesD[3] = NOTE_FS3;
          notesD[10] = NOTE_FS4;
        } else {
          notesD[3] = NOTE_F3;
          notesD[10] = NOTE_F4;
        }
        if (digitalRead(FIFTH) == HIGH) {
          notesD[4] = NOTE_G3;
          notesD[11] = NOTE_G4;
        } else {
          notesD[4] = NOTE_FS3;
          notesD[11] = NOTE_FS4;
        }
        if (digitalRead(SIXTH) == HIGH) {
          notesD[5] = NOTE_A3;
          notesD[12] = NOTE_A4;
        } else {
          notesD[5] = NOTE_GS3;
          notesD[12] = NOTE_GS4;
        }
        if (digitalRead(SEVENTH) == HIGH) {
          notesD[6] = NOTE_B3;
          notesD[13] = NOTE_B4;
        } else {
          notesD[6] = NOTE_AS3;
          notesD[13] = NOTE_AS4;
        }

        note = map(dist, MIN, MAX, 0, sizeof(notesD) / sizeof(int));
        pot = pot * 0.6 + float(analogRead(PITCH)) * 0.4; // smoothing function
        pitch = 0.5 + (1.5 * (pot / 1024.0));       // pitch modifier (linear float mapping 0.5 - 2.0)
        tone(OUTPUT, notesD[note] * pitch);
      } else if (analogRead(MODE) < 100) {  // pentatonic
        note = map(dist, MIN, MAX, 0, sizeof(notesP) / sizeof(int));
        pot = pot * 0.6 + float(analogRead(PITCH)) * 0.4;
        pitch = 0.5 + (1.5 * (pot / 1024.0));
        tone(OUTPUT, notesP[note] * pitch);
      } else {  // chromatic
        note = map(dist, MIN, MAX, 0, sizeof(notesC) / sizeof(int));
        pot = pot * 0.9 + float(analogRead(PITCH)) * 0.1;
        pitch = 0.5 + (1.5 * (pot / 1024.0));
        tone(OUTPUT, notesC[note] * pitch);
      }
    }
  } else {
    noTone(OUTPUT);
  }

  // comprova valors només cada cert temps (interval in milliseconds)
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    vol = analogRead(F1);
    wav = analogRead(F2);
    fil = analogRead(F3);
    dro = analogRead(F4);
    rev = analogRead(F5);

    if (vol != pVol) {
      Serial.println("V " + vol);
      pVol = vol;
    }
    if (wav != pWav) {
      Serial.println("W " + wav);
      pWav = wav;
    }
    if (fil != pFil) {
      Serial.println("F " + fil);
      pFil = fil;
    }
    if (dro != pDro) {
      Serial.println("D " + dro);
      pDro = dro;
    }
    if (rev != pRev) {
      Serial.println("R " + rev);
      pRev = rev;
    }
}
