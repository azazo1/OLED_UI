//
// Created by azazo1 on 2024/9/6.
//

#include "TextInput.h"

#include <event/ButtonEvent.h>
#include <event/KnobEvent.h>
#include <sche/DelaySchedulable.h>
#include <sche/ScalaTransition.h>
#include <sche/SchedulableFromLambda.h>
#include <sys/stat.h>

#include "Screen.h"

namespace view {
    void TextInput::onDraw(const int16_t borderX, const int16_t borderY, const int16_t borderW,
                           const int16_t borderH,
                           SSD1306Wire *display, sche::Scheduler *scheduler) {
        if (!alive) {
            return;
        }
        width = borderW;
        height = borderH;
        if (!registeredCursorLoop) {
            scheduler->addSchedule(new sche::DelaySchedulable(
                0, cursorFlickeringInterval, new sche::SchedulableFromLambda([this](sche::mtime_t) {
                    showCursor_ = !showCursor_;
                    return alive;
                })));
            registeredCursorLoop = true;
        }
        display->drawRect(borderX, borderY, borderW, borderH);
        display->drawString(static_cast<int16_t>(borderX + PADDING), borderY, text);
        display->drawHorizontalLine(borderX, static_cast<int16_t>(borderY + lineHeight), borderW);
        if (showCursor_) {
            const auto x = static_cast<int16_t>(display->getStringWidth(text) + PADDING);
            display->drawVerticalLine(static_cast<int16_t>(borderX + x),
                                      static_cast<int16_t>(borderY + PADDING),
                                      static_cast<int16_t>(lineHeight - 2 * PADDING));
        }
        // 显示选项.
        if (itemWidth == 0) {
            itemWidth = static_cast<int16_t>(max(display->getStringWidth(SPACE_STR),
                                                 display->getStringWidth(BACKSPACE_STR))
            );
        }
        innerBoxHeight = static_cast<int16_t>(borderH - lineHeight);
        innerBoxWidth = static_cast<int16_t>(borderW - 2 * PADDING);
        innerBoxX = static_cast<int16_t>(borderX + PADDING);
        innerBoxY = static_cast<int16_t>(borderY + lineHeight);
        // 确定显示列数.
        column = static_cast<int16_t>(innerBoxWidth / itemWidth);

        auto y = static_cast<int16_t>(relativeY_ + innerBoxY);
        int16_t showingCharIdx = 0;
        // 过滤掉在显示区域上侧的选项, 因为它们显示不完全, 不好看.
        while (y < innerBoxY) {
            showingCharIdx = static_cast<int16_t>(showingCharIdx + column);
            y = static_cast<int16_t>(y + lineHeight);
        }
        // 只显示出显示区域内的选项.
        while (y + lineHeight <= innerBoxY + innerBoxHeight) {
            int16_t x = innerBoxX;
            for (int i = 0; i < column && showingCharIdx < ITEM_NUM; ++i, ++showingCharIdx) {
                // 显示一行的选项.
                if (items[showingCharIdx] == '\b') {
                    display->drawString(x, y, BACKSPACE_STR);
                } else if (items[showingCharIdx] == ' ') {
                    display->drawString(x, y, SPACE_STR);
                } else {
                    display->drawString(x, y, String(items[showingCharIdx]));
                }
                x = static_cast<int16_t>(x + itemWidth);
            }
            y = static_cast<int16_t>(y + lineHeight);
        }

        // 绘制高亮矩形.
        if (selected == -1) {
            // 说明是第一次调用.
            performSelectionChange(0, *display, *scheduler);
        }
        display->drawRect(rectX_, rectY_, rectWidth_, rectHeight);
    }

    String TextInput::getText() const {
        return text;
    }

    void TextInput::setText(String text) {
        this->text = std::move(text);
    }

    bool TextInput::dispatchEvent(const event::Event &event) {
        switch (event.getType()) {
            case EVENT_TYPE_BUTTON: {
                if (!static_cast<const event::ButtonEvent *>(&event)->isLongClick()) {
                    // 消耗短按事件, 为了增强回馈, 使光标立刻显示.
                    showCursor_ = true;
                    const char append = items[selected];
                    if (append == '\b') {
                        if (!text.isEmpty()) {
                            text.remove(text.length() - 1);
                        }
                    } else {
                        text += append;
                    }
                    return true;
                }
                onDone(text);
                return false; // 不消耗事件, 只是触发回调函数.
            }
            case EVENT_TYPE_KNOB: {
                const int delta = event.getPrimaryValue();
                performSelectionChange(static_cast<int16_t>(selected + delta),
                                       event.getScreen().getDisplay(),
                                       event.getScreen().getScheduler());
                return true;
            }
            default: ;
        }
        return false;
    }

