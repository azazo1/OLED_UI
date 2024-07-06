//
// Created by azazo1 on 2024/7/6.
//

#ifndef TEXTVIEW_H
#define TEXTVIEW_H
#include "View.h"

namespace view {
    class TextView final : public View {
        String text;

    public:
        void setText(String text);

        void onDraw(int16_t borderX, int16_t borderY, int16_t borderW, int16_t borderH,
                    SSD1306Wire *display, sche::Scheduler *scheduler) override;

        bool dispatchEvent(const event::Event &event) override;
    };
} // view

#endif //TEXTVIEW_H
