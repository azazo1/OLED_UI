//
// Created by azazo1 on 2024/7/6.
//

#ifndef VIEW_H
#define VIEW_H
#include <cstdint>
#include <SSD1306Wire.h>
#include <sche/Scheduler.h>

#include "event/Event.h"

namespace view {
    class View {
    protected:
        static const uint8_t *font;
        static int16_t lineHeight;
        bool alive = true;

    public:
        /**
         * 需要额外调用 SSD1306Wire::setFont
         */
        static void setFont(const uint8_t *newFont);

        virtual ~View();

        virtual void onDraw(int16_t borderX, int16_t borderY, int16_t borderW, int16_t borderH,
                            SSD1306Wire *display, sche::Scheduler *scheduler) = 0;

        /**
         * 传递事件.
         * @param event 传递的事件.
         * @return 是否消耗了此事件.
         */
        virtual bool dispatchEvent(const event::Event &event) = 0;

        virtual void destroy();

        bool isAlive() const;
    };
} // view

#endif //VIEW_H
