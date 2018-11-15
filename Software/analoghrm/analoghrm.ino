/*
  ButtonBlink

  This code is capable of turning on and off a function that dims an LED every second. 
  This function is turned on when the switch is on, and turned off when the switch is off.

  modified 19 May 2018
  by Haeryn Kim

  This code is part of a project available in the public domain.
  
  http://github.com/haerynkim/FlashlightRepos
*/
//#define USE_ARDUINO_INTERRUPTS false
//#include <PulseSensorPlayground.h>

// Define constants:
const int interruptPin = 2;
int PWM_max = 255;
int ledPin = 3;
const int PULSE_INPUT = A0;

// Define variables subject to change:
volatile bool buttonState = HIGH;
bool lastButtonState = HIGH;
int buttonPushCounter = 0;
int THRES = 0;

//PulseSensorPlayground pulseSensor;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  // initialize interruptPin (pin 2) as an input.
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changestate, FALLING);
  // initialize digital pin 3 as an output.
  pinMode(ledPin, OUTPUT);
//  pulseSensor.analogInput(PULSE_INPUT);
//  pulseSensor.setSerial(Serial);
//  if (!pulseSensor.begin()) {
//    for(;;) {
//      digitalWrite(ledPin, LOW);
//      delay(50);
//      digitalWrite(ledPin, HIGH);
//      delay(50);
//    }
//  }

}

float sig = 0;
float newsig = 0;
int val[200];
int maxVal = 0;
float oldtime = 0;
float newtime = 0;
float PInt = 0;
float HR = 0;
// the loop function runs over and over again forever
void loop() {
//  while (i<sample){
//    int val = analogRead(PULSE_INPUT);
//    if (val > maxVal) {
//      maxVal = val;
//  }
//  Serial.println(maxVal);
//  THRES = int(maxVal*0.8);
//  pulseSensor.setThreshold(THRES);
//  }
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
      for (int i=0; i <= 200; i++){
      val[i] = analogRead(PULSE_INPUT);
        if (val[i] > maxVal) {
          maxVal = val[i];
        }
      }
      sig = analogRead(PULSE_INPUT);
      Serial.println(sig);
      delay(800);
      newsig = analogRead(PULSE_INPUT);
      Serial.println(newsig);
      if ((newsig - sig) > float(maxVal)*0.5) {
        oldtime = millis();
        PInt = newtime - oldtime;
        newtime = oldtime;
        HR = 60/(PInt/1000);
      }   
      Serial.println(HR);
      break;
  }
  lastButtonState = buttonState;
}

// the if statement detects if the button has been pushed and changes the value of the state variable, buttonState
void changestate() {
  buttonState = !buttonState;
  buttonPushCounter ++;
}

