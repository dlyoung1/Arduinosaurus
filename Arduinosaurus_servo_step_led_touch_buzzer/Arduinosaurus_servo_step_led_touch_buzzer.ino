//Most of the following code was gathered from the DroneBot Workshop. The lessons can be found on youtube at https://www.youtube.com/channel/UCzml9bXoEM0itbcE96CB03w
//The xmas song came from https://create.arduino.cc/projecthub/joshi/piezo-christmas-songs-fd1ae9

#define touchPin 2
#define melodyPin 12
#include "pitches.h"
#include <Stepper.h>
#include <Servo.h>

const int ledCount = 5;
int ledPins[] = {
  3, 4, 5, 6, 7
};

int melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};
int tempo[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

const float STEPS_PER_REV = 32;
const float GEAR_RED = 64;
const float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED;
int StepsRequired;
Stepper steppermotor(STEPS_PER_REV, 8, 10, 9, 11);
Servo left_arm_servo;
Servo right_arm_servo;
int servo_position = 180;

void setup() {
  Serial.begin(9600);
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    pinMode(ledPins[thisLed], OUTPUT);
  }
  pinMode(touchPin, INPUT);
  pinMode(melodyPin, OUTPUT); //Buzzer
  left_arm_servo.attach (13);
  right_arm_servo.attach (19);
}

void loop() {
  int touchValue = digitalRead(touchPin);
  if (touchValue == HIGH) {
    
    for(int thisLed = 0; thisLed < ledCount; thisLed++) {
      digitalWrite(ledPins[thisLed], HIGH);
    }
    
    StepsRequired = STEPS_PER_OUT_REV / 4;
    steppermotor.setSpeed(900);
    
    for(servo_position = 180; servo_position >= 112; servo_position -= 1) {
      buzz(melodyPin, NOTE_G2, 100);
      left_arm_servo.write(servo_position);
      right_arm_servo.write(270 - servo_position);
      if(servo_position > 145) {
        steppermotor.step(StepsRequired / 69);
      } else {
        steppermotor.step( - StepsRequired / 69);
      }
      delay(10);
    }

    for(servo_position = 112; servo_position <= 180; servo_position +=1) {
      buzz(melodyPin, NOTE_A1, 50);
      left_arm_servo.write(servo_position);
      right_arm_servo.write(270 - servo_position);
      if(servo_position > 146) {
        steppermotor.step(StepsRequired / 69);
      } else {
        steppermotor.step( - StepsRequired / 69);
      }
      delay(10);
    }

    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      if(thisNote <= 10) {
        left_arm_servo.write(servo_position);
        right_arm_servo.write(270 - servo_position);
        servo_position -= 6;
        steppermotor.step(StepsRequired / (2 * 11));
      } else {
        left_arm_servo.write(servo_position);
        right_arm_servo.write(270 - servo_position);
        servo_position += 4;
        steppermotor.step( - StepsRequired / (2 * 15));
      }
      int noteDuration = 1000 / tempo[thisNote];

      buzz(melodyPin, melody[thisNote], noteDuration);

      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      buzz(melodyPin, 0, noteDuration);

    }

  } else {
    for(int thisLed = 0; thisLed < ledCount; thisLed++) {
      digitalWrite(ledPins[thisLed], LOW);
    }
  }
  delay(350);
}

void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000 / frequency / 2;
  long numCycles = frequency * length / 1000;
  for (long i = 0; i < numCycles; i++) { 
    digitalWrite(targetPin, HIGH); 
    delayMicroseconds(delayValue); 
    digitalWrite(targetPin, LOW); 
    delayMicroseconds(delayValue); 
  }

}
