#include <Arduino.h>
#include <Servo.h>

#include "CalibratedServos.hpp"
namespace cs = CalibratedServos;

// external state

// servo state

const int numberServos = 5;

// with calibration data
cs::CalibratedServo servos[5] = {
    cs::CalibratedServo(3, 0, SERVO_DEFAULT_MIN_PWM - 400 + 65 + 00, SERVO_DEFAULT_MAX_PWM + 300 - 100),
    cs::CalibratedServo(5, 1, SERVO_DEFAULT_MIN_PWM - 400 + 70 + 00, SERVO_DEFAULT_MAX_PWM + 300 - 150),
    cs::CalibratedServo(6, 2, SERVO_DEFAULT_MIN_PWM - 400 + 130 + 0, SERVO_DEFAULT_MAX_PWM + 300),
    cs::CalibratedServo(9, 3, SERVO_DEFAULT_MIN_PWM - 400 + 130 + 0, SERVO_DEFAULT_MAX_PWM + 300 - 10),
    cs::CalibratedServo(10, 4, SERVO_DEFAULT_MIN_PWM - 400 + 30 + 0, SERVO_DEFAULT_MAX_PWM + 300 - 210),
};

// helper definitions

// run a command on every servo
#define FOR_SERVO(command)                 \
    for (int i = 0; i < numberServos; i++) \
        command;

// custom types

typedef unsigned long TimeMilliSec_t;

// https://forum.arduino.cc/t/pointers-to-function/58518/2
typedef void (*SchedulableFunction)(TimeMilliSec_t);

void setup()
{
    Serial.begin(115200);
    delay(2000);

    FOR_SERVO(servos[i].begin());
    delay(100);
}

// scheduler handling

// schedules
void RunServos(TimeMilliSec_t);

// setup schedule
SchedulableFunction schedule[] = {
    RunServos,
    nullptr,
};

// variables
TimeMilliSec_t currentTime = 0;

void loop()
{
    for (
        unsigned long scheduleIndex = 0;
        schedule[scheduleIndex] != nullptr;
        scheduleIndex++)
    {
        currentTime = millis();
        schedule[scheduleIndex](currentTime);
    }
}

// servo handling

enum ServoState_t
{
    zero,
    ninety,
    oneEighty
};

ServoState_t servoState = zero;
TimeMilliSec_t servoUpdateInterval = 500;
TimeMilliSec_t servoLastUpdate = 0;

void RunServos(TimeMilliSec_t currentTime)
{
    if (currentTime - servoLastUpdate >= servoUpdateInterval)
    {
        switch (servoState)
        {
        case zero:
            FOR_SERVO(servos[i].move(90));
            servoState = ninety;
            break;
        case ninety:
            FOR_SERVO(servos[i].move(180));
            servoState = oneEighty;
            break;
        case oneEighty:
            FOR_SERVO(servos[i].move(0));
            servoState = zero;
            break;
        default:
            Serial.println("[-] Invalid Servo State -- Defaulting to zero!");
        }
        servoLastUpdate = currentTime;
    }
}