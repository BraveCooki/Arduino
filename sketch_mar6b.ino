/****************************************************************

Hardware Connections:

IMPORTANT: The APDS-9960 can only accept 3.3V!
 
 Arduino Pin  APDS-9960 Board  Function
 
 3.3V         VCC              Power
 GND          GND              Ground
 A4           SDA              I2C Data
 A5           SCL              I2C Clock
 2            INT              Interrupt

****************************************************************/

#include <Wire.h>
#include <SparkFun_APDS9960.h>

#define APDS9960_INT    2 // Needs to be an interrupt pin

SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;

void setup() {
  pinMode(8, OUTPUT);
  pinMode(10, OUTPUT);

  pinMode(APDS9960_INT, INPUT);

  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));
  
  attachInterrupt(0, interruptRoutine, FALLING);

  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
}

void loop() {
  if( isr_flag == 1 ) {
    detachInterrupt(0);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
}

void interruptRoutine() {
  isr_flag = 1;
}

void handleGesture() {
    if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {
      case DIR_UP:
        digitalWrite(8, HIGH);
        Serial.println("UP");
        break;
      case DIR_DOWN:
        digitalWrite(8, LOW);
        Serial.println("DOWN");
        break;
      case DIR_LEFT:
        digitalWrite(10, HIGH);
        Serial.println("LEFT");
        break;
      case DIR_RIGHT:
        digitalWrite(10, LOW);
        Serial.println("RIGHT");
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
        digitalWrite(11, HIGH);
        break;
      case DIR_FAR:
        Serial.println("FAR");
        digitalWrite(11, LOW);
        break;  
      default:
        Serial.println("The input given is invalid please try again");

    }
  }
}