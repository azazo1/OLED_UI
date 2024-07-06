//
// Created by azazo1 on 2024/7/6.
//

#include "SchedulableButtonEvent.h"

namespace sche {
    SchedulableButtonEvent::SchedulableButtonEvent(const int pin, std::function<bool(mtime_t)> onRelease)
        : btn(pin), onRelease(std::move(onRelease)) {
    }

    bool SchedulableButtonEvent::schedule(const mtime_t deltaTime) {
        const mtime_t pressedTime = btn.checkRoutine(deltaTime);
        if (pressedTime) {
            alive = onRelease(pressedTime);
        }
        return alive;
    }
} // sche
