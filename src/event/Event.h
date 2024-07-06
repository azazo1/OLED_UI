//
// Created by azazo1 on 2024/7/6.
//

#ifndef EVENT_H
#define EVENT_H

namespace view {
    class Screen;
}

namespace event {
    class Event {
        view::Screen *screen;

    public:
        explicit Event(view::Screen &screen);

        virtual int getType() const = 0;

        virtual ~Event() = default;

        /**
         * 返回这个事件的主要值, 详见具体事件, 可以没有.
         */
        virtual int getPrimaryValue() const = 0;

        view::Screen &getScreen() const;
    };
} // event

#endif //EVENT_H
