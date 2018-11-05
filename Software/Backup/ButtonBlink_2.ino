/*
  ButtonBlink

  This code is capable of turning on and off a function that dims an LED every second. 
  This function is turned on when the switch is on, and turned off when the switch is off.

  modified 19 May 2018
  by Haeryn Kim

  This code is part of a project available in the public domain.
  
  http://github.com/haerynkim/FlashlightRepos
*/
// Define constants subject to change:
const int interruptPin = 2;

// Define variables subject to change:
volatile bool buttonState = HIGH;
bool lastButtonState = HIGH;
int buttonPushCounter = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  // initialize interruptPin (pin 2) as an input.
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changestate, FALLING);
  // initialize digital pin 3 as an output.
  pinMode(3, OUTPUT);

}

// the loop function runs over and over again forever
void loop() {
 // if (buttonState != lastButtonState) {
   
  switch (buttonPushCounter % 4) {
    case 0:
      digitalWrite(3, LOW);
      break;
    case 1:
      analogWrite(3, 51);
      break;
    case 2:
      analogWrite(3, 127);
      break;
    case 3:
      digitalWrite(3, HIGH);
      break;
  }
  lastButtonState = buttonState;
}

void blinkLED(){
  digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  analogWrite(3, 127);   // turn the LED on intermediate brightness at 50% duty cycle
  delay(1000); 
  analogWrite(3, 51);   // turn the LED on lowest brightness setting at 20% duty cycle
  delay(1000);
  digitalWrite(3, LOW);    // turn the LED off by making the voltage LOW
  delay(1000); 
}

// the if statement detects if the button has been pushed and changes the value of the state variable, buttonState
void changestate() {
  buttonState = !buttonState;
  buttonPushCounter ++;
  Serial.println(buttonPushCounter);
}
