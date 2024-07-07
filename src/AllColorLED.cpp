//
// Created by azazo1 on 2024/7/7.
//

#include "AllColorLED.h"

#include <esp32-hal.h>

namespace acled {
    AllColorLED::AllColorLED(const int rPin, const int gPin, const int bPin)
        : rPin(rPin), gPin(gPin), bPin(bPin) {
        pinMode(rPin, OUTPUT);
        pinMode(gPin, OUTPUT);
        pinMode(bPin, OUTPUT);
        setRGB(0, 0, 0);
    }

    void AllColorLED::setR(const unsigned char r) const {
        analogWrite(rPin, r);
    }

    void AllColorLED::setG(const unsigned char g) const {
        analogWrite(gPin, g);
    }

    void AllColorLED::setB(const unsigned char b) const {
        analogWrite(bPin, b);
    }

    void AllColorLED::setRGB(const unsigned char r, const unsigned char g,
                             const unsigned char b) const {
        analogWrite(rPin, r);
        analogWrite(gPin, g);
        analogWrite(bPin, b);
    }

    AllColorLED::~AllColorLED() {
        pinMode(rPin, INPUT);
        pinMode(gPin, INPUT);
        pinMode(bPin, INPUT);
    }
} // acled
