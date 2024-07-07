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
        items.push_back(std::move(item));
    }

    void TextSelector::clearItems() {
        items.clear();
    }

    const String &TextSelector::itemAt(const size_t idx) const {
        return items.at(idx);
    }


    void TextSelector::onDraw(const int16_t borderX, const int16_t borderY, const int16_t borderW,
                              const int16_t borderH,
                              SSD1306Wire *display, sche::Scheduler *scheduler) {
        if (!alive) {
            return;
        }
        width = borderW;
        height = borderH;
        const auto absX = static_cast<int16_t>(borderX + PADDING);
        for (size_t i = 0; i < items.size(); ++i) {
            const auto relY = static_cast<int16_t>(relativeY + lineHeight * i);
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

    void TextSelector::setOnChangeListener(std::function<void(size_t)> listener) {
        onChangeListener = std::move(listener);
    }

    void TextSelector::setOnConfirmListener(std::function<void(size_t)> listener) {
        onConfirmListener = std::move(listener);
    }

    bool TextSelector::dispatchEvent(const event::Event &event) {
        if (!alive) {
            return false;
        }
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

            animBatch++;
            // 自动选择要动的是高亮矩形还是文字, 还是两个.
            animSelectionChange(selectionDelta, display, scheduler);
            onChangeListener(selected);
            return true;
        }
        if (event.getType() == EVENT_TYPE_BUTTON) {
            if (!static_cast<const event::ButtonEvent *>(&event)->isLongClick()) {
                onConfirmListener(selected);
                return true;
            }
        }
        return false;
    }

    void TextSelector::animSelectionChange(const int selectionDelta, SSD1306Wire &display,
                                           sche::Scheduler &scheduler) {
        auto curBatch = animBatch;
        // currentY 是现在矩形的位置(之前的动画结束后, 上部).
        const int currentY = relativeYTarget + static_cast<int16_t>(selected - selectionDelta) *
                             lineHeight;
        // nextY 是要切换到的文字的位置(之前的动画结束后, 上部).
        const int nextY = relativeYTarget + static_cast<int16_t>(selected) * lineHeight;
        const auto expectedWidth = static_cast<int16_t>(display.getStringWidth(items.at(selected))
                                                        + 2 * PADDING);
        if (selectionDelta > 0) {
            // 向下滚动, 文字可能向上移动, 矩形可能向下移动.
            // height 绝对会比 y 大.
            if (height - currentY > (1 + selectionDelta) * lineHeight) {
                // 矩形底部还有可以放置 delta 个矩形的空间,
                // 直接变换矩形而不改变文字位置.
                animMoveRect(static_cast<int16_t>(currentY + lineHeight * selectionDelta),
                             expectedWidth, scheduler);
                animMoveItems(0, scheduler); // 防止原来那个被取消导致动画不完全.
                // Serial.printf("1");
            } else {
                const int bottomY = height - lineHeight;
                // 不够矩形向下移动 delta 行高度, 那么就让矩形贴底.
                animMoveRect(static_cast<int16_t>(bottomY), expectedWidth, scheduler);
                // 然后把目标的文字也贴底.
                animMoveItems(static_cast<int16_t>(bottomY - nextY), scheduler);
                // Serial.printf("2");
            }
        } else if (selectionDelta < 0) {
            // 这里需要特别注意 delta 是负数.
            // 向上滚动, 文字可能向下移动, 矩形可能向上移动.
            // currentY 一定是非负数.
            if (currentY > lineHeight * -selectionDelta) {
                // 矩形可以向上移动 delta 行.
                animMoveRect(static_cast<int16_t>(currentY + lineHeight * selectionDelta),
                             expectedWidth, scheduler);
                animMoveItems(0, scheduler); // 防止原来那个被取消导致动画不完全.
                // Serial.printf("3");
            } else {
                // 矩形不能向上移动 delta 行.
                // 矩形和目标文字贴底.
                animMoveItems(static_cast<int16_t>(-nextY), scheduler);
                animMoveRect(0, expectedWidth, scheduler);
                // Serial.printf("4");
            }
        }
        // Serial.printf(" %d %d %d %d\n", selectionDelta, selected, relativeY, relativeYTarget);
    }

    void TextSelector::animMoveItems(const int16_t deltaY, sche::Scheduler &scheduler) {
        auto curBatch = animBatch;
        const auto newY = static_cast<int16_t>(relativeYTarget + deltaY);
        relativeYTarget = newY;
        scheduler.addSchedule(new sche::ScalaTransition(
            relativeY,
            newY,
            ANIM_DURATION,
            nullptr,
            [this, curBatch](const int16_t cur) {
                relativeY = cur;
                return curBatch == animBatch;
            }
        ));
    }

    void TextSelector::animMoveRect(const int16_t targetY, const int16_t targetWidth,
                                    sche::Scheduler &scheduler) {
        auto curBatch = animBatch;
        scheduler.addSchedule(new sche::ScalaTransition(
            relRectY, targetY, ANIM_DURATION, nullptr,
            [this, curBatch](const int16_t cur) {
                relRectY = cur;
                return curBatch == animBatch;
            }
        ));
        scheduler.addSchedule(new sche::ScalaTransition(
            rectWidth, targetWidth, ANIM_DURATION, nullptr,
            [this, curBatch](const int16_t cur) {
                rectWidth = cur;
                return curBatch == animBatch;
            }
        ));
    }

    const String &TextSelector::current() const {
        return items.at(selected);
    }
} // view
