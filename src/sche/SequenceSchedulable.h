//
// Created by azazo1 on 2024/7/6.
//

#ifndef SEQUENCESCHEDULABLE_H
#define SEQUENCESCHEDULABLE_H
#include <queue>
#include <vector>

#include "Schedulable.h"
#include "SchedulableFromLambda.h"

namespace sche {
    class SequenceSchedulable final : public Schedulable {
        std::queue<Schedulable *> sequence;

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
    };
} // sche

#endif //SEQUENCESCHEDULABLE_H
