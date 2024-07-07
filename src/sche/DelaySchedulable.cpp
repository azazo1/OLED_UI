//
// Created by azazo1 on 2024/7/7.
//

#include "DelaySchedulable.h"

namespace sche {
    DelaySchedulable::DelaySchedulable(const mtime_t initialDelay, const mtime_t interval,
                                       Schedulable *const schedulable)
        : delay(interval), initialDelay(initialDelay), schedulable(schedulable) {
    }

    bool DelaySchedulable::schedule(const mtime_t deltaTime) {
        if (schedulable == nullptr) {
            return false;
        }
        accumulatedDeltaTime += deltaTime;
        if (accumulatedDeltaTime >= (first ? initialDelay : delay)) {
            if (!schedulable->schedule(accumulatedDeltaTime)) {
                delete schedulable;
                schedulable = nullptr;
                return false;
            }
            accumulatedDeltaTime = 0;
            first = false;
        }
        return true;
    }

    DelaySchedulable::~DelaySchedulable() {
        delete schedulable;
    }
} // sche
