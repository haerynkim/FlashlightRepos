/*
  ButtonBlink

  This code is capable of turning on and off a function that dims an LED every second. 
  This function is turned on when the switch is on, and turned off when the switch is off.

  modified 19 May 2018
  by Haeryn Kim

  This code is part of a project available in the public domain.
  
  http://github.com/haerynkim/FlashlightRepos
*/
// Define constants:
const int interruptPin = 2;
int PWM_max = 255;
const long interval = 500;
const int ledPin = 3;

// Define variables subject to change:
volatile bool buttonState = HIGH;
bool lastButtonState = HIGH;
int buttonPushCounter = 0;
unsigned long previousMillis = 0;
int PWM_b = 0;
volatile bool pinState = LOW;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  // initialize interruptPin (pin 2) as an input.
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changestate, FALLING);
  // initialize digital pin 3 as an output.
  pinMode(ledPin, OUTPUT);

}

// the loop function runs over and over again forever
void loop() {
  unsigned long currentMillis = millis();
  switch (buttonPushCounter % 5) {
    case 0:
      digitalWrite(ledPin, 0);
      break;
    case 1:
      analogWrite(ledPin, int(PWM_max/4));
      break;
    case 2:
      analogWrite(ledPin, int(PWM_max/2));
      break;
    case 3:
      digitalWrite(ledPin, PWM_max);
      break;
    case 4:
      blinkState = pinState;
      blink2(currentMillis, previousMillis);
  }
  lastButtonState = buttonState;
}

// the if statement detects if the button has been pushed and changes the value of the state variable, buttonState
void changestate() {
  buttonState = !buttonState;
  buttonPushCounter ++;
  Serial.println(buttonPushCounter);
}

void blink2(unsigned long currentMillis, unsigned long previousMillis) {
  if (currentMillis - previousMillis >= interval)
    previousMillis = currentMillis;
    if (blinkState == HIGH) {
      PWM_b = 0;
      pinState = HIGH;
      //return pinState;
    } else {
      PWM_b = PWM_max;
      pinState = LOW;
      //return pinState;
    }
    digitalWrite(ledPin, PWM_b);
}

