//
// Created by azazo1 on 2024/7/6.
//

#include "Screen.h"

#include <sche/SchedulableFromLambda.h>

#include "event/Event.h"

namespace view {
    Screen::Screen(SSD1306Wire *display): display(display) {
    }

    void Screen::attachToScheduler(sche::Scheduler *scheduler) {
        if (this->scheduler != nullptr) {
            // 确保原来的子 Scheduler 被销毁.
            this->scheduler->clear();
            this->scheduler->setRemain(false);
        }
        const auto subScheduler = new sche::Scheduler();
        subScheduler->setRemain(true);
        scheduler->addSchedule(subScheduler);
        this->scheduler = subScheduler;
        subScheduler->addSchedule(new sche::SchedulableFromLambda(
            [this](sche::mtime_t) {
                if (rootView != nullptr) {
                    rootView->onDraw(0, 0,
                                     static_cast<int16_t>(display->getWidth()),
                                     static_cast<int16_t>(display->getHeight()),
                                     display, this->scheduler);
                }
                return alive;
            }
        ));
    }

    void Screen::setRootView(View *view) {
        delete rootView;
        rootView = view;
    }

    void Screen::dispatchEvent(const event::Event &event) const {
        if (rootView != nullptr) {
            rootView->dispatchEvent(event);
        }
    }

    void Screen::destroy() {
        if (scheduler != nullptr) {
            scheduler->setRemain(false);
        }
        alive = false;
        scheduler = nullptr; // scheduler 的所有权已经在 attachToScheduler 时交给了父 scheduler.
        delete rootView;
        rootView = nullptr;
    }

    Screen::~Screen() {
        destroy();
    }

    bool Screen::isAlive() const {
        return alive;
    }
} // view
