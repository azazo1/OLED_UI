//
// Created by azazo1 on 2024/7/6.
//

#include "Rect.h"

#include <sche/ScalaTransition.h>
#include <sche/SchedulableFromLambda.h>

namespace view {
    Rect::Rect(const int16_t startX, const int16_t startY,
               const int16_t startWidth,
               const int16_t startHeight, const sche::mtime_t transitionTime,
               const propmap::PropotionMapping *mapping,
               SSD1306Wire *display)
        : transitionTime(transitionTime), mapping(mapping),
          curX(startX), curY(startY), curW(startWidth), curH(startHeight),
          display(display) {
    }

    void Rect::setScheduler(sche::Scheduler *scheduler) {
        this->scheduler = scheduler;
        scheduler->setRemain(true);
    }

    void Rect::init() const {
        scheduler->addSchedule(new sche::SchedulableFromLambda([this](sche::mtime_t) {
            display->drawRect(curX, curY, curW, curH);
            return alive;
        }));
    }

    void Rect::update(const int16_t newX, const int16_t newY,
                      const int16_t newW, const int16_t newH) {
        if (alive) {
            size_t curBatch = ++transitionBatch;
            if (newX >= 0) {
                scheduler->addSchedule(new sche::ScalaTransition(
                    curX, newX, transitionTime, mapping,
                    [curBatch, this](const int16_t val) {
                        curX = val;
                        return alive && curBatch == transitionBatch;
                        // 每次 update 都会更新 transitionBatch, 使之前的可安排项自动无效.
                    }
                ));
            }
            if (newY >= 0) {
                scheduler->addSchedule(new sche::ScalaTransition(
                    curY, newY, transitionTime, mapping,
                    [curBatch, this](const int16_t val) {
                        curY = val;
                        return alive && curBatch == transitionBatch;
                        // 每次 update 都会更新 transitionBatch, 使之前的可安排项自动无效.
                    }
                ));
            }
            if (newW >= 0) {
                scheduler->addSchedule(new sche::ScalaTransition(
                    curW, newW, transitionTime, mapping,
                    [curBatch, this](const int16_t val) {
                        curW = val;
                        return alive && curBatch == transitionBatch;
                        // 每次 update 都会更新 transitionBatch, 使之前的可安排项自动无效.
                    }
                ));
            }
            if (newH >= 0) {
                scheduler->addSchedule(new sche::ScalaTransition(
                    curH, newH, transitionTime, mapping,
                    [curBatch, this](const int16_t val) {
                        curH = val;
                        return alive && curBatch == transitionBatch;
                        // 每次 update 都会更新 transitionBatch, 使之前的可安排项自动无效.
                    }
                ));
            }
        }
    }

    void Rect::destroy() {
        scheduler->setRemain(false);
        alive = false;
    }

    Rect::~Rect() {
        destroy();
    }
} // view
