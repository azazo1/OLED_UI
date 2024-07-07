//
// Created by azazo1 on 2024/7/7.
//

#include "ConsumerSchedulable.h"

namespace sche {
    ConsumerSchedulable::ConsumerSchedulable(Producer producer): producer(std::move(producer)) {
    }

    bool ConsumerSchedulable::schedule(const mtime_t deltaTime) {
        if (current == nullptr) {
            current = producer();
        }
        if (current == nullptr) {
            return false;
        }
        if (!current->schedule(deltaTime)) {
            delete current;
            current = nullptr;
        }
        return true;
    }

    ConsumerSchedulable::~ConsumerSchedulable() {
        delete current;
    }
} // sche
