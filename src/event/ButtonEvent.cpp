//
// Created by azazo1 on 2024/7/6.
//

#include "ButtonEvent.h"

namespace event {
    ButtonEvent::ButtonEvent(const int pressedDuration): duration(pressedDuration) {
    }

    int ButtonEvent::getType() const {
        return EVENT_TYPE_BUTTON;
    }

    int ButtonEvent::getPressedDuration() const {
        return duration;
    }

    int ButtonEvent::getPrimaryValue() const {
        return duration;
    }
} // event
