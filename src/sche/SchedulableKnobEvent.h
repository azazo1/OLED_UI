//
// Created by azazo1 on 2024/9/3.
//

#ifndef SCHEDULABLEKNOBEVENT_H
#define SCHEDULABLEKNOBEVENT_H
#include <Knob.h>

#include "Schedulable.h"
#include "SchedulableFromLambda.h"

namespace sche {
    class SchedulableKnobEvent final : public Schedulable {
        knob::Knob kb;
        bool alive = true;
        std::function<bool(int)> onRotation;

    public:
        /// 创建一个旋钮事件, 事件会在旋钮产生旋转时发生.
        /// \param pinA 旋钮的 1 号引脚.
        /// \param pinB 旋钮的 2 号引脚.
        /// \param onRotation 旋钮发生旋转时触发的回调函数, 回调函数的参数是旋转的格数, 有正有负.
        SchedulableKnobEvent(int pinA, int pinB, std::function<bool(int)> onRotation);

        bool schedule(unsigned long deltaTime) override;
    };
} // sche

#endif //SCHEDULABLEKNOBEVENT_H
