//
// Created by azazo1 on 2024/7/7.
//

#ifndef DELAYSCHEDULABLE_H
#define DELAYSCHEDULABLE_H
#include "Schedulable.h"

namespace sche {
    class DelaySchedulable final : public Schedulable {
        const mtime_t delay;
        const mtime_t initialDelay;
        Schedulable *schedulable;
        mtime_t accumulatedDeltaTime = 0;
        mtime_t deltaTimeForSchedulable = 0;
        bool first = true;

    public:
        /**
         * 让可安排项相距一定时间被安排(可重复).
         * @param initialDelay 执行 schedulable 之前的等待时间.
         * @param interval 执行一次 schedulable 之后的等待时间.
         * @param schedulable Schedulable 堆指针, 传入所有权,
         *                    schedule 方法如果返回 true 则会触发下次 delay, 以此实现重复.
         */
        explicit DelaySchedulable(mtime_t initialDelay, mtime_t interval, Schedulable *schedulable);

        bool schedule(unsigned long deltaTime) override;

        ~DelaySchedulable() override;
    };
} // sche

#endif //DELAYSCHEDULABLE_H
