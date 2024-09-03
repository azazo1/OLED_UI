//
// Created by azazo1 on 2024/9/3.
//

#include "SchedulableKnobEvent.h"

namespace sche {
    SchedulableKnobEvent::SchedulableKnobEvent(
        const int pinA, const int pinB, std::function<bool(int)> onRotation
    ): kb(pinA, pinB), onRotation(std::move(onRotation)) {
    }

    bool SchedulableKnobEvent::schedule(unsigned long deltaTime) {
        const int delta = kb.delta();
        if (delta != 0) {
            alive = onRotation(delta);
        }
        return alive;
    }
} // sche
