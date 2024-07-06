//
// Created by azazo1 on 2024/7/6.
//

#include "Button.h"

#include <esp32-hal-gpio.h>

namespace btn {
    Button::Button(const int pin): pin(pin) {
        pinMode(pin, INPUT_PULLUP);
    }

    bool Button::pressed() const {
        return lastState;
    }

    unsigned long Button::checkRoutine(const unsigned long deltaTime) {
        accumulatedDeltaTime += deltaTime;
        unsigned long rst = 0;
        if (accumulatedDeltaTime > DETECT_INTERVAL) {
            const int p = !digitalRead(pin);
            if (p) {
                if (lastState) {
                    // 按钮按下且不是上升沿, 积累持续按下时间.
                    accumulatedPressedTime += accumulatedDeltaTime;
                }
                // 重新等待 Interval
                accumulatedDeltaTime = 0;
            } else {
                // 按钮没按下时, 不重置 accumulatedDeltaTime, 效果是对 button 的检测间隔更短.
                if (lastState) {
                    // 下降沿, 返回值.
                    rst = accumulatedPressedTime;
                }
                // 按钮没按下, 清空持续按下时间.
                accumulatedPressedTime = 0;
            }
            lastState = p;
        }
        return rst;
    }
} // btn
