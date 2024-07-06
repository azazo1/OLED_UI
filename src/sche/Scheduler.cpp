//
// Created by azazo1 on 2024/7/6.
//

#include "Scheduler.h"
#include <esp32-hal.h>
#include <Arduino.h>

namespace sche {
    void Scheduler::addSchedule(Schedulable *schedulable, const int priority) {
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
        for (const auto sc: scheListLow) {
            delete sc.first;
        }
        for (const auto sc: scheListPlain) {
            delete sc.first;
        }
        for (const auto sc: scheListHigh) {
            delete sc.first;
        }
        scheListLow.clear();
        scheListPlain.clear();
        scheListHigh.clear();
    }

    void Scheduler::mainloop() {
        while (schedule(millis() - nowTime)); // 这里的 nowTime 是上一次执行的时间.
    }

    void Scheduler::setRemain(const bool remain) {
        dieOnEmpty = !remain;
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

    bool Scheduler::schedule(const mtime_t deltaTime) {
        nowTime += deltaTime;
        traverseAndFilter(scheListHigh, nowTime);
        traverseAndFilter(scheListPlain, nowTime);
        traverseAndFilter(scheListLow, nowTime);
        if (dieOnEmpty) {
            return !scheListLow.empty()
                   || !scheListPlain.empty()
                   || !scheListHigh.empty();
        }
        return true;
    }

    Scheduler::~Scheduler() {
        clear();
    }
} // sche
