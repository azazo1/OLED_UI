//
// Created by azazo1 on 2024/7/7.
//

#ifndef SEEKBAR_H
#define SEEKBAR_H
#include "View.h"

namespace view {
    class Seekbar final : public View {
        /// 水平 PADDING
        static constexpr int16_t PADDING = 3;
        /// 拖动条的粗细.
        static constexpr int16_t BAR_THICK = 5;
        int16_t current = 0;
        int16_t maximum = 100;
        int16_t minimum = 0;
        int16_t step = 1;
        std::function<void(int16_t)> onChangeListener = [](int16_t) {
        };
        std::function<void(int16_t)> onConfirmListener = [](int16_t) {
        };
        bool centering = true;

    public:
        /**
         * 创建一个可拖动的条, 使用旋钮事件拖动.
         * 短按按钮触发确认回调函数.
         */
        Seekbar() = default;

        /**
         * 设置视图是否垂直居中.
         * @note 此 UI 偏向于一个 Frame 只占有一个视图,
         *       如果要显示列表则使用 TextSelector 加二级菜单的形式,
         *       此 UI 的非 Frame 视图默认占一个 lineHeight 的高度,
         *       默认占有整个行宽度.
         */
        void setCentering(bool center);

        /**
         * 不会检查 maximum 大于等于 minimum, 需要用户自己保证.
         */
        void setMax(int16_t maximum);

        /**
         * \copydoc setMax(int16_t)
         */
        void setMin(int16_t minimum);

        /**
         * 设置步长大小, 即旋钮事件的一格将会移动多大的步长.
         * @param step 步长大小, 只支持非负数.
         */
        void setStep(int16_t step);

        void onDraw(int16_t borderX, int16_t borderY, int16_t borderW, int16_t borderH,
                    SSD1306Wire *display, sche::Scheduler *scheduler) override;

        void setOnChangeListener(std::function<void(int16_t)> listener);

        void setOnConfirmListener(std::function<void(int16_t)> listener);

        bool dispatchEvent(const event::Event &event) override;
    };
} // view

#endif //SEEKBAR_H
