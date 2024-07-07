//
// Created by azazo1 on 2024/7/7.
//

#include "Seekbar.h"

#include <event/ButtonEvent.h>
#include <event/KnobEvent.h>

namespace view {
    void Seekbar::setMax(const int16_t maximum) {
        this->maximum = maximum;
    }

    void Seekbar::setMin(const int16_t minimum) {
        this->minimum = minimum;
    }

    void Seekbar::setStep(const int16_t step) {
        if (step >= 0) {
            this->step = step;
        }
    }

    void Seekbar::setOnChangeListener(std::function<void(int16_t)> listener) {
        this->onChangeListener = std::move(listener);
    }

    void Seekbar::setOnConfirmListener(std::function<void(int16_t)> listener) {
        this->onConfirmListener = std::move(listener);
    }

    bool Seekbar::dispatchEvent(const event::Event &event) {
        if (event.getType() == EVENT_TYPE_KNOB) {
            current = static_cast<int16_t>(current + event.getPrimaryValue() * step);
            if (current > maximum) {
                current = maximum;
            }
            if (current < minimum) {
                current = minimum;
            }
            onChangeListener(current);
            return true;
        }
        if (event.getType() == EVENT_TYPE_BUTTON) {
            if (!static_cast<const event::ButtonEvent *>(&event)->isLongClick()) {
                // 不是长按.
                onConfirmListener(current);
                return true;
            }
        }
        return false;
    }

    void Seekbar::onDraw(int16_t borderX, int16_t borderY, int16_t borderW, int16_t borderH,
                         SSD1306Wire *display, sche::Scheduler *scheduler) {
        if (!alive) {
            return;
        }
        if (centering) {
            borderY = static_cast<int16_t>(borderY + borderH * 0.5 - lineHeight * 0.5);
        }
        borderH = lineHeight; // 一定要在 borderY 设置之后.
        borderX = static_cast<int16_t>(PADDING + borderX);
        borderW = static_cast<int16_t>(borderW - 2 * PADDING);

        const double ratio = 1.0 * (current - minimum) / (maximum - minimum);
        const auto x = static_cast<int16_t>(ratio * borderW - 0.5 * BAR_THICK + borderX);
        // 当前进度的短柱矩形, 高度为 borderH, 宽度为 BAR_THICK.
        display->fillRect(x, borderY, BAR_THICK, borderH);
        // 左半边实心进度.
        const auto y = static_cast<int16_t>(borderY + 0.5 * borderH - 0.5 * BAR_THICK);
        display->fillRect(borderX, y, x, BAR_THICK);
        // 右半边空心进度.
        display->fillRect(x, y, static_cast<int16_t>(borderX + borderW - x), BAR_THICK);
    }

    void Seekbar::setCentering(const bool center) {
        centering = center;
    }
} // view
