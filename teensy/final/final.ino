#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define HWSERIAL Serial1
char incomingByte;
String inputString = "";

// GUItool: begin automatically generated code
AudioSynthWaveform       wave4; //xy=142.1999969482422,398.1999816894531
AudioSynthWaveform       wave1; //xy=144.19996643066406,191.1999969482422
AudioSynthWaveform       wave2;      //xy=147.20000457763672,259.1999969482422
AudioSynthWaveform       wave3; //xy=147.1999969482422,323.1999816894531
AudioMixer4              mix1;         //xy=352.20001220703125,209.1999969482422
AudioFilterLadder        filter;        //xy=492.20001220703125,214.20001220703125
AudioMixer4              mix2; //xy=647,275
AudioEffectFreeverbStereo freeverb;     //xy=821.2000732421875,357.20001220703125
AudioMixer4              L;         //xy=1019.2000732421875,274.20001220703125
AudioMixer4              R;         //xy=1022.2000312805176,359.200008392334
AudioOutputI2S           out;           //xy=1163.0002479553223,325.0000286102295
AudioConnection          patchCord1(wave4, 0, mix2, 1);
AudioConnection          patchCord2(wave1, 0, mix1, 0);
AudioConnection          patchCord3(wave2, 0, mix1, 1);
AudioConnection          patchCord4(wave3, 0, mix1, 2);
AudioConnection          patchCord5(mix1, 0, filter, 0);
AudioConnection          patchCord6(filter, 0, mix2, 0);
AudioConnection          patchCord7(mix2, freeverb);
AudioConnection          patchCord8(mix2, 0, L, 0);
AudioConnection          patchCord9(mix2, 0, R, 0);
AudioConnection          patchCord10(freeverb, 0, L, 1);
AudioConnection          patchCord11(freeverb, 1, R, 1);
AudioConnection          patchCord12(L, 0, out, 0);
AudioConnection          patchCord13(R, 0, out, 1);
AudioControlSGTL5000     audioShield;     //xy=154.19998168945312,473.1999969482422
// GUItool: end automatically generated code


float vol, freq, dro, rev;

void setup(void) {
  HWSERIAL.begin(115200);

  AudioMemory(8);
  audioShield.enable();
  audioShield.volume(1.);

  mix1.gain(0, .3);
  mix1.gain(1, .3);
  mix1.gain(2, .3);
  mix2.gain(0, .8);
  mix2.gain(1, .2);

  wave1.begin(1, 330, WAVEFORM_SINE);
  wave2.begin(1, 330, WAVEFORM_TRIANGLE);
  wave3.begin(1, 330, WAVEFORM_SAWTOOTH);
  wave4.begin(1, 220, WAVEFORM_TRIANGLE);

  freeverb.damping(0.3);
  filter.resonance(0.1);
}

void loop() {
  if (HWSERIAL.available() > 0) {
    incomingByte = HWSERIAL.read();

    if (incomingByte == 10) {
      inputString.trim();
      // Serial.println(inputString);

      if (inputString.substring(0,1) == "V") {
        // Serial.println("volum rebut: " + (String)inputString.substring(2).toFloat());
        float vol = map(inputString.substring(2).toFloat(), 0, 1024, 0., 1.);
        mix1.gain(0, .3 * vol);
        mix1.gain(1, .3 * vol);
        mix1.gain(2, .3 * vol);

      } else if (inputString.substring(0,1) == "W") {
        // Serial.println("ona rebuda: " + (String)inputString.substring(2).toFloat());
        wave1.amplitude(step(inputString.substring(2).toFloat(), -350., 310.));
        wave2.amplitude(step(inputString.substring(2).toFloat(), 310., 710.));
        wave3.amplitude(step(inputString.substring(2).toFloat(), 710., 1400.));

      } else if (inputString.substring(0,1) == "F") {
        // Serial.println("filtre rebut: " + (String)inputString.substring(2).toFloat());
        int freq = map(inputString.substring(2).toFloat(), 0, 1024, 0, 6000);
        filter.frequency(freq);

      } else if (inputString.substring(0,1) == "D") {
        // Serial.println("drone rebut: " + (String)inputString.substring(2).toFloat());
        float dro = map(inputString.substring(2).toFloat(), 0, 1024, 0., 1.);
        mix2.gain(1, .3 * dro);

      } else if (inputString.substring(0,1) == "R") {
        // Serial.println("reverb rebuda: " + (String)inputString.substring(2).toFloat());
        float rev = map(inputString.substring(2).toFloat(), 0, 1024, 0., 1.);
        freeverb.roomsize(rev);

      } else if (inputString.substring(0,1) == "N") {
        // Serial.println("nota rebuda: " + (String)inputString.substring(2).toFloat());
        wave1.frequency(inputString.substring(2).toFloat());
        wave2.frequency(inputString.substring(2).toFloat());
        wave3.frequency(inputString.substring(2).toFloat());

      } else if (inputString.substring(0,1) == "B") {
        Serial.println("baix rebut: " + (String)inputString.substring(2).toFloat());
        wave4.frequency(inputString.substring(2).toFloat());
      }

      inputString = "";
    } else {
      inputString = inputString + String(incomingByte);
    }
  }
}

float sigmoid(float x) {
  return 1 / (1 + pow(1.02, - x));
}

float step(float x, float a, float b) {
  return .5 * (sigmoid(x - a) - sigmoid(x - b));
}