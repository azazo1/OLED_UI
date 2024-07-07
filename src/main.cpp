#include <AllColorLED.h>
#include <Arduino.h>
#include <sche/SchedulableButtonEvent.h>
#include "event/ButtonEvent.h"
#include <propotion_mapping/LinearMapping.h>
#include <view/LabeledFrame.h>
#include <view/Screen.h>
#include <event/KnobEvent.h>
#include <sche/ConsumerSchedulable.h>
#include <sche/DelaySchedulable.h>
#include <sche/ScalaTransition.h>
#include <sche/SequenceSchedulable.h>
#include <view/Seekbar.h>
#include <view/Switch.h>
#include <view/TextSelector.h>
#include <view/TextView.h>

#include "sche/Schedulable.h"
#include "sche/SchedulableFromLambda.h"
#include "sche/Scheduler.h"
#include "SSD1306Wire.h"
#include "Knob.h"

// attention: ESP32 34-39引脚只能作为输入. https://blog.csdn.net/neo_Wordsworth/article/details/124290783
#define KNOB_PIN_A GPIO_NUM_32
#define KNOB_PIN_B GPIO_NUM_35
#define BUTTON_PIN 23
#define LED_R 27
#define LED_G 26
#define LED_B 25

#define FONT_DATA ArialMT_Plain_16

const String MAIN_FRAME_TITLE = "Actions";
const String RGB_LED_FRAME_TITLE = "RGB LED";
const String RGB_LED_TWEAK_FRAME_TITLE = "Color Tweaking";
const String R_FRAME_TITLE = "Red";
const String G_FRAME_TITLE = "Green";
const String B_FRAME_TITLE = "Blue";
const String BRE_SETTING_FRAME_TITLE = "BRE Setting";
const String BRE_OPTION_FRAME_TITLE = "BRE Option";
const String BRE_SPEED_FRAME_TITLE = "BRE Speed";
const String ENABLE_BRE = "Breathing?";
const String WATERFALL_FRAME_TITLE = "Waterfall";
const String WATERFALL_INDEV = "In developing...";

auto display = SSD1306Wire(0x3C, SDA, SCL);


