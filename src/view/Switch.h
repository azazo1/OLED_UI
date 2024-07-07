//
// Created by azazo1 on 2024/7/7.
//

#ifndef SWITCH_H
#define SWITCH_H
#include "View.h"

namespace view {
    class Switch final : public View {
        static constexpr int16_t PADDING = 3;
        static constexpr int16_t SWITCH_OUTBOX_WIDTH = 20;
        static constexpr sche::mtime_t ANIM_DURATION = 100;
        /**
         * InBox 的宽度, 这里要注意外部 Box 和内部 Box也有 PADDING.
         * InBox 可用的空间大小其实是 (SWITCH_WIDTH - 2 * PADDING, lineHeight - 2 * PADDING).
         *
         * 设想是 InBox 在状态开时变成实心的且移动到 OutBox 内部右侧, 在状态关时变成空心的且移动到 OutBox 内部左侧.
         */
        static constexpr int16_t SWITCH_INBOX_WIDTH = 8;
        /**
         * InBox 相对外部 Box 的 x坐标(不计 PADDING).
         */
        int16_t relInX = PADDING;
        /**
         * 开关状态.
         */
        bool state = false;
        /**
         * InBox 的实心程度, 0 ~ 100.
         *
         * 实现的思路是使用随机数决定是否绘制像素, 同时在每次 onDraw 时重置随机种子.
         */
        int16_t solidRatio = 0;
        /// 由于开关按钮消耗了短按事件, 故没有 onConfirmListener.
        std::function<void(bool)> onChangeListener = [](bool) {
        };
        // 开关没有 onCancelListener, 因为想要结束编辑只能长按.

        String text;
        bool centering = true;
        size_t animBatch = 0;

        /**
         * 将 InBox 的位置立即调整到 state 对应的位置.
         */
        void resetAppearance();

    public:
        Switch(String text);

        Switch() = default;

        void onDraw(int16_t borderX, int16_t borderY, int16_t borderW, int16_t borderH,
                    SSD1306Wire *display, sche::Scheduler *scheduler) override;

        /// @see Seekbar::setCentering
        void setCentering(bool center);

        /**
         * 只消耗按钮短按事件.
         */
        bool dispatchEvent(const event::Event &event) override;

        void setOnChangeListener(std::function<void(bool)> listener);

        /**
         * 设置开关状态, 但是不会触发动画和 listener, 会调用 resetAppearance, 并禁用以前所有在此 View 上的动画.
         */
        void setState(bool newState);

        /**
         * 添加动画让 InBox 移动到 state 对应的位置, 同时显示 InBox 的实心空心过渡动画.
         */
        void animState(sche::Scheduler &scheduler);

        /**
         * 设置开关左边显示的文字.
         */
        void setText(String text);
    };
} // view

#endif //SWITCH_H
