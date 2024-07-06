//
// Created by azazo1 on 2024/7/6.
//

#ifndef SEQUENCESCHEDULABLE_H
#define SEQUENCESCHEDULABLE_H
#include <queue>

#include "Schedulable.h"
#include "SchedulableFromLambda.h"

namespace sche {
    class SequenceSchedulable final : public Schedulable {
        std::queue<Schedulable *> sequence;
        /// 是否在 sequence 为空时结束生命.
        bool dieOnEmpty = true;

    public:
        /**
         * 创建一个序列可安排项, 即添加到其中的可安排项会在第一个项生命结束后开始执行第二个可安排项, 以此类推.
         */
        SequenceSchedulable();

        /**
         * 添加可安排项到队列.
         * @param schedulable 要添加到队列的可安排项, 需要是堆上的对象, 以便内部 delete.
         * @return 返回自身以供链式调用.
         */
        SequenceSchedulable *then(Schedulable *schedulable);

        bool schedule(unsigned long deltaTime) override;

        /**
         * 设置生命周期长度.
         * @param remain 是否在 sequence 为空时保持生命.
         * @return 返回自身以供链式调用.
         * @note 如果参数为 true,
         *       请确保在 Scheduler 的 mainloop 之外持有 SequenceSchedulable 对象(指针),
         *       否则会导致死锁.
         *       此时 SequenceSchedulable 对象的生命周期为无限长, 可放心在外面持有此对象(指针).
         */
        SequenceSchedulable *setRemain(bool remain);
    };
} // sche

#endif //SEQUENCESCHEDULABLE_H
