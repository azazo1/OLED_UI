//
// Created by azazo1 on 2024/7/6.
//

#ifndef TEXTSELECTOR_H
#define TEXTSELECTOR_H
#include "Frame.h"

namespace view {
    class TextSelector final : public View {
        static constexpr int16_t PADDING = 3;
        static constexpr sche::mtime_t ANIM_DURATION = 100;

        std::vector<String> items;
        std::vector<int16_t> relativeYs;
        size_t selected = 0;

        // 文字高亮矩形.
        // h 参数是固定的 lineHeight.
        // relX 参数就是 PADDING.
        int16_t relRectY = 0;
        int16_t rectWidth = 0; // 需要贴合文字长度.

        std::function<void(const String &)> onChangeListener;
        std::function<void(const String &)> onConfirmListener;

        bool animating = false;

        /**
         * 将所有文字项移动一定的距离.
         * @param deltaY 移动有向距离.
         * @param scheduler Scheduler 对象.
         */
        void animMoveItems(int16_t deltaY, sche::Scheduler &scheduler);

        /**
         * 改变矩形的位置和宽度.
         * @param targetY 目标位置的相对 Y 值.
         * @param targetWidth 目标的矩形宽度.
         * @param scheduler Scheduler 对象.
         */
        void animMoveRect(int16_t targetY, int16_t targetWidth, sche::Scheduler &scheduler);

    public:
        /**
         * 添加可选项.
         */
        void addItem(String item);

        /**
         * 清空可选项.
         */
        void clearItems();

        // 请确保在 TextSelector 显示之前配置完 items, 否则可能遇到运行时错误.

        void onDraw(int16_t borderX, int16_t borderY, int16_t borderW, int16_t borderH,
                    SSD1306Wire *display, sche::Scheduler *scheduler) override;

        bool dispatchEvent(const event::Event &event) override;

        void setOnChangeListener(std::function<void(const String &)> listener);

        void setOnConfirmListener(std::function<void (const String &)> listener);

        /**
         * 获取当前选择的文本.
         */
        const String &current() const;
    };
} // view

#endif //TEXTSELECTOR_H
