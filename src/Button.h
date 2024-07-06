//
// Created by azazo1 on 2024/7/6.
//

#ifndef BUTTON_H
#define BUTTON_H

namespace btn {
    constexpr int DETECT_INTERVAL = 20;

    class Button {
        const int pin;
        bool lastState = false;
        unsigned long accumulatedDeltaTime = 0;
        unsigned long accumulatedPressedTime = 0;

    public:
        /**
         * 创建 Button, 并设置端口模式.
         * @param pin 监听端口, 使用内部上拉来读取输入.
         */
        explicit Button(int pin);

        /**
         * 保持此方法的调用来不断检测按钮的状态.
         * @param deltaTime 毫秒, 距离上一次调用的时间.
         * @return == 0, 按钮没按下.
         *         > 0, 按钮按下时长(此时按钮已松开).
         */
        unsigned long checkRoutine(unsigned long deltaTime);

        /**
         * 获取按钮是否按下的状态.
         */
        bool pressed() const;
    };
} // btn

#endif //BUTTON_H
