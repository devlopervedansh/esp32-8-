#include <Arduino.h>

int leds[] = {14, 32, 33, 13, 27};
int numLeds = 5;

unsigned long nowTime = 0;
unsigned long lastTick = 0;

int globalSpeed = 5;
bool invertOutputs = false;

unsigned long patternStart = 0;
int currentPattern = 0;
int totalPatterns = 24;

unsigned long patternDurations[] = {
  3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000,
  3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000,
  3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000
};

int seedPin = 35;

void setupPins() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
}

void setLed(int idx, bool state) {
  if (idx < 0 || idx >= numLeds) return;
  if (invertOutputs) state = !state;
  digitalWrite(leds[idx], state ? HIGH : LOW);
}

void setAll(bool state) {
  for (int i = 0; i < numLeds; i++) setLed(i, state);
}

void patternOneByOneForward(unsigned long msPerLed) {
  static int i = 0;
  static unsigned long last = 0;
  if (nowTime - last >= msPerLed) {
    last = nowTime;
    setAll(false);
    setLed(i, true);
    i = (i + 1) % numLeds;
  }
}

void patternOneByOneBackward(unsigned long msPerLed) {
  static int i = numLeds - 1;
  static unsigned long last = 0;
  if (nowTime - last >= msPerLed) {
    last = nowTime;
    setAll(false);
    setLed(i, true);
    i = (i - 1 + numLeds) % numLeds;
  }
}

void patternBounce(unsigned long msPerLed) {
  static int i = 0, dir = 1;
  static unsigned long last = 0;
  if (nowTime - last >= msPerLed) {
    last = nowTime;
    setAll(false);
    setLed(i, true);
    i += dir;
    if (i >= numLeds) { i = numLeds - 2; dir = -1; }
    if (i < 0) { i = 1; dir = 1; }
  }
}

void patternAllBlink(unsigned long msOn, unsigned long msOff) {
  static bool on = false;
  static unsigned long last = 0;
  if (on) {
    if (nowTime - last >= msOn) { last = nowTime; on = false; setAll(false); }
  } else {
    if (nowTime - last >= msOff) { last = nowTime; on = true; setAll(true); }
  }
}

void patternTwoAtATimeSlide(unsigned long msPerStep) {
  static int i = 0;
  static unsigned long last = 0;
  if (nowTime - last >= msPerStep) {
    last = nowTime;
    setAll(false);
    setLed(i, true);
    setLed((i + 1) % numLeds, true);
    i = (i + 1) % numLeds;
  }
}

void patternRandomPulse(unsigned long msPerFlash) {
  static unsigned long last = 0;
  if (nowTime - last >= msPerFlash) {
    last = nowTime;
    setAll(false);
    setLed(random(0, numLeds), true);
  }
}

void patternKnightRider(unsigned long msPerStep) {
  static int i = 0, dir = 1;
  static unsigned long last = 0;
  if (nowTime - last >= msPerStep) {
    last = nowTime;
    setAll(false);
    setLed(i, true);
    i += dir;
    if (i >= numLeds) { i = numLeds - 2; dir = -1; }
    if (i < 0) { i = 1; dir = 1; }
  }
}

void runPatternByIndex(int idx) {
  int speed = 40;
  switch (idx) {
    case 0: patternOneByOneForward(speed); break;
    case 1: patternOneByOneBackward(speed); break;
    case 2: patternBounce(speed); break;
    case 3: patternAllBlink(60, 60); break;
    case 4: patternTwoAtATimeSlide(speed); break;
    case 5: patternRandomPulse(50); break;
    case 6: patternKnightRider(speed); break;
    default: patternRandomPulse(40); break;
  }
}

void advancePatternIfNeeded() {
  if (nowTime - patternStart >= patternDurations[currentPattern]) {
    currentPattern = (currentPattern + 1) % totalPatterns;
    patternStart = nowTime;
  }
}

void setup() {
  Serial.begin(115200);
  setupPins();
  pinMode(seedPin, INPUT);
  randomSeed(analogRead(seedPin));
  setAll(false);
  patternStart = millis();
}

void loop() {
  nowTime = millis();
  runPatternByIndex(currentPattern);
  advancePatternIfNeeded();
}
