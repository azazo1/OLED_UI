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
        const int16_t start;
        const int16_t end;
        const mtime_t duration;
        const propmap::PropotionMapping *const mapping;
        mtime_t passedTime = 0;
        /// 回调函数, 接收变化值当前的值, 返回是否需要继续变化.
        const std::function<bool(int16_t)> reporter;

    public:
        ScalaTransition(int16_t start, int16_t end, mtime_t durationMillis,
                        const propmap::PropotionMapping *mapping,
                        std::function<bool(int16_t)> reporter);

        bool schedule(unsigned long deltaTime) override;
    };
} // sche

#endif //SCALATRANSITION_H
