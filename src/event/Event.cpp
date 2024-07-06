//
// Created by azazo1 on 2024/7/6.
//

#include "Event.h"

namespace event {
    Event::Event(view::Screen &screen): screen(&screen) {
    }

    view::Screen &Event::getScreen() const {
        return *screen;
    }
} // event
