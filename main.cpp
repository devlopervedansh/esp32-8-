#include <Arduino.h>

int buttonPin = 13;
int whiteLED = 14;
int redLED = 33;
int greenLED = 32;

bool gameActive = false;
bool whiteOn = false;
bool buttonPressed = false;

unsigned long waitTime;
unsigned long whiteOnTime;
unsigned long reactionTimeLimit = 250;

void startGame();
void showSuccess();
void showFail();

void setup() {
  pinMode(whiteLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(115200);
  delay(1000);
  randomSeed(analogRead(0));

  Serial.println("Setup done ✅");
  startGame();
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  // Turn on white LED after the random wait
  if (!whiteOn && millis() >= whiteOnTime) {
    whiteOn = true;
    digitalWrite(whiteLED, HIGH);
    Serial.println("⚪️ White ON — Press any time now");
  }

  // Handle button press
  if (whiteOn && !buttonPressed && buttonState == LOW) {
    buttonPressed = true;

    unsigned long reactionTime = millis() - whiteOnTime;

    if (reactionTime <= reactionTimeLimit) {
      Serial.print("🟢 Pressed in ");
      Serial.print(reactionTime);
      Serial.println(" ms ✅");
      showSuccess();
    } else {
      Serial.print("🔴 Pressed too late (");
      Serial.print(reactionTime);
      Serial.println(" ms) ❌");
      showFail();
    }

    delay(2000);
    startGame();
  }
}

void startGame() {
  Serial.println("\n-----------------------------");
  Serial.println("New Round 🎮");

  digitalWrite(whiteLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);

  waitTime = random(2000, 5000);  // wait before turning on white
  whiteOnTime = millis() + waitTime;

  whiteOn = false;
  buttonPressed = false;
  gameActive = true;

  Serial.print("⏳ Waiting ");
  Serial.print(waitTime);
  Serial.println("ms before white LED");
}

void showSuccess() {
  digitalWrite(whiteLED, LOW);
  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, LOW);
  Serial.println("✅ SUCCESS: Green ON");
}

void showFail() {
  digitalWrite(whiteLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, HIGH);
  Serial.println("❌ FAIL: Red ON");
}
