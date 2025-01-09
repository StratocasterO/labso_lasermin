#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define HWSERIAL Serial1
char incomingByte;
String inputString = "";

AudioSynthWaveform       osc1;      //xy=254.20000076293945,203.19999980926514
AudioSynthWaveform       osc2;      //xy=283.1999702453613,157.19999980926514
AudioMixer4              mix;       //xy=571.2000350952148,208.20000076293945
AudioEffectEnvelope      env;       //xy=730.2000350952148,209.20001602172852
AudioOutputI2S           out;       //xy=877.0001487731934,210.0000343322754
AudioConnection          patchCord1(osc1, 0, mix, 1);
AudioConnection          patchCord2(osc2, 0, mix, 0);
AudioConnection          patchCord3(mix, env);
AudioConnection          patchCord4(env, 0, out, 0);
AudioConnection          patchCord5(env, 0, out, 1);
AudioControlSGTL5000     audioShield;

float vol;

void setup(void) {
  HWSERIAL.begin(115200);

  AudioMemory(8);
  audioShield.enable();
  audioShield.volume(0.65);

  osc1.begin(0.5, 220, WAVEFORM_SINE);
  osc2.begin(0.5, 220, WAVEFORM_SAWTOOTH);

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

      vol = inputString.toFloat();
      vol = map(vol, 100., 800., 0.0, 1.0);
      osc2.amplitude(vol);
      osc1.amplitude(1.0 - vol);

      inputString = "";
    } else {
      inputString = inputString + String(incomingByte);
    }
  }
}