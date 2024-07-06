//
// Created by azazo1 on 2024/7/6.
//

#include "KnobEvent.h"

namespace event {
    KnobEvent::KnobEvent(view::Screen &screen, const int delta): Event(screen), delta(delta) {
    }

    int KnobEvent::getType() const {
        return EVENT_TYPE_KNOB;
    }

    int KnobEvent::getDelta() const {
        return delta;
    }

    int KnobEvent::getPrimaryValue() const {
        return delta;
    }
} // event
