//
// Created by azazo1 on 2024/7/6.
//

#ifndef SCREEN_H
#define SCREEN_H
#include <SSD1306Wire.h>
#include <stack>
#include <vector>
#include <sche/Scheduler.h>

#include "View.h"
#include "event/Event.h"

// todo 实现多级菜单
namespace view {
    class Screen {
        SSD1306Wire *const display;
        std::stack<View *> rootViewStack;
        sche::Scheduler *scheduler = nullptr;
        bool alive = true;
        /// 当 Screen 接收到长按按钮事件(且 RootView 没有消耗掉)时触发返回操作,
        /// Screen 内部弹出栈顶的 RootView, 并且作为参数传递给 onBackward 回调函数,
        /// 此函数可以用来进行 RootView 的析构或其他处理.
        std::function<void(View *)> onBackward = [](View *) {
        };

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
         * 获取 rootViewStack 大小.
         */
        size_t getStackSize() const;

        /**
         * 设置进一层根视图(用于实现多级界面).
         * @param view 根视图, 不需要是堆指针, 不对其生命周期进行管理.
         */
        void pushRootView(View *view);

        /**
         * 弹出最顶层的 RootView(用于实现多级菜单的返回).
         * @return rootView, 需要手动控制生命周期.
         */
        View *popRootView();

        /**
         * 向 Root View 传递事件.
         * @param event 要传递的事件.
         */
        void dispatchEvent(const event::Event &event);

        sche::Scheduler &getScheduler() const;

        SSD1306Wire &getDisplay() const;

        /**
         * 见 onBackward.
         */
        void setOnBackward(std::function<void(View *)> onBackward);

        /**
         * 在销毁 Screen 之前, 需要确保 rootViewStack 中没有堆指针, 否则会导致内存泄漏.
         */
        void destroy();

        bool isAlive() const;

        ~Screen();
    };
} // view

#endif //SCREEN_H
