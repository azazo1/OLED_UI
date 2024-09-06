#include <Arduino.h>
#include <sche/SchedulableButtonEvent.h>
#include "event/ButtonEvent.h"
#include <propotion_mapping/LinearMapping.h>
#include <view/LabeledFrame.h>
#include <view/Screen.h>
#include <event/KnobEvent.h>
#include <sche/SchedulableKnobEvent.h>
#include <view/Seekbar.h>
#include <view/Switch.h>
#include <view/TextInput.h>
#include <view/TextSelector.h>

#include "sche/Schedulable.h"
#include "sche/Scheduler.h"
#include "SSD1306Wire.h"
#include "Knob.h"

#define KNOB_PIN_A 15
#define KNOB_PIN_B 4
#define BUTTON_PIN 16

#define FONT_DATA ArialMT_Plain_16

auto display = SSD1306Wire(0x3C, SDA, SCL);

void setup() {
    Serial.begin(9600);

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

    auto lf1 = view::LabeledFrame("Hello World");
    auto ts1 = view::TextSelector();
    auto lf2 = view::LabeledFrame("Fruit");
    auto ts2 = view::TextSelector();
    auto lf3 = view::LabeledFrame("Seekbar");
    auto sb3 = view::Seekbar();
    auto lf32 = view::LabeledFrame("Switch");
    auto st32 = view::Switch("Open?");
    auto input = view::TextInput();

    // 1
    ts1.addItem("Hello");
    ts1.addItem("Ni hao a");
    ts1.addItem("I like apple");
    ts1.addItem("Thks");
    ts1.addItem("Debug");
    ts1.setOnConfirmListener(
        [&screen, &ts1, &lf2, &input](const size_t idx) {
            Serial.print("Primary Selected: ");
            Serial.print(ts1.itemAt(idx));
            Serial.print(", ");
            Serial.println(idx);
            if (idx == 0) {
                screen.pushRootView(&input);
            } else {
                screen.pushRootView(&lf2);
            }
        });
    lf1.addChild(&ts1);

    // 2
    lf2.addChild(&ts2);
    ts2.addItem("Apple");
    ts2.addItem("Banana");
    ts2.addItem("Orange");
    ts2.addItem("Peach");
    ts2.addItem("Grape");
    ts2.setOnConfirmListener(
        [&screen, &ts2, &lf3, &lf32](const size_t idx) {
            Serial.print("Secondary Selected: ");
            Serial.print(ts2.itemAt(idx));
            Serial.print(", ");
            Serial.println(idx);
            if (idx == 0) {
                screen.pushRootView(&lf3);
            } else if (idx == 1) {
                screen.pushRootView(&lf32);
            }
        }
    );

    // 3
    lf3.addChild(&sb3);
    sb3.setOnChangeListener([&lf3](const int16_t cur) {
        Serial.printf("Seekbar3,now: %d\n", cur);
        lf3.setTitle(String("Seekbar: ") + cur);
    });
    sb3.setOnConfirmListener([&screen](const int16_t cur) {
        Serial.printf("Seekbar3: %d\n", cur);
        screen.popRootView();
    });
    sb3.setOnCancelListener([&screen](const int16_t cur) {
        Serial.printf("Seekbar3 canceled: %d\n", cur);
        screen.popRootView();
    });
    sb3.setMax(100);
    sb3.setMin(-100);
    sb3.setStep(5);

    // 4
    lf32.addChild(&st32);
    st32.setOnChangeListener([](const bool switched) {
        Serial.printf("Switch32: %d\n", switched);
    });

    // 5
    input.setOnDoneListener([](const String &str) {
        Serial.println(str);
    });

    screen.pushRootView(&lf1);

    scheduler.addSchedule(new sche::SchedulableKnobEvent(
        KNOB_PIN_A, KNOB_PIN_B,
        [&screen](const int delta) {
            screen.dispatchEvent(event::KnobEvent(screen, delta));
            return screen.isAlive();
        }));
    scheduler.addSchedule(new sche::SchedulableButtonEvent(
        BUTTON_PIN, [&screen](const sche::mtime_t pt) {
            screen.dispatchEvent(event::ButtonEvent(screen, static_cast<int>(pt)));
            return screen.isAlive();
        }));
    scheduler.mainloop();
}


void loop() {
}
