#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define HWSERIAL Serial1
char incomingByte;
String inputString = "";

AudioSynthWaveform osc;
AudioEffectEnvelope env;
AudioOutputI2S out;
AudioConnection patchCord1(osc, env);
AudioConnection patchCord2(env, 0, out, 0);
AudioConnection patchCord3(env, 0, out, 1);
AudioControlSGTL5000 audioShield;

void setup(void) {
  HWSERIAL.begin(115200);

  AudioMemory(8);
  audioShield.enable();
  audioShield.volume(0.65);

  osc.begin(0.5, 220, WAVEFORM_SINE);

  env.attack(50);
  env.decay(50);
  env.release(250);
  env.noteOn();
}

void loop() {
  if (HWSERIAL.available() > 0) {
    incomingByte = HWSERIAL.read();

    if (incomingByte == 10) {
      inputString.trim();
      Serial.println(inputString);

      if (inputString == "SINE") osc.begin(WAVEFORM_SINE);
      if (inputString == "TRI") osc.begin(WAVEFORM_TRIANGLE);
      if (inputString == "SAW") osc.begin(WAVEFORM_SAWTOOTH);

      inputString = "";
    } else {
      inputString = inputString + String(incomingByte);
    }
  }
}