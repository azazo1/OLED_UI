//
// Created by azazo1 on 2024/7/6.
//

#include "ScalaTransition.h"

#include <utility>

namespace sche {
    ScalaTransition::ScalaTransition(const int16_t start, const int16_t end,
                                     const mtime_t durationMillis,
                                     const propmap::PropotionMapping *mapping,
                                     std::function<bool(int16_t)> reporter)
        : start(start), end(end), duration(durationMillis), mapping(mapping),
          reporter(std::move(reporter)) {
    }

    bool ScalaTransition::schedule(const mtime_t deltaTime) {
        passedTime += deltaTime;
        if (passedTime > duration) {
            return false;
        }
        double passedRatio = 1.0 * passedTime / duration;
        passedRatio = mapping->mapping(passedRatio);
        return reporter(static_cast<int16_t>(start + (end - start) * passedRatio));
    }
} // sche
