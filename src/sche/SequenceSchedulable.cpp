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
            delete schedulable;
            sequence.pop();
            if (sequence.empty()) {
                return false;
            }
        }
        return true;
    }
} // sche
