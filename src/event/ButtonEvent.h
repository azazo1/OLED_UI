//
// Created by azazo1 on 2024/7/6.
//

#ifndef BUTTONEVENT_H
#define BUTTONEVENT_H
#include "Event.h"

#define EVENT_TYPE_BUTTON 2

namespace event {
    class ButtonEvent final : public Event {
        int duration;

    public:
        explicit ButtonEvent(int pressedDuration);

        int getType() const override;

        int getPressedDuration() const;

        int getPrimaryValue() const override;
    };
} // event

#endif //BUTTONEVENT_H
