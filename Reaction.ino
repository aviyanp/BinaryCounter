const int ledPins[4]    = {6, 0, 7, 4};
const int switchPins[4] = {26, 27, 28, 29};

enum GameState {
  WAITING_TO_START,
  WAITING_FOR_LIGHT,
  WAITING_FOR_REACTION
};

GameState state = WAITING_TO_START;

unsigned long randomDelay = 0;
unsigned long stateStartTime = 0;
unsigned long ledOnTime = 0;
int activeLed = -1;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(switchPins[i], INPUT_PULLUP);
  }
  randomSeed(10);
  resetGame();
}

int isAnySwitchPressed() {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(switchPins[i]) == LOW) {
      return i;
    }
  }
  return -1;
}

void showScoreBinary(int score) {
  for (int i = 0; i < 4; i++) {
    int bit = (score >> i) & 0x01;
    if (bit == 1) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
  delay(1500);
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void resetGame() {
  for (int i = 0; i < 4; i++) digitalWrite(ledPins[i], LOW);
  state = WAITING_TO_START;
  activeLed = -1;
}

void loop() {
  switch (state) {
    case WAITING_TO_START: {
      int pressed = isAnySwitchPressed();
      if (pressed != -1) {
        randomDelay = random(2000, 5001);
        stateStartTime = millis();
        state = WAITING_FOR_LIGHT;
      }
      break;
    }
    case WAITING_FOR_LIGHT: {
      if (isAnySwitchPressed() != -1) {
        showScoreBinary(0);
        resetGame();
        break;
      }

      if (millis() - stateStartTime >= randomDelay) {
        activeLed = random(0, 4);
        digitalWrite(ledPins[activeLed], HIGH);
        ledOnTime = millis();
        state = WAITING_FOR_REACTION;
      }
      break;
    }
    case WAITING_FOR_REACTION: {
      int pressed = isAnySwitchPressed();
      if (pressed != -1) {
        digitalWrite(ledPins[activeLed], LOW);
        if (pressed == activeLed) {
          unsigned long reactionTime = millis() - ledOnTime;
          // Score: 0 (slowest) to 15 (fastest)
          int score = 15 - constrain((reactionTime - 200) / 53, 0, 15);
          showScoreBinary(score);
        } else {
          showScoreBinary(0);
        }
        resetGame();
      }
      break;
    }
  }
}
