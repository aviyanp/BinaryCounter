const int ledPins[4]    = {6, 0, 7, 4};
const int switchPins[4] = {26, 27, 28, 29};

const int maxSequenceLength = 32;
int sequence[maxSequenceLength];
int playerInput[maxSequenceLength];
int currentLength = 0;

const unsigned long debounceDelay = 40;

const int ledOnTime = 500;
const int ledOffTime = 200;
const int successFlashInterval = 250;
const int failureFlashInterval = 150;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(switchPins[i], INPUT_PULLUP);
  }
  randomSeed(10);
}

void loop() {
  if (currentLength < maxSequenceLength) {
    sequence[currentLength] = random(0, 4);
    currentLength++;
  }

  playSequence(currentLength);

  bool correct = readPlayerInput(currentLength);

  if (correct) {
    successFlashAll();
    delay(1000);
  } else {
    failureFlash();
    currentLength = 0;
    delay(1000);
  }
}

void playSequence(int length) {
  delay(400);
  for (int i = 0; i < length; i++) {
    int led = sequence[i];
    digitalWrite(ledPins[led], HIGH);
    delay(ledOnTime);
    digitalWrite(ledPins[led], LOW);
    delay(ledOffTime);
  }
}

bool readPlayerInput(int length) {
  for (int i = 0; i < length; i++) {
    int pressed = waitForButtonPress();
    playerInput[i] = pressed;
    digitalWrite(ledPins[pressed], HIGH);
    delay(150);
    digitalWrite(ledPins[pressed], LOW);

    if (pressed != sequence[i]) {
      return false;
    }
  }
  return true;
}

int waitForButtonPress() {
  while (true) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(switchPins[i]) == LOW) {
        unsigned long t0 = millis();
        while (digitalRead(switchPins[i]) == LOW) {
          if (millis() - t0 > debounceDelay) {
            while (digitalRead(switchPins[i]) == LOW);
            delay(10);
            return i;
          }
        }
      }
    }
  }
}

void successFlashAll() {
  for (int i = 0; i < 3; i++) {
    setAllLeds(HIGH);
    delay(successFlashInterval);
    setAllLeds(LOW);
    delay(successFlashInterval);
  }
}

void failureFlash() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPins[0], HIGH);
    digitalWrite(ledPins[3], HIGH);
    delay(failureFlashInterval);
    digitalWrite(ledPins[0], LOW);
    digitalWrite(ledPins[3], LOW);
    delay(failureFlashInterval);
  }
}

void setAllLeds(int state) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], state);
  }
}
