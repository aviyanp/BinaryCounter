// LED pins
const int twoZero = 4;
const int twoOne = 0;
const int twoTwo = 7;
const int twoThree = 6;

// Switch pins
const int plusTwoZero = 28;
const int plusTwoOne = 29;
const int minusTwoZero = 27;
const int minusTwoOne = 26;

int counter = 0;
const int maxValue = 15; // 4 bits: 0-15

void setup() {
    pinMode(twoZero, OUTPUT);
    pinMode(twoOne, OUTPUT);
    pinMode(twoTwo, OUTPUT);
    pinMode(twoThree, OUTPUT);

    pinMode(plusTwoZero, INPUT_PULLUP);
    pinMode(plusTwoOne, INPUT_PULLUP);
    pinMode(minusTwoZero, INPUT_PULLUP);
    pinMode(minusTwoOne, INPUT_PULLUP);

}

void loop() {
    // Increment counter + debouncing
    if(digitalRead(plusTwoZero) == LOW){
        counter++;
        if(counter > maxValue) counter = 0;
        delay(100);
    }
   
    if(digitalRead(plusTwoOne) == LOW){
        counter += 2;
        if(counter > maxValue) counter = 0;
        delay(100);
    }
   
    if(digitalRead(minusTwoZero) == LOW){
        counter--;
        if(counter < 0) counter = maxValue;
        delay(100);
    }

    if(digitalRead(minusTwoOne) == LOW){
        counter -= 2;
        if(counter < 0) counter = maxValue;
        delay(100);
    }

    // Little easter egg - if you click all at the same time it blinks on/off
    
    if(digitalRead(plusTwoZero) && digitalRead(plusTwoOne) && digitalRead(minusTwoZero) && digitalRead(minusTwoOne) == LOW){
        digitalWrite(twoZero,HIGH);
        digitalWrite(twoOne, HIGH);
        digitalWrite(twoTwo, HIGH);
        digitalWrite(twoThree, HIGH);

        delay(500);

        digitalWrite(twoZero,LOW);
        digitalWrite(twoOne, LOW);
        digitalWrite(twoTwo, LOW);
        digitalWrite(twoThree, LOW);

    }

    digitalWrite(twoZero, (counter >> 0) & 1);
    digitalWrite(twoOne, (counter >> 1) & 1);
    digitalWrite(twoTwo, (counter >> 2) & 1);
    digitalWrite(twoThree, (counter >> 3) & 1)

}
