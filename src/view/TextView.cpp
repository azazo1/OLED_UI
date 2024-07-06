//
// Created by azazo1 on 2024/7/6.
//

#include "TextView.h"

namespace view {
    void TextView::setText(String text) {
        this->text = std::move(text);
    }

    void TextView::onDraw(const int16_t borderX, const int16_t borderY,
                          const int16_t borderW, const int16_t borderH,
                          SSD1306Wire *display, sche::Scheduler *scheduler) {
        display->drawString(borderX, borderY, text);
    }

    bool TextView::dispatchEvent(const event::Event &event) {
        // TextView 不接收事件.
        Serial.printf("%d %d", event.getType(), event.getPrimaryValue());
        return false;
    }
} // view