void setup() {
    // 保存属性.
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    int16_t speed = 10;
    bool breath = false;
    // 临时属性.
    uint8_t tmpR = r;
    uint8_t tmpG = g;
    uint8_t tmpB = b;
    int16_t tmpSpeed = speed;
    // 上一次循环被中断/结束时的彩色 LED 亮度大小, 用于作为新一轮循环的起始亮度.
    int16_t ratioOnChange = 0;
    size_t speedAnimBatch = 0;

    Serial.begin(9600);

    acled::AllColorLED cled(LED_R, LED_G, LED_B);
    knob::Knob kb(KNOB_PIN_A, KNOB_PIN_B);
    sche::Scheduler scheduler;
    const propmap::LinearMapping linear_mapping;
    view::Screen screen(&display);
    screen.attachToScheduler(&scheduler);
    screen.setOnBackward([](view::View *popen) {
    });

    display.init();
    display.setBrightness(50);
    display.flipScreenVertically();
    view::View::setFont(FONT_DATA);
    display.setFont(FONT_DATA);

    // 主菜单.
    auto mainFrame = view::LabeledFrame(MAIN_FRAME_TITLE);
    // - RGB 灯设置.
    auto rgbLedFrame = view::LabeledFrame(RGB_LED_FRAME_TITLE);
    // -- 调色.
    auto rgbLedTweakingFrame = view::LabeledFrame(RGB_LED_TWEAK_FRAME_TITLE);
    // --- 三颜色色彩设置.
    auto rgbLedRFrame = view::LabeledFrame(R_FRAME_TITLE);
    auto rgbLedGFrame = view::LabeledFrame(G_FRAME_TITLE);
    auto rgbLedBFrame = view::LabeledFrame(B_FRAME_TITLE);
    // -- 呼吸灯设置.
    auto rgbLedBreSettingFrame = view::LabeledFrame(BRE_SETTING_FRAME_TITLE);
    // --- RGB 灯是否呼吸设置.
    auto rgbLedBreSwitchFrame = view::LabeledFrame(BRE_OPTION_FRAME_TITLE); // "Enable BRE?"
    // --- 呼吸灯速度设置.
    auto rgbLedBreSpeedFrame = view::LabeledFrame(BRE_SPEED_FRAME_TITLE);
    // - 流水灯设置(没开发, 仅供演示).
    auto waterfallFrame = view::LabeledFrame(WATERFALL_FRAME_TITLE);

    screen.pushRootView(&mainFrame);

    auto mainTextSel = view::TextSelector();
    mainTextSel.setLooping(false);
    mainTextSel.addItem(RGB_LED_FRAME_TITLE);
    mainTextSel.addItem(WATERFALL_FRAME_TITLE); // 没开发 仅供演示.
    mainFrame.addChild(&mainTextSel);

    auto rgbLedSel = view::TextSelector();
    rgbLedSel.setLooping(false);
    rgbLedSel.addItem(RGB_LED_TWEAK_FRAME_TITLE);
    rgbLedSel.addItem(BRE_SETTING_FRAME_TITLE);
    rgbLedFrame.addChild(&rgbLedSel);

    auto rgbLedRGBSel = view::TextSelector();
    rgbLedRGBSel.setLooping(false);
    rgbLedRGBSel.addItem(R_FRAME_TITLE);
    rgbLedRGBSel.addItem(G_FRAME_TITLE);
    rgbLedRGBSel.addItem(B_FRAME_TITLE);
    rgbLedTweakingFrame.addChild(&rgbLedRGBSel);

    auto rSb = view::Seekbar();
    auto gSb = view::Seekbar();
    auto bSb = view::Seekbar();
    rSb.setMax(255);
    gSb.setMax(255);
    bSb.setMax(255);
    rSb.setMin(0);
    gSb.setMin(0);
    bSb.setMin(0);
    rSb.setStep(3);
    gSb.setStep(3);
    bSb.setStep(3);
    rgbLedRFrame.addChild(&rSb);
    rgbLedGFrame.addChild(&gSb);
    rgbLedBFrame.addChild(&bSb);

    auto rgbLedBreSettingSel = view::TextSelector();
    rgbLedBreSettingSel.setLooping(false);
    rgbLedBreSettingSel.addItem(BRE_OPTION_FRAME_TITLE);
    rgbLedBreSettingSel.addItem(BRE_SPEED_FRAME_TITLE);
    rgbLedBreSettingFrame.addChild(&rgbLedBreSettingSel);

    auto rgbLedBreSwitch = view::Switch(ENABLE_BRE);
    rgbLedBreSwitchFrame.addChild(&rgbLedBreSwitch);

    auto rgbLedBreSpeedSb = view::Seekbar();
    rgbLedBreSpeedSb.setMax(100);
    rgbLedBreSpeedSb.setMin(1);
    rgbLedBreSpeedSb.setStep(5);
    rgbLedBreSpeedFrame.addChild(&rgbLedBreSpeedSb);

    auto waterfallText = view::TextView(WATERFALL_INDEV);
    waterfallFrame.addChild(&waterfallText);


    mainTextSel.setOnConfirmListener(
        [&screen, &rgbLedFrame, &waterfallFrame](const size_t selected) {
            if (selected == 0) {
                screen.pushRootView(&rgbLedFrame);
            } else if (selected == 1) {
                screen.pushRootView(&waterfallFrame);
            }
        }
    );

    rgbLedSel.setOnConfirmListener(
        [&screen, &rgbLedTweakingFrame, &rgbLedBreSettingFrame](const size_t selected) {
            if (selected == 0) {
                // Color Tweaking.
                screen.pushRootView(&rgbLedTweakingFrame);
            } else if (selected == 1) {
                screen.pushRootView(&rgbLedBreSettingFrame);
            }
        }
    );

    rgbLedRGBSel.setOnConfirmListener(
        [&](const size_t selected) {
            cled.setRGB(r, g, b);
            switch (selected) {
                case 0:
                    rSb.setCurrent(r);
                    screen.pushRootView(&rgbLedRFrame);
                    break;
                case 1:
                    gSb.setCurrent(g);
                    screen.pushRootView(&rgbLedGFrame);
                    break;
                case 2:
                    bSb.setCurrent(b);
                    screen.pushRootView(&rgbLedBFrame);
                default: {
                }
            }
        }
    );

    rSb.setOnChangeListener([&](const int16_t cur) {
        tmpR = cur; // 改变颜色, 但是不保存, 退出后重置.
        rgbLedRFrame.setTitle(R_FRAME_TITLE + ": " + cur);
    });
    rSb.setOnCancelListener([&](const int16_t) {
        tmpR = r; // 还原颜色.
        rgbLedRFrame.setTitle(R_FRAME_TITLE);
        screen.popRootView();
    });
    rSb.setOnConfirmListener([&](const int16_t cur) {
        tmpR = r = cur; // 保存颜色
        rgbLedRFrame.setTitle(R_FRAME_TITLE);
        screen.popRootView();
    });

    gSb.setOnChangeListener([&](const int16_t cur) {
        tmpG = cur; // 改变颜色, 但是不保存, 退出后重置.
        rgbLedGFrame.setTitle(G_FRAME_TITLE + ": " + cur);
    });
    gSb.setOnCancelListener([&](const int16_t) {
        tmpG = g; // 还原颜色.
        rgbLedGFrame.setTitle(G_FRAME_TITLE);
        screen.popRootView();
    });
    gSb.setOnConfirmListener([&](const int16_t cur) {
        tmpG = g = cur;
        rgbLedGFrame.setTitle(G_FRAME_TITLE);
        screen.popRootView();
    });

    bSb.setOnChangeListener([&](const int16_t cur) {
        tmpB = cur; // 改变颜色, 但是不保存, 退出后重置.
        rgbLedBFrame.setTitle(B_FRAME_TITLE + ": " + cur);
    });
    bSb.setOnCancelListener([&](const int16_t) {
        tmpB = b; // 还原颜色.
        rgbLedBFrame.setTitle(B_FRAME_TITLE);
        screen.popRootView();
    });
    bSb.setOnConfirmListener([&](const int16_t cur) {
        tmpB = b = cur;
        rgbLedBFrame.setTitle(B_FRAME_TITLE);
        screen.popRootView();
    });

    rgbLedBreSettingSel.setOnConfirmListener([&](const size_t selected) {
        if (selected == 0) {
            rgbLedBreSwitch.setState(breath);
            screen.pushRootView(&rgbLedBreSwitchFrame);
        } else if (selected == 1) {
            rgbLedBreSpeedSb.setCurrent(speed);
            screen.pushRootView(&rgbLedBreSpeedFrame);
        }
    });

    rgbLedBreSwitch.setOnChangeListener([&](const bool checked) {
        breath = checked;
    });

    rgbLedBreSpeedSb.setOnChangeListener([&](const int16_t newSpeed) {
        speedAnimBatch++;
        tmpSpeed = newSpeed;
        rgbLedBreSpeedFrame.setTitle(BRE_SPEED_FRAME_TITLE + ": " + newSpeed);
    });
    rgbLedBreSpeedSb.setOnConfirmListener([&](const int16_t confirmSpeed) {
        speedAnimBatch++;
        speed = tmpSpeed = confirmSpeed;
        rgbLedBreSpeedFrame.setTitle(BRE_SPEED_FRAME_TITLE);
        screen.popRootView();
    });
    rgbLedBreSpeedSb.setOnCancelListener([&](const int16_t) {
        speedAnimBatch++;
        tmpSpeed = speed;
        rgbLedBreSpeedFrame.setTitle(BRE_SPEED_FRAME_TITLE);
        screen.popRootView();
    });


    // 清屏.
    scheduler.addSchedule(new sche::SchedulableFromLambda([&screen](sche::mtime_t) {
        display.clear();
        return screen.isAlive();
    }), PRIORITY_HIGH);

    // 配置旋钮事件来源.
    scheduler.addSchedule(new sche::SchedulableFromLambda(
                              [&screen, &kb](sche::mtime_t) {
                                  const auto delta = kb.delta();
                                  if (delta != 0) {
                                      screen.dispatchEvent(event::KnobEvent(screen, delta));
                                  }
                                  return screen.isAlive();
                              }
                          ), PRIORITY_PLAIN);

    // 配置按钮事件来源.
    scheduler.addSchedule(new sche::SchedulableButtonEvent(
                              BUTTON_PIN, [&screen](const sche::mtime_t pt) {
                                  screen.dispatchEvent(
                                      event::ButtonEvent(screen, static_cast<int>(pt)));
                                  return screen.isAlive();
                              }
                          ), PRIORITY_PLAIN);

    // 设置全彩 LED 显示.
    // 如果是呼吸模式, 那么往复地设置PWM.
    // 如果不是呼吸模式, 按照 tmp 值进行显示.
    auto curSpeedAnimBatch = speedAnimBatch;
    auto setACLEDLight = [&](const int16_t cur) {
        // 接受一个比例, 用来设置彩色 LED 灯光.
        if (breath) {
            const double ratio = cur * 0.01;
            const auto cr = static_cast<uint8_t>(ratio * tmpR);
            const auto cg = static_cast<uint8_t>(ratio * tmpG);
            const auto cb = static_cast<uint8_t>(ratio * tmpB);
            cled.setRGB(cr, cg, cb);
        } else {
            cled.setRGB(tmpR, tmpG, tmpB);
        }
        if (curSpeedAnimBatch != speedAnimBatch) {
            // 呼吸灯动画更新了, 直接死亡.
            // 此操作能够让用户在 设置很慢的速度之后直接设置较快的速度 的情况下快速让动画切换速度,
            // 而不是等待上一轮动画结束.
            return false;
        }
        ratioOnChange = cur; // 放在这里防止本来要死亡的 Schedulable 影响.
        return true;
    };
    // 产生 SequenceSchedulable 给 Consumer, Consumer 消耗完上一个 SequenceSchedulable 后, 再次产生, 以此循环.
    auto produceSequence = [&] {
        return (new sche::SequenceSchedulable())->then(new sche::ScalaTransition( // 正向数值过渡.
            ratioOnChange, 100, 10000 / tmpSpeed, nullptr,
            setACLEDLight
        ))->then(new sche::ScalaTransition( // 反向数值过渡.
            100, 0, 10000 / tmpSpeed, nullptr,
            setACLEDLight
        ))->then(new sche::SchedulableFromLambda([&](sche::mtime_t) {
            curSpeedAnimBatch = speedAnimBatch; // 维持下一轮循环的生命.
            return false; // 一次性.
        }));
    };
    // 加个 DelaySchedulable 稳定 analogWrite 输出.
    scheduler.addSchedule(new sche::DelaySchedulable(
                              0, 10, new sche::ConsumerSchedulable(produceSequence)
                          ), PRIORITY_PLAIN);

    // 显示内容.
    scheduler.addSchedule(new sche::SchedulableFromLambda([&screen](sche::mtime_t) {
        display.display();
        return screen.isAlive();
    }), PRIORITY_LOW);

    scheduler.mainloop();
}


void loop() {
}
