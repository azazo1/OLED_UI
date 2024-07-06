//
// Created by azazo1 on 2024/7/6.
//

#include "LabeledFrame.h"

namespace view {
    void LabeledFrame::setTitle(String title) {
        this->title = std::move(title);
    }

    void LabeledFrame::onDraw(const int16_t borderX, const int16_t borderY, const int16_t borderW,
                              const int16_t borderH,
                              SSD1306Wire *display, sche::Scheduler *scheduler) {
        display->setFont(font);
        display->drawRect(borderX, borderY, borderW, borderH);
        display->drawString(static_cast<int16_t>(borderX + PADDING), borderY, title);
        display->drawHorizontalLine(borderX, static_cast<int16_t>(borderY + lineHeight), borderW);
        if (borderH >= lineHeight) {
            for (View *ch: children) {
                ch->onDraw(static_cast<int16_t>(borderX + PADDING),
                           static_cast<int16_t>(borderY + lineHeight + PADDING),
                           static_cast<int16_t>(borderW - 2 * PADDING),
                           static_cast<int16_t>(borderH - lineHeight - 2 * PADDING),
                           display, scheduler);
            }
        }
    }
} // view
