//
// Created by azazo1 on 2024/7/7.
//

#include "Switch.h"

#include <event/ButtonEvent.h>
#include <sche/ScalaTransition.h>

#include "Screen.h"
#include "Arduino.h"

namespace view {
    Switch::Switch(String text): View(), text(std::move(text)) {
    }

    void Switch::onDraw(const int16_t borderX, int16_t borderY, const int16_t borderW,
                        const int16_t borderH,
                        SSD1306Wire *display, sche::Scheduler *scheduler) {
        if (!alive) {
            return;
        }
        if (centering) {
            borderY = static_cast<int16_t>(borderY + borderH * 0.5 - lineHeight * 0.5);
        }
        // 文字放左边, Box 放右边
        const auto textX = static_cast<int16_t>(borderX + PADDING);
        const auto textW = static_cast<int16_t>(borderW - 2 * PADDING - SWITCH_OUTBOX_WIDTH);
        const auto outBoxX = static_cast<int16_t>(
            borderX + borderW - SWITCH_OUTBOX_WIDTH - PADDING
        );
        const auto inBoxX = static_cast<int16_t>(outBoxX + relInX);
        const auto inBoxY = static_cast<int16_t>(borderY + PADDING);
        const auto inBoxH = static_cast<int16_t>(lineHeight - 2 * PADDING);

        // 绘制文字.
        display->drawStringMaxWidth(textX, borderY, textW, text);
        // 绘制 OutBox.
        display->drawRect(outBoxX, borderY, SWITCH_OUTBOX_WIDTH, lineHeight);
        // 绘制 InBox
        srand(42);
        const int inBoxRightX = inBoxX + SWITCH_INBOX_WIDTH;
        const int inBoxBottomY = inBoxH + inBoxY;
        display->drawRect(inBoxX, inBoxY, SWITCH_INBOX_WIDTH, inBoxH);
        for (int16_t x = inBoxX; x < inBoxRightX; ++x) {
            for (int16_t y = inBoxY; y < inBoxBottomY; ++y) {
                if (rand() % 100 < solidRatio) {
                    display->setPixel(x, y);
                }
            }
        }
    }

    void Switch::setCentering(const bool center) {
        centering = center;
    }

    void Switch::setText(String text) {
        this->text = std::move(text);
    }

    bool Switch::dispatchEvent(const event::Event &event) {
        if (event.getType() == EVENT_TYPE_BUTTON) {
            if (!static_cast<const event::ButtonEvent *>(&event)->isLongClick()) {
                state = !state;
                onChangeListener(state);
                animState(event.getScreen().getScheduler());
                return true;
            }
        }
        return false;
    }

    void Switch::setOnChangeListener(std::function<void(bool)> listener) {
        onChangeListener = std::move(listener);
    }

    void Switch::setState(const bool newState) {
        state = newState;
        resetAppearance();
    }

    void Switch::animState(sche::Scheduler &scheduler) {
        auto curBatch = animBatch;
        int16_t newPos = PADDING;
        if (state) {
            newPos = static_cast<int16_t>(SWITCH_OUTBOX_WIDTH - PADDING - SWITCH_INBOX_WIDTH);
        }
        scheduler.addSchedule(new sche::ScalaTransition(
            relInX, newPos, ANIM_DURATION, nullptr,
            [this, curBatch](const double cur) {
                relInX = static_cast<int16_t>(cur);
                return curBatch == animBatch;
            }
        ));
        scheduler.addSchedule(new sche::ScalaTransition(
            solidRatio, state ? 100 : 0, ANIM_DURATION, nullptr,
            [this, curBatch](const double cur) {
                solidRatio = cur;
                return curBatch == animBatch;
            }
        ));
    }

    void Switch::resetAppearance() {
        animBatch++;
        if (state) {
            relInX = static_cast<int16_t>(SWITCH_OUTBOX_WIDTH - PADDING - SWITCH_INBOX_WIDTH);
            solidRatio = 100;
        } else {
            relInX = PADDING;
            solidRatio = 0;
        }
    }
} // view
