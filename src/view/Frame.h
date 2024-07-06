//
// Created by azazo1 on 2024/7/6.
//

#ifndef FRAME_H
#define FRAME_H
#include <vector>

#include "View.h"

namespace view {
    class Frame : public View {
    protected:
        std::vector<View *> children;

    public:
        Frame() = default;

        /**
         * 添加子视图.
         * @param view 视图, 堆指针, 传入所有权.
         */
        void addChild(View *view);

        void onDraw(int16_t borderX, int16_t borderY, int16_t borderW, int16_t borderH,
                    SSD1306Wire *display, sche::Scheduler *scheduler) override;

        ~Frame() override;

        void destroy() override;

        bool dispatchEvent(const event::Event &event) override;
    };
} // view

#endif //FRAME_H
