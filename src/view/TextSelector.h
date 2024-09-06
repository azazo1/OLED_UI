//
// Created by azazo1 on 2024/7/6.
//

#ifndef TEXTSELECTOR_H
#define TEXTSELECTOR_H
#include "Frame.h"

// todo 使用中间动画视图来实现视图间的过渡.
namespace view {
    class TextSelector final : public View {
        static constexpr int16_t PADDING = 3;
        static constexpr sche::mtime_t ANIM_DURATION = 100;

        std::vector<String> items;
        /// 只存了第一个文本的 Y 值, 其他的可以自动计算.
        int16_t relativeY = 0;
        /// relativeY 中的值的趋向(relativeY 在动画结束后就和 relativeYTarget 相等).
        int16_t relativeYTarget = 0;
        size_t selected = 0;
        /// 是否在在末尾向后选时回到开头(反过来同理).
        // bool loop = true;
        bool loop = false;

        // 文字高亮矩形.
        // h 参数是固定的 lineHeight.
        // relX 参数就是 PADDING.
        int16_t relRectY = 0;
        int16_t rectWidth = 0; // 需要贴合文字长度.

        // 回调参数接受字符串所在的索引位置.
        std::function<void(size_t)> onChangeListener = [](size_t) {
        };
        std::function<void(size_t)> onConfirmListener = [](size_t) {
        };

        size_t animBatch = 0;

        // 记录调用 onDraw 时的 borderW 和 borderH.
        int16_t width = 0;
        int16_t height = 0;

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

        /**
         * 显示选项变化的动画, 自动协调高亮矩形和文字位置.
         */
        void animSelectionChange(int selectionDelta, SSD1306Wire &display,
                                 sche::Scheduler &scheduler);

    public:
        /**
         * 添加可选项.
         */
        void addItem(String item);

        /**
         * @see TextSelector::loop
         */
        void setLooping(bool loop);

        /**
         * 清空可选项.
         */
        void clearItems();

        String &itemAt(size_t idx);

        // 请确保在 TextSelector 显示之前配置完 items, 否则可能遇到运行时错误.

        void onDraw(int16_t borderX, int16_t borderY, int16_t borderW, int16_t borderH,
                    SSD1306Wire *display, sche::Scheduler *scheduler) override;

        bool dispatchEvent(const event::Event &event) override;

        void setOnChangeListener(std::function<void(size_t)> listener);

        void setOnConfirmListener(std::function<void(size_t)> listener);

        /**
         * 获取当前选择的文本.
         */
        const String &current() const;
    };
} // view

#endif //TEXTSELECTOR_H
