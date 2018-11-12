/*
  WorkingBPM

  This code is capable of processing BPM values from a pulse sensor and displaying the values on
  a LCD screen display.

  modified 11 Nov 2018
  by Haeryn Kim
  
  http://github.com/haerynkim/FlashlightRepos
*/
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include <Wire.h>
#include <SeeedOLED.h>

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
  Wire.begin();
  SeeedOled.init(); //initialize SeeedOLED display
  SeeedOled.clearDisplay(); //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay(); //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode(); //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0,0); //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("Hello BME590!"); //Print the String

  Serial.begin(9600);
  pulseSensor.begin(); //initialize pulse sensor
  pulseSensor.analogInput(PULSE_INPUT); //initialize pin A0 as analog input pin for pulse sensor
  pulseSensor.setSerial(Serial);
  
  pinMode(interruptPin, INPUT); //initialize pin 2 as interrupt pin for pushbutton
  attachInterrupt(digitalPinToInterrupt(interruptPin), changestate, FALLING);
  
  pinMode(ledPin, OUTPUT); //initialize digital pin 3 as an output for LED.
  
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
      SeeedOled.clearDisplay();
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
      pulseSensor.fadeOnPulse(ledPin);
      if (pulseSensor.sawStartOfBeat()) {
        SeeedOled.setTextXY(0,0);
        SeeedOled.putString("BPM:");
        SeeedOled.setTextXY(0,5);
        SeeedOled.putNumber(BPM);
        SeeedOled.setTextXY(1,0);
        SeeedOled.putString("Max BPM:");
        SeeedOled.setTextXY(1,9);
        SeeedOled.putNumber(maxBPM);
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
