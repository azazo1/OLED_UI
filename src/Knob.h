//
// Created by azazo1 on 2024/7/5.
//

#ifndef KNOB_H
#define KNOB_H
#include <esp32-hal-timer.h>
#include <esp_attr.h>
#include <unordered_map>

typedef unsigned int id_t;

namespace knob {
    class Knob {
        static std::unordered_map<id_t, std::pair<int, int> > interruptMap;
        static std::unordered_map<id_t, int> deltaMap;
        static std::unordered_map<id_t, int> lastState;
        /// 用于在中断函数中辨别
        const id_t id;
        const int pinA;
        const int pinB;
        int accumulated = 0;

        static void IRAM_ATTR routine();

        static hw_timer_t *timer;

    public:
        /**
         * 创建一个旋钮对象, 不是使用外部中断而是使用定时器中断检测输入,
         * 只会使用一个定时器中断.
         * @note 不知道什么原因, 此初始化函数只能放在 setup 内初始化, 而不能在 setup 外作为全局变量初始化.
         * @param pinA 旋钮输入引脚 A
         * @param pinB 旋钮输入引脚 B
         */
        explicit Knob(int pinA, int pinB);

        /**
         * 获取旋钮扭动的格数, 有正有负有零.
         *
         * 当中断触发时, 如果 A 引脚先变化, 则 delta += 1,
         *             如果 B 引脚先变化, 则 delta -= 1.
         * 当没有变化时, delta 为 0.
         *
         * delta 值在每次 delta() 方法被调用间隔时期进行积累,
         *      并在 delta() 被调用后清零.
         */
        int delta();

        /**
         * 获取累积量, 即所有 delta 调用返回值的总和.
         */
        int getAccumulated();

        /**
         * 清除引脚绑定的中断.
         */
        void deinit() const;

        /**
         * 调用 deinit.
         */
        ~Knob();
    };
} // knob

#endif //KNOB_H
