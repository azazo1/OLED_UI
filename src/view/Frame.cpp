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
        if (!alive) {
            return;
        }
        for (View *ch: children) {
            ch->onDraw(borderX, borderY, borderW, borderH, display, scheduler);
        }
    }

    Frame::~Frame() {
        destroy();
        // 随后基类的析构函数也被调用.
    }

    void Frame::destroy() {
        for (const View *ch: children) {
            delete ch;
        }
        children.clear();
        View::destroy();
    }

    bool Frame::dispatchEvent(const event::Event &event) {
        if (!alive) {
            return false;
        }
        for (View *ch: children) {
            if (ch->dispatchEvent(event)) {
                return true;
            }
        }
        return false;
    }
} // view