    void TextInput::performSelectionChange(int16_t newSelected, SSD1306Wire &display,
                                           sche::Scheduler &scheduler) {
        if (newSelected < 0) {
            newSelected = 0;
        } else if (newSelected > ITEM_NUM - 1) {
            newSelected = ITEM_NUM - 1;
        }
        const auto currentRow = static_cast<int16_t>(selected / column);
        const auto nextRow = static_cast<int16_t>(newSelected / column);
        const auto nextColumn = static_cast<int16_t>(newSelected % column);
        const int rowDelta = nextRow - currentRow; // 切换的行数.
        int16_t itemActualWidth; // 选项实际宽度, 和占位宽度不同.
        switch (items[newSelected]) {
            case '\b':
                itemActualWidth = static_cast<int16_t>(
                    display.getStringWidth(BACKSPACE_STR)
                );
                break;
            case ' ':
                itemActualWidth = static_cast<int16_t>(
                    display.getStringWidth(SPACE_STR)
                );
                break;
            default:
                itemActualWidth = static_cast<int16_t>(
                    display.getStringWidth(String(items[newSelected]))
                );
        }
        // 矩形左侧要移动到的 x 值.
        const auto nextX = static_cast<int16_t>(innerBoxX + nextColumn * itemWidth - PADDING / 2);
        // 矩形要变成的 width.
        const auto nextWidth = static_cast<int16_t>(itemActualWidth + 2 * PADDING / 2);
        // 如果不考虑边界情况, 矩形顶部要移动到的 y 值.
        const auto nextY = static_cast<int16_t>(innerBoxY + relativeYTarget + lineHeight * nextRow);
        animBatch++;
        if (rowDelta == 0) {
            // 如果所处的行一致, 不对选项的 y 值做动画.
            animMoveItem(0, scheduler); // 防止原来的动画被中断无法显示完全.
            animMoveRect(nextX, nextY, nextWidth, scheduler);
        } else {
            // 所处行不一致.
            // 当前的矩形动画结束后顶部 y 值.
            const int currentY = innerBoxY + relativeYTarget + lineHeight * currentRow;
            // 显示区域底部 y 值.
            const int bottomY = innerBoxHeight + innerBoxY;
            if (rowDelta > 0) {
                // 如果是向下滚动.
                if (bottomY - (currentY + lineHeight) < rowDelta * lineHeight) {
                    // 矩形底部距离显示区域底部不足一行.
                    // 矩形贴底, 选项贴底.
                    animMoveRect(nextX, static_cast<int16_t>(bottomY - lineHeight),
                                 nextWidth, scheduler);
                    animMoveItem(static_cast<int16_t>(bottomY - lineHeight - nextY),
                                 scheduler);
                } else {
                    // 空隙大于一行, 直接移动矩形.
                    animMoveItem(0, scheduler); // 防止原来的动画被中断无法显示完全.
                    animMoveRect(nextX, nextY, nextWidth, scheduler);
                }
            } else {
                // 向上滚动.
                const int absRowDelta = -rowDelta;
                if (currentY < innerBoxY + absRowDelta * lineHeight) {
                    // 矩形顶部距离显示区域顶部不足一行.
                    // 矩形贴顶, 选项贴顶.
                    animMoveRect(nextX, innerBoxY, nextWidth, scheduler);
                    animMoveItem(static_cast<int16_t>(innerBoxY - nextY), scheduler);
                } else {
                    // 空隙大于一行, 直接移动矩形.
                    animMoveItem(0, scheduler); // 防止原来的动画被中断无法显示完全.
                    animMoveRect(nextX, nextY, nextWidth, scheduler);
                }
            }
        }
        selected = newSelected;
    }

    void TextInput::animMoveItem(const int16_t deltaY, sche::Scheduler &scheduler) {
        const auto curBatch = animBatch;
        const auto newY = static_cast<int16_t>(relativeYTarget + deltaY);
        relativeYTarget = newY;
        scheduler.addSchedule(new sche::ScalaTransition(
            relativeY_,
            newY,
            ANIM_DURATION,
            nullptr,
            [this, curBatch](const double cur) {
                relativeY_ = static_cast<int16_t>(cur);
                return curBatch == animBatch && alive;
            }
        ));
    }

    void TextInput::animMoveRect(const int16_t x, const int16_t y, const int16_t width,
                                 sche::Scheduler &scheduler) {
        const auto curBatch = animBatch;
        scheduler.addSchedule(new sche::ScalaTransition(
            rectX_, x, ANIM_DURATION, nullptr,
            [this, curBatch](const double cur) {
                rectX_ = static_cast<int16_t>(cur);
                return animBatch == curBatch && alive;
            }
        ));
        scheduler.addSchedule(new sche::ScalaTransition(
            rectY_, y, ANIM_DURATION, nullptr,
            [this, curBatch](const double cur) {
                rectY_ = static_cast<int16_t>(cur);
                return animBatch == curBatch && alive;
            }
        ));
        scheduler.addSchedule(new sche::ScalaTransition(
            rectWidth_, width, ANIM_DURATION, nullptr,
            [this, curBatch](const double cur) {
                rectWidth_ = static_cast<int16_t>(cur);
                return animBatch == curBatch && alive;
            }
        ));
    }

    void TextInput::setOnDoneListener(std::function<void(String)> listener) {
        onDone = std::move(listener);
    }
} // view
