//
// Created by azazo1 on 2024/7/6.
//

#ifndef SCHEDULABLE_H
#define SCHEDULABLE_H

namespace sche {
    typedef unsigned long mtime_t;

    class Schedulable {
    public:
        /**
         * 创建一个可安排项.
         * @note 可安排项一般以堆指针的形式出现在代码中, 一旦创建, 直接传入 Scheduler,
         *       意味着在 Scheduler 外的代码不能持有可安排项的指针, 否则容易导致无效指针.
         *       但是 SequenceSchedulable 是个例外, 因为其手动控制子项的生命周期,
         *       保证了子项的生命周期一定比其生命周期短.
         */
        Schedulable() = default;

        /**
         * 子类应继承此方法, 起到回调函数的作用, 此方法不应阻塞, 否则其他可安排项将得不到安排资源.
         * @param deltaTime 距离上一次开始调用此函数时所消耗的时间.
         * @return 此对象是否还要继续被安排.
         */
        virtual bool schedule(unsigned long deltaTime) =0;

        virtual ~Schedulable() = default;
    };
} // sche

#endif //SCHEDULABLE_H
