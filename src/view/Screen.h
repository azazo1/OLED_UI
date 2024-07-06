//
// Created by azazo1 on 2024/7/6.
//

#ifndef SCREEN_H
#define SCREEN_H
#include <SSD1306Wire.h>
#include <sche/Scheduler.h>

#include "View.h"
#include "event/Event.h"

// todo 实现多级菜单
namespace view {
    class Screen {
        SSD1306Wire *const display;
        View *rootView = nullptr;
        sche::Scheduler *scheduler = nullptr;
        bool alive = true;

    public:
        /**
         * 创建一个 Screen 对象, 负责建立 Scheduler 和 View 的桥梁.
         * @param display OLED 显示操作, 不需要是堆指针, 不会对其生命周期进行管理.
         */
        explicit Screen(SSD1306Wire *display);

        /**
         * 在提供的 Scheduler 下创建一个子 Scheduler 并将子 Scheduler 附加到 提供的 Scheduler(普通优先级) 上.
         * @param scheduler 提供的 Scheduler, 不需要是堆指针, 不会对其生命周期进行管理.
         */
        void attachToScheduler(sche::Scheduler *scheduler);

        /**
         * 设置根视图.
         * @param view 根视图, 需要是堆指针, 在 Screen 被摧毁时或再次调用 setRootView 时对其进行销毁.
         */
        void setRootView(View *view);

        /**
         * 向 Root View 传递事件.
         * @param event 要传递的事件.
         */
        void dispatchEvent(const event::Event &event) const;

        void destroy();

        bool isAlive() const;

        ~Screen();
    };
} // view

#endif //SCREEN_H
