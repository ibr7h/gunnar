#include <Servo.h>
#include <Wire.h>
#include <gunnar.h>
#include "constants.h"

Gunnar gunnar;

// Interrupt Service Routines
void doEncoder0()
{
    gunnar.encoder0.update();
}

void doEncoder1()
{
    gunnar.encoder1.update();
}

void setup()
{  
    Serial.begin(BAUDRATE);
    Serial.println("setup()");
    gunnar = Gunnar();
    gunnar.init();
    
    // Turn on pullup resistors on interrupt lines:
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    attachInterrupt(0, doEncoder0, CHANGE);
    attachInterrupt(1, doEncoder1, CHANGE);
}

void loop()
{
    gunnar.loop();
}
