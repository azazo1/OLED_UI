//
// Created by azazo1 on 2024/7/6.
//

#include "Screen.h"

#include <Button.h>
#include <event/ButtonEvent.h>
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
                if (!rootViewStack.empty()) {
                    rootViewStack.top()->onDraw(0, 0,
                                                static_cast<int16_t>(display->getWidth()),
                                                static_cast<int16_t>(display->getHeight()),
                                                display, this->scheduler);
                }
                return alive;
            }
        ));
    }

    void Screen::pushRootView(View *view) {
        rootViewStack.push(view);
    }

    void Screen::dispatchEvent(const event::Event &event) {
        if (!rootViewStack.empty()) {
            if (rootViewStack.top()->dispatchEvent(event)) {
                return;
            } // 如果 rootView 消耗了长按事件则不进行返回操作.
        }
        if (event.getType() == EVENT_TYPE_BUTTON) {
            if (static_cast<const event::ButtonEvent *>(&event)->isLongClick()) {
                if (rootViewStack.size() > 1) {
                    onBackward(popRootView());
                }
            }
        }
    }

    size_t Screen::getStackSize() const {
        return rootViewStack.size();
    }

    View *Screen::popRootView() {
        View *rst = rootViewStack.top();
        rootViewStack.pop();
        return rst;
    }

    void Screen::destroy() {
        if (scheduler != nullptr) {
            scheduler->setRemain(false);
        }
        alive = false;
        scheduler = nullptr; // scheduler 的所有权已经在 attachToScheduler 时交给了父 scheduler.
        // 此处不对 rootViews 进行析构.
    }

    Screen::~Screen() {
        destroy();
    }

    bool Screen::isAlive() const {
        return alive;
    }

    sche::Scheduler &Screen::getScheduler() const {
        return *scheduler;
    }

    SSD1306Wire &Screen::getDisplay() const {
        return *display;
    }

    void Screen::setOnBackward(std::function<void(View *)> onBackward) {
        this->onBackward = std::move(onBackward);
    }
} // view
