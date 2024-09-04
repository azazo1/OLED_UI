//
// Created by azazo1 on 2024/7/6.
//

#ifndef SCALATRANSITION_H
#define SCALATRANSITION_H
#include <cstdint>
#include <functional>
#include <propotion_mapping/PropotionMapping.h>

#include "Schedulable.h"

namespace sche {
    class ScalaTransition final : public Schedulable {
        const double start;
        const double end;
        const mtime_t duration;
        const propmap::PropotionMapping *const mapping;
        mtime_t passedTime = 0;
        /// 回调函数, 接收变化值当前的值, 返回是否需要继续变化.
        const std::function<bool(double)> reporter;

        bool ended = false;

    public:
        /**
         * 创建一个数值过渡 Schedulable, 在回调中, end 保证能够回调, 且仅有一次.
         *
         * 结束此对象的生命周期有一下几种可能:
         * 1. 超时, 即经过时间大于 durationMillis.
         * 2. reporter 回调返回 false.
         * 3. reporter(end) 被调用.
         *
         * @note mapping 参数不必是堆指针, 不会对其生命周期进行管理, 也可以为 null, 此时为线性映射.
         */
        ScalaTransition(double start, double end, mtime_t durationMillis,
                        const propmap::PropotionMapping *mapping,
                        std::function<bool(double)> reporter);

        bool schedule(unsigned long deltaTime) override;
    };
} // sche

#endif //SCALATRANSITION_H
