#ifndef ENCODER_H
#define ENCODER_H
#include "Adafruit_MotorShield_modified.h"

const long SPEEDSCALE = 256000L; // We need to keep the numerator and denominator similarly scaled.

class Encoder
{
public:

    // constructor : sets pins as inputs and turns on pullup resistors
    Encoder( int8_t PinA, int8_t PinB, Adafruit_DCMotor* assocMotor) : pin_a ( PinA), pin_b( PinB )
    {
        motor = assocMotor;
       
        // set pin a and b to be input 
        pinMode(pin_a, INPUT); 
        pinMode(pin_b, INPUT); 
       
        // and turn on pullup resistors
        digitalWrite(pin_a, HIGH);    
        digitalWrite(pin_b, HIGH);
    };

    
    // Call this from your interrupt function.
    void update()
    {
        noInterrupts();
        
        long now = micros();
        updateDelay = now - lastUpdateTime;
        lastUpdateTime = now;
        trueUpdateDelay = updateDelay;
        
        ticks++;
                
        if(
            (digitalRead(pin_a) && digitalRead(pin_b))
            ||
            (!digitalRead(pin_a) && !digitalRead(pin_b))
            )
            position--;
        else
            position++;
        
        interrupts();

    };
    
    float getSpeed()
    {
        uint8_t motorStatus = motor->getStatus();
        if(motorStatus==BRAKE || motorStatus==RELEASE)
        {
            lastSpeedQueryResult = 0;
            return 0;
        }
        long now = micros();
        if(now - lastSpeedQueryMicros < 100L)
        {
            return lastSpeedQueryResult;
        }
        else
        {
            long here = ticks;
            long there = lastSpeedQueryTicks;
//             long here = position;
//             long there = lastSpeedQueryPosition;
            
            float speed = SPEEDSCALE * (float) (here - there) / (float) (now - lastSpeedQueryMicros);
            
            if(motorStatus == BACKWARD)
                speed *= -1;
            lastSpeedQueryTicks = here;
            
//             lastSpeedQueryPosition = here;
            lastSpeedQueryMicros = now;
            lastSpeedQueryResult = speed;
            
            return speed;
        }

    }
    
    volatile long trueUpdateDelay;
    long volatile position;
    
private:
    Adafruit_DCMotor* motor;
    volatile long updateDelay;
    volatile long lastUpdateTime;
    float lastSpeedQueryResult;
    long lastSpeedQueryMicros;
//     long lastSpeedQueryPosition;
    long lastSpeedQueryTicks;
    long volatile ticks;
    int8_t pin_a;
    int8_t pin_b;
};

void interruptibleDelay(float ms);

#endif