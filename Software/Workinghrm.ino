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
include <SeeedOLED.h>

// Define constants:
const int interruptPin = 2;
int PWM_max = 255;
int ledPin = 3;
const int PULSE_INPUT = 0;


// Define variables subject to change:
volatile bool buttonState = HIGH;
bool lastButtonState = HIGH;
int buttonPushCounter = 0;
int THRES = 520;

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
  Serial.begin(115200);
  Wire.begin();
  pulseSensor.begin();
  // initialize interruptPin (pin 2) as an input.
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changestate, FALLING);
  // initialize digital pin 3 as an output.
  pinMode(ledPin, OUTPUT);
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.setSerial(Serial);
  pulseSensor.setThreshold(THRES);
  SeedOled.init(); //initialize SEEED OLED display

  SeeedOled.clearDisplay();           //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();       //Set display to Normal mode
  SeeedOled.setPageMode();            //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0,0);           //Set the cursor to 0th Page, 0th Column  
  SeeedOled.putString("Hello World!");           //Print the String
  SeeedOled.setTextXY(1,0);           //Set the cursor to 1st Page, 0th Column  
  SeeedOled.putNumber(0xFFFF);        //Print number
  SeeedOled.setTextXY(2,0);           //Set the cursor to 2nd Page, 0th Column  
  SeeedOled.putNumber(0xFFFFFFFF);    //Print number
  SeeedOled.setTextXY(3,0);           //Set the cursor to 3rd Page, 0th Column  
  SeeedOled.putNumber(-12345);        //Print number
}

// the loop function runs over and over again forever
void loop() {
  switch (5) {
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
      int BPM = pulseSensor.getBeatsPerMinute();
      Serial.println(BPM);
      break;
  }
  lastButtonState = buttonState;
}

// the if statement detects if the button has been pushed and changes the value of the state variable, buttonState
void changestate() {
  buttonState = !buttonState;
  buttonPushCounter ++;
}

