//
// Created by azazo1 on 2024/7/6.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <functional>
#include <list>
#include "Schedulable.h"

#define PRIORITY_LOW (-1)
#define PRIORITY_PLAIN (0)
#define PRIORITY_HIGH (1)


namespace sche {
    typedef std::list<std::pair<Schedulable *, mtime_t> > ScheList;

    class Scheduler final : public Schedulable {
        /// [( (mtime_t)->bool, lastExecutedTime )]
        ScheList scheListLow;
        ScheList scheListPlain;
        ScheList scheListHigh;

        /// 由 schedule 方法改变.
        mtime_t nowTime = 0;

        /// 是否在三个列表都为空后结束生命.
        bool dieOnEmpty = true;

        static void traverseAndFilter(ScheList &lst, mtime_t nowTime);

    public:
        /**
         * 创建一个安排者, 向安排者添加的可安排项会被反复`安排`直到其生命结束(可安排项返回 false).
         */
        Scheduler() = default;

        /**
         * 在 Scheduler 中添加一个任务.
         * @param schedulable 可安排对象, 传入的参数需要是在堆中的对象, 以便 delete 操作.
         * @param priority 函数被调用优先级, 高优先级偏向于更早调用.
         */
        void addSchedule(Schedulable *schedulable, int priority = PRIORITY_PLAIN);

        /**
         * 清空所有待执行的任务.
         */
        void clear();

        /**
         * 设置生命周期长度.
         * @param remain 是否在消耗完可安排项后仍保持生命.
         * 见 SequenceSchedulable::setRemain
         */
        void setRemain(bool remain);

        /**
         * 主循环函数, 调用此函数会一直阻塞直到 scheList 为空,
         * 只有调用此函数, scheList 中的函数才会被调用.
         */
        void mainloop();

        bool schedule(unsigned long deltaTime) override;

        ~Scheduler() override;
    };
} // sche

#endif //SCHEDULER_H
