//
// Created by azazo1 on 2024/7/6.
//
#include "Schedulable.h"

namespace sche {
    void Schedulable::then(Schedulable *next) {
        this->next = next;
    }
}
