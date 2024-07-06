//
// Created by azazo1 on 2024/7/6.
//

#include "Frame.h"

namespace view {
    void Frame::addChild(View *view) {
        children.push_back(view);
    }

    void Frame::onDraw(const int16_t borderX, const int16_t borderY, const int16_t borderW,
                       const int16_t borderH,
                       SSD1306Wire *display, sche::Scheduler *scheduler) {
        for (View *ch: children) {
            ch->onDraw(borderX, borderY, borderW, borderH, display, scheduler);
        }
    }

    Frame::~Frame() {
        for (const View *ch: children) {
            delete ch;
        }
        children.clear();
    }
} // view
