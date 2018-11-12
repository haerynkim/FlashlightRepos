/*
  ButtonBlink

  This code is capable of turning on and off a function that dims an LED every second. 
  This function is turned on when the switch is on, and turned off when the switch is off.

  modified 19 May 2018
  by Haeryn Kim

  This code is part of a project available in the public domain.
  
  http://github.com/haerynkim/FlashlightRepos
*/
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include <Wire.h>

// Define constants:
const int interruptPin = 2;
int PWM_max = 255;
int ledPin = 3;
const int PULSE_INPUT = 0;


// Define variables subject to change:
volatile bool buttonState = HIGH;
bool lastButtonState = HIGH;
int buttonPushCounter = 0;

PulseSensorPlayground pulseSensor;

// the setup function runs once when you press reset or power the board
void setup() {
  /*
     Use 115200 baud because that's what the Processing Sketch expects to read,
     and because that speed provides about 11 bytes per millisecond.

     If we used a slower baud rate, we'd likely write bytes faster than
     they can be transmitted, which would mess up the timing
     of readSensor() calls, which would make the pulse measurement
     not work properly.
  */
  Serial.begin(9600);
  pulseSensor.begin();
  // initialize interruptPin (pin 2) as an input.
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changestate, FALLING);
  // initialize digital pin 3 as an output.
  pinMode(ledPin, OUTPUT);
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.setSerial(Serial);
}

int maxBPM = 0;
int BPM = 0;

// the loop function runs over and over again forever
void loop() {
  int BPM = pulseSensor.getBeatsPerMinute();
  if (BPM>maxBPM){
    maxBPM = BPM;
  }
  switch (buttonPushCounter % 6) {
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
      digitalWrite(ledPin, PWM_max);
      delay(500);
      digitalWrite(ledPin, 0);
      delay(500);
      break;
    case 5:
      delay(20);
      if (pulseSensor.sawStartOfBeat()) {
        Serial.print("BPM: ");
        Serial.println(BPM);
        Serial.print("Max BPM: ");
        Serial.println(maxBPM);
      break;
      }
  lastButtonState = buttonState;
  }
}

// the if statement detects if the button has been pushed and changes the value of the state variable, buttonState
void changestate() {
  buttonState = !buttonState;
  buttonPushCounter ++;
}
