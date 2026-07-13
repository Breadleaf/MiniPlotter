#ifndef CALIBRATED_SERVOS_HPP
#define CALIBRATED_SERVOS_HPP

#include <Arduino.h>
#include <Servo.h>

// #define SERVO_MIN_DEGREE 5
#define SERVO_MIN_DEGREE 0
// #define SERVO_MAX_DEGREE 150
#define SERVO_MAX_DEGREE 180

#define SERVO_DEFAULT_MIN_PWM 1000
#define SERVO_DEFAULT_MAX_PWM 2000

namespace Calibrations
{
    struct Profile
    {
        int minPulseWidth;
        int maxPulseWidth;
    };
    const Profile entries[] = {
        {.minPulseWidth = SERVO_DEFAULT_MIN_PWM - 400 + 65 + 00, .maxPulseWidth = SERVO_DEFAULT_MAX_PWM + 300 - 100 + 0}, // cbp#0
        {.minPulseWidth = SERVO_DEFAULT_MIN_PWM - 400 + 70 + 00, .maxPulseWidth = SERVO_DEFAULT_MAX_PWM + 300 - 150 + 0}, // cbp#1
        {.minPulseWidth = SERVO_DEFAULT_MIN_PWM - 400 + 130 + 0, .maxPulseWidth = SERVO_DEFAULT_MAX_PWM + 300 + 0000000}, // cbp#2
        {.minPulseWidth = SERVO_DEFAULT_MIN_PWM - 400 + 130 + 0, .maxPulseWidth = SERVO_DEFAULT_MAX_PWM + 300 - 10 + 00}, // cbp#3
        {.minPulseWidth = SERVO_DEFAULT_MIN_PWM - 400 + 30 + 00, .maxPulseWidth = SERVO_DEFAULT_MAX_PWM + 300 - 210 + 0}, // cbp#4
        {.minPulseWidth = SERVO_DEFAULT_MIN_PWM, .maxPulseWidth = SERVO_DEFAULT_MAX_PWM},                                 // non-calibrated profile
        {.minPulseWidth = SERVO_DEFAULT_MIN_PWM - 300, .maxPulseWidth = SERVO_DEFAULT_MAX_PWM + 300}                      // average profile
    };

} // namespace Calibrations

namespace CalibratedServos
{
    struct CalibratedServo
    {
        Servo servo;
        int minPulseWidth;
        int maxPulseWidth;
        uint8_t pin;

        // min and max should only be used when tuning a servo
        // all servo profiles contain the correct min and max ideally
        CalibratedServo(uint8_t pin, unsigned int profileNumber, int overrideMin = -1, int overrideMax = -1)
            : pin(pin)
        {
            minPulseWidth = (overrideMin == -1)
                                ? Calibrations::entries[profileNumber].minPulseWidth
                                : overrideMin;
            maxPulseWidth = (overrideMax == -1)
                                ? Calibrations::entries[profileNumber].maxPulseWidth
                                : overrideMax;
        }

        void begin()
        {
            servo.attach(pin);
        }

        void move(int degree)
        {
            // constrain input to force between SERVO_MIN_DEGREE <-> SERVO_MAX_DEGREE
            degree = constrain(degree, SERVO_MIN_DEGREE, SERVO_MAX_DEGREE);
            // map the range to whatever pulse width the specific servo needs
            int pulseWidth = map(
                degree,
                SERVO_MIN_DEGREE, SERVO_MAX_DEGREE,
                minPulseWidth, maxPulseWidth);
            // write to the servo
            servo.writeMicroseconds(pulseWidth);
        }
    };
} // namespace CalibratedServos

#endif // CALIBRATED_SERVOS_HPP