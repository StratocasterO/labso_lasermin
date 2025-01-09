#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define HWSERIAL Serial1
char incomingByte;
String inputString = "";

// GUItool: begin automatically generated code
AudioSynthWaveform       wave1; //xy=146.19997024536133,169.19999885559082
AudioSynthWaveform       wave2;      //xy=117.20000076293945,215.19999885559082
AudioSynthWaveform       wave3; //xy=121.19999694824219,271.1999816894531
AudioSynthWaveform       wave4; //xy=143.1999969482422,329.1999816894531
AudioMixer4              mix;         //xy=305.2000312805176,236.1999969482422
AudioFilterLadder        filter;        //xy=421.2000045776367,337.1999988555908
AudioAmplifier           amp;           //xy=480.2000961303711,223.1999969482422
AudioEffectFreeverbStereo freeverb;     //xy=678.2000389099121,203.20001220703125
AudioMixer4              L;         //xy=842.2000389099121,306.1999988555908
AudioMixer4              R;         //xy=845.1999969482422,391.19999504089355
AudioOutputI2S           out;           //xy=986.0002136230469,357.00001525878906
AudioConnection          patchCord1(wave2, 0, mix, 1);
AudioConnection          patchCord2(wave3, 0, mix, 2);
AudioConnection          patchCord3(wave4, 0, mix, 3);
AudioConnection          patchCord4(wave1, 0, mix, 0);
AudioConnection          patchCord5(mix, 0, filter, 0);
AudioConnection          patchCord6(filter, amp);
AudioConnection          patchCord7(amp, freeverb);
AudioConnection          patchCord8(amp, 0, L, 0);
AudioConnection          patchCord9(amp, 0, R, 0);
AudioConnection          patchCord10(freeverb, 0, L, 1);
AudioConnection          patchCord11(freeverb, 1, R, 1);
AudioConnection          patchCord12(L, 0, out, 0);
AudioConnection          patchCord13(R, 0, out, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=258.1999740600586,74.20000076293945
// GUItool: end automatically generated code


float vol;

void setup(void) {
  HWSERIAL.begin(115200);

  AudioMemory(8);
  audioShield.enable();
  audioShield.volume(0.65);

  osc1.begin(0.25, 220, WAVEFORM_SINE);
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