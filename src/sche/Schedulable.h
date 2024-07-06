//
// Created by azazo1 on 2024/7/6.
//

#ifndef SCHEDULABLE_H
#define SCHEDULABLE_H

namespace sche {
    typedef unsigned long mtime_t;

    class Schedulable {
    public:
        Schedulable() = default;

        /**
         * 起到回调函数的作用.
         * @param deltaTime 距离上一次开始调用此函数时所消耗的时间.
         * @return 此对象是否还要继续被安排.
         */
        virtual bool schedule(unsigned long deltaTime) =0;

        virtual ~Schedulable() = default;
    };
} // sche

#endif //SCHEDULABLE_H
