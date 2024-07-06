//
// Created by azazo1 on 2024/7/6.
//

#include "TextSelector.h"

#include <event/ButtonEvent.h>
#include <event/KnobEvent.h>
#include <sche/ScalaTransition.h>
#include <sche/SequenceSchedulable.h>

#include "Screen.h"

namespace view {
    void TextSelector::addItem(String item) {
        relativeYs.push_back(static_cast<int16_t>(lineHeight * items.size()));
        items.push_back(std::move(item));
    }

    void TextSelector::clearItems() {
        items.clear();
        relativeYs.clear();
    }


    void TextSelector::onDraw(const int16_t borderX, const int16_t borderY, const int16_t borderW,
                              const int16_t borderH,
                              SSD1306Wire *display, sche::Scheduler *scheduler) {
        if (!alive) {
            return;
        }
        const auto absX = static_cast<int16_t>(borderX + PADDING);
        for (size_t i = 0; i < items.size(); ++i) {
            const auto relY = relativeYs.at(i);
            // 绘制文字, 在屏幕外的跳过.
            if (relY < 0 || relY + lineHeight > borderH) {
                // 有一点超出都不显示.
                continue;
            }
            // 文字显示需要添加 PADDING.
            display->drawStringMaxWidth(absX,
                                        static_cast<int16_t>(relY + borderY),
                                        borderW, items.at(i));
        }
        // 绘制矩形.
        // 绘制从视图最左边开始, 在其他地方设置 rectWidth 时需要注意添加两倍的 PADDING(相对目标文字宽度).
        if (rectWidth == 0 && !items.empty()) {
            selected = 0;
            rectWidth = static_cast<int16_t>(display->getStringWidth(items.at(0)) + 2 * PADDING);
        }
        display->drawRect(borderX, static_cast<int16_t>(relRectY + borderY), rectWidth, lineHeight);
    }

    void TextSelector::setOnChangeListener(std::function<void(const String &)> listener) {
        onChangeListener = std::move(listener);
    }

    void TextSelector::setOnConfirmListener(std::function<void(const String &)> listener) {
        onConfirmListener = std::move(listener);
    }

    bool TextSelector::dispatchEvent(const event::Event &event) {
        if (!alive) {
            return false;
        }
        // todo 解决动画重叠问题.
        if (event.getType() == EVENT_TYPE_KNOB) {
            int delta = event.getPrimaryValue();
            const size_t size = items.size();
            while (delta < 0) {
                delta += static_cast<int>(size);
            }
            const int selectionDelta = static_cast<int>(
                (selected + delta) % size // newSelected, 这部分能保证是 [0, size - 1] 内.
                - selected);
            selected += selectionDelta;

            sche::Scheduler &scheduler = event.getScreen().getScheduler();
            SSD1306Wire &display = event.getScreen().getDisplay();

            // 文字过渡动画.
            animMoveItems(static_cast<int16_t>(-selectionDelta * lineHeight), scheduler);
            // 矩形过渡动画.
            const uint16_t expectedWidth = display.getStringWidth(items.at(selected));
            animMoveRect(0, static_cast<int16_t>(expectedWidth + 2 * PADDING), scheduler);
            return true;
        }
        if (event.getType() == EVENT_TYPE_BUTTON) {
            if (!static_cast<const event::ButtonEvent *>(&event)->isLongClick()) {
                onConfirmListener(items.at(selected));
                return true;
            }
        }
        return false;
    }

    void TextSelector::animMoveItems(const int16_t deltaY, sche::Scheduler &scheduler) {
        for (size_t i = 0; i < items.size(); ++i) {
            scheduler.addSchedule(new sche::ScalaTransition(
                relativeYs.at(i),
                static_cast<int16_t>(relativeYs.at(i) + deltaY),
                ANIM_DURATION,
                nullptr,
                [this, i](const int16_t cur) {
                    relativeYs.at(i) = cur;
                    return alive;
                }
            ));
        }
    }

    void TextSelector::animMoveRect(const int16_t targetY, const int16_t targetWidth,
                                    sche::Scheduler &scheduler) {
        scheduler.addSchedule(new sche::ScalaTransition(
            relRectY, targetY, ANIM_DURATION, nullptr,
            [this](const int16_t cur) {
                relRectY = cur;
                return true;
            }
        ));
        scheduler.addSchedule(new sche::ScalaTransition(
            rectWidth, targetWidth, ANIM_DURATION, nullptr,
            [this](const int16_t cur) {
                rectWidth = cur;
                return true;
            }
        ));
    }

    const String &TextSelector::current() const {
        return items.at(selected);
    }
} // view
