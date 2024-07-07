//
// Created by azazo1 on 2024/7/7.
//

#ifndef CONSUMERSCHEDULABLE_H
#define CONSUMERSCHEDULABLE_H
#include <functional>

#include "Schedulable.h"

namespace sche {
    typedef std::function<Schedulable*()> Producer;

    class ConsumerSchedulable final : public Schedulable {
        Producer producer;
        Schedulable *current = nullptr;

    public:
        /**
        * 此类和 SequenceSchedulable 类似, 都是让 Schedulable 依次执行, 但是其内部 Schedulable 可以动态添加.
        *
        * 此类对象接受一个 producer 回调函数, 通过调用这个函数来获取新的 Schedulable, 当 producer 返回 nullptr 时,
        * 此对象生命结束.
        */
        explicit ConsumerSchedulable(Producer producer);

        bool schedule(unsigned long deltaTime) override;

        ~ConsumerSchedulable() override;
    };
} // sche

#endif //CONSUMERSCHEDULABLE_H
