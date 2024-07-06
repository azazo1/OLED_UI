//
// Created by azazo1 on 2024/7/6.
//

#include "SequenceSchedulable.h"

namespace sche {
    SequenceSchedulable::SequenceSchedulable() = default;

    SequenceSchedulable *SequenceSchedulable::then(Schedulable *schedulable) {
        sequence.push(schedulable);
        return this;
    }

    bool SequenceSchedulable::schedule(const mtime_t deltaTime) {
        if (sequence.empty()) {
            return false;
        }
        const auto schedulable = sequence.front();
        const bool rst = schedulable->schedule(deltaTime);
        if (!rst) {
            // 结束子项生命
            delete schedulable;
            sequence.pop();
            if (dieOnEmpty && sequence.empty()) {
                return false;
            }
        }
        return true;
    }

    SequenceSchedulable *SequenceSchedulable::setRemain(const bool remain) {
        dieOnEmpty = !remain;
        return this;
    }
} // sche
