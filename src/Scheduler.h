//
// Created by azazo1 on 2024/7/6.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <functional>
#include <list>
#include <Schedulable.h>

#define PRIORITY_LOW (-1)
#define PRIORITY_PLAIN (0)
#define PRIORITY_HIGH (1)


namespace sche {
    typedef std::list<std::pair<Schedulable *, mtime_t> > ScheList;

    class Scheduler {
        /// [( (mtime_t)->bool, lastExecutedTime )]
        ScheList scheListLow;
        ScheList scheListPlain;
        ScheList scheListHigh;

        static void traverseAndFilter(ScheList &lst, mtime_t nowTime);

    public:
        Scheduler();

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
         * 主循环函数, 调用此函数会一直阻塞直到 scheList 为空,
         * 只有调用此函数, scheList 中的函数才会被调用.
         */
        void mainloop();
    };
} // sche

#endif //SCHEDULER_H
