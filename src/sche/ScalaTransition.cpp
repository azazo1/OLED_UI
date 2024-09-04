//
// Created by azazo1 on 2024/7/6.
//

#include "ScalaTransition.h"

#include <utility>

namespace sche {
    ScalaTransition::ScalaTransition(const double start, const double end,
                                     const mtime_t durationMillis,
                                     const propmap::PropotionMapping *mapping,
                                     std::function<bool(double)> reporter)
        : start(start), end(end), duration(durationMillis), mapping(mapping),
          reporter(std::move(reporter)) {
    }

    bool ScalaTransition::schedule(const mtime_t deltaTime) {
        if (ended) {
            return false;
        }
        // 以下都是在 !ended 的前提下进行的, 也就是说 reporter(end) 没被调用过.

        passedTime += deltaTime;
        if (passedTime > duration) {
            reporter(end); // 保证接收到 end.
            return false;
        }
        double passedRatio = 1.0 * passedTime / duration;
        if (mapping != nullptr) {
            passedRatio = mapping->mapping(passedRatio);
        }
        const auto arg = start + (end - start) * passedRatio;
        if (arg == end) {
            ended = true;
        }
        return reporter(arg);
    }
} // sche
