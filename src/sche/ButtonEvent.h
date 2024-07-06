//
// Created by azazo1 on 2024/7/6.
//

#ifndef BUTTONEVENT_H
#define BUTTONEVENT_H
#include <Button.h>
#include <functional>

#include "ButtonEvent.h"
#include "Schedulable.h"
#include "SchedulableFromLambda.h"


namespace sche {
    class ButtonEvent final : public Schedulable {
        btn::Button btn;
        bool alive = true;
        CallbackFunc onRelease;

    public:
        /**
         * 创建一个按钮释放事件, 即事件将会在按钮松开后触发.
         * @param pin 要监听的引脚
         * @param onRelease 按钮释放时要触发的函数, 函数接收一个参数, 表示按钮按下了多久(ms), 函数返回值是是否继续监听.
         */
        ButtonEvent(int pin, CallbackFunc onRelease);

        bool schedule(unsigned long deltaTime) override;
    };
} // sche

#endif //BUTTONEVENT_H
