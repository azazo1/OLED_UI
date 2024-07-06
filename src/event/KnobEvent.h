//
// Created by azazo1 on 2024/7/6.
//

#ifndef KNOBEVENT_H
#define KNOBEVENT_H
#include <Knob.h>

#include "Event.h"
#define EVENT_TYPE_KNOB 1

namespace event {
    /**
     * 旋钮事件
     */
    class KnobEvent final : public Event {
        int delta;

    public:
        explicit KnobEvent(int delta);

        int getType() const override;

        int getDelta() const;

        int getPrimaryValue() const override;
    };
} // event

#endif //KNOBEVENT_H
