//
// Created by azazo1 on 2024/7/6.
//

#ifndef EVENT_H
#define EVENT_H

namespace event {
    class Event {
    public:
        virtual int getType() const = 0;

        virtual ~Event() = default;

        /**
         * 返回这个事件的主要值, 详见具体事件, 可以没有.
         */
        virtual int getPrimaryValue() const = 0;
    };
} // event

#endif //EVENT_H
