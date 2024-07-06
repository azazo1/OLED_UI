//
// Created by azazo1 on 2024/7/6.
//

#ifndef LABELEDFRAME_H
#define LABELEDFRAME_H
#include "Frame.h"

namespace view {
    class LabeledFrame final : public Frame {
        String title;
        static constexpr int16_t PADDING = 3;

    public:
        void setTitle(String title);

        void onDraw(int16_t borderX, int16_t borderY, int16_t borderW, int16_t borderH,
                    SSD1306Wire *display, sche::Scheduler *scheduler) override;
    };
} // view

#endif //LABELEDFRAME_H
