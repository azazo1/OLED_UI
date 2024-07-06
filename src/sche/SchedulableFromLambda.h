//
// Created by azazo1 on 2024/7/6.
//

#ifndef SCHEDULABLEFROMLAMBDA_H
#define SCHEDULABLEFROMLAMBDA_H
#include <functional>
#include "Schedulable.h"

namespace sche {
    typedef std::function<bool(mtime_t)> CallbackFunc;

    class SchedulableFromLambda final : public Schedulable {
        CallbackFunc func;

    public:
        explicit SchedulableFromLambda(CallbackFunc func);

        bool schedule(mtime_t deltaTime) override;
    };
} // sche

#endif //SCHEDULABLEFROMLAMBDA_H
