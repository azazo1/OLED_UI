//
// Created by azazo1 on 2024/7/6.
//

#include "SchedulableFromLambda.h"

#include <utility>

namespace sche {
    SchedulableFromLambda::SchedulableFromLambda(CallbackFunc func): func(std::move(func)) {
    }

    bool SchedulableFromLambda::schedule(const mtime_t deltaTime) {
        return func(deltaTime);
    }
} // sche
