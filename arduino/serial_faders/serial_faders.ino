unsigned long previousMillis = 0;
const long interval = 10;

// define analog 15 as input
#define F1 2
#define F2 3
#define F3 4
#define F4 5
#define F5 6

// current values
int vol = 0;
int wav = 0;
int fil = 0;
int dro = 0;
int rev = 0;

// previous values
int pVol = 0;
int pWav = 0;
int pFil = 0;
int pDro = 0;
int pRev = 0;

void setup() {
  Serial.begin(115200);

  pinMode(F1, INPUT);
  pinMode(F2, INPUT);
  pinMode(F3, INPUT);
  pinMode(F4, INPUT);
  pinMode(F5, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // comprova valors només cada cert temps (interval in milliseconds)
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    vol = analogRead(F1);
    wav = analogRead(F2);
    fil = analogRead(F3);
    dro = analogRead(F4);
    rev = analogRead(F5);

    Serial.print(vol);
    Serial.print(",");
    Serial.print(wav);
    Serial.print(",");
    Serial.print(fil);
    Serial.print(",");
    Serial.print(dro);
    Serial.print(",");
    Serial.println(rev);

    // if (vol != pVol) {
    //   Serial.println("V " + vol);
    //   pVol = vol;
    // }
    // if (wav != pWav) {
    //   Serial.println("W " + wav);
    //   pWav = wav;
    // }
    // if (fil != pFil) {
    //   Serial.println("F " + fil);
    //   pFil = fil;
    // }
    // if (dro != pDro) {
    //   Serial.println("D " + dro);
    //   pDro = dro;
    // }
    // if (rev != pRev) {
    //   Serial.println("R " + rev);
    //   pRev = rev;
    // }
  }
}