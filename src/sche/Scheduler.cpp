//
// Created by azazo1 on 2024/7/6.
//

#include "Scheduler.h"
#include <esp32-hal.h>


namespace sche {
    Scheduler::Scheduler() = default;

    void Scheduler::addSchedule(Schedulable *schedulable, const int priority) {
        mtime_t nowTime = millis();
        switch (priority) {
            case PRIORITY_LOW:
                scheListLow.emplace_back(schedulable, nowTime);
                break;
            case PRIORITY_PLAIN:
                scheListPlain.emplace_back(schedulable, nowTime);
                break;
            case PRIORITY_HIGH:
                scheListHigh.emplace_back(schedulable, nowTime);
                break;
            default:
                log_e("Invalid priority");
        }
    }

    void Scheduler::clear() {
        scheListLow.clear();
        scheListPlain.clear();
        scheListHigh.clear();
    }

    void Scheduler::mainloop() {
        while (
            !scheListLow.empty()
            || !scheListPlain.empty()
            || !scheListHigh.empty()
        ) {
            const mtime_t nowTime = millis();
            traverseAndFilter(scheListHigh, nowTime);
            traverseAndFilter(scheListPlain, nowTime);
            traverseAndFilter(scheListLow, nowTime);
        }
    }

    void Scheduler::traverseAndFilter(ScheList &lst, const mtime_t nowTime) {
        auto beginIter = lst.begin();
        const auto endIter = lst.end();
        while (beginIter != endIter) {
            if (beginIter->first->schedule(nowTime - beginIter->second)) {
                // continue
                beginIter->second = nowTime;
                ++beginIter;
            } else {
                delete beginIter->first;
                beginIter = lst.erase(beginIter);
            }
        }
    }
} // sche
