//
// Created by azazo1 on 2024/7/6.
//

#include "TextView.h"

#include <event/ButtonEvent.h>
#include <sche/ScalaTransition.h>
#include <sche/SequenceSchedulable.h>

#include "Screen.h"

namespace view {
    TextView::TextView(String text): text(std::move(text)) {
    }

    void TextView::setText(String text) {
        this->text = std::move(text);
    }

    void TextView::onDraw(const int16_t borderX, const int16_t borderY,
                          const int16_t borderW, const int16_t borderH,
                          SSD1306Wire *display, sche::Scheduler *scheduler) {
        if (alive) {
            if (displayLength >= 0) {
                display->drawString(borderX, borderY, text.substring(0, displayLength));
            } else {
                display->drawStringMaxWidth(borderX, borderY, borderW, text);
            }
        }
    }

    bool TextView::dispatchEvent(const event::Event &event) {
        if (!alive) {
            return false;
        }
        if (event.getType() == EVENT_TYPE_BUTTON) {
            // 只在短按时有效.
            if (!static_cast<const event::ButtonEvent *>(&event)->isLongClick()) {
                // 字符逐渐出现动画.
                auto curBatch = ++aniBatch;
                event.getScreen().getScheduler().addSchedule(
                    (new sche::SequenceSchedulable())->then(
                        new sche::ScalaTransition(
                            0, this->text.length(), 500, nullptr,
                            [curBatch,this](const double current) {
                                displayLength = static_cast<int16_t>(current);
                                return curBatch == aniBatch;
                            }
                        )
                    )->then(
                        new sche::SchedulableFromLambda([this](sche::mtime_t) {
                            // 一次性.
                            displayLength = -1;
                            return false;
                        })
                    )
                );
                return true;
            }
        }
        return false;
    }
} // view
