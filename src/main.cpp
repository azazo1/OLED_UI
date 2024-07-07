#include <Arduino.h>
#include <sche/SchedulableButtonEvent.h>
#include "event/ButtonEvent.h"
#include <propotion_mapping/LinearMapping.h>
#include <view/LabeledFrame.h>
#include <view/Screen.h>
#include <event/KnobEvent.h>
#include <view/Seekbar.h>
#include <view/TextSelector.h>

#include "sche/Schedulable.h"
#include "sche/SchedulableFromLambda.h"
#include "sche/Scheduler.h"
#include "SSD1306Wire.h"
#include "Knob.h"

#define KNOB_PIN_A GPIO_NUM_32
#define KNOB_PIN_B GPIO_NUM_35
#define BUTTON_PIN 23

#define FONT_DATA ArialMT_Plain_10

auto display = SSD1306Wire(0x3C, SDA, SCL);

int rectWidth = 128;
int rectHeight = 16;

void drawString(const int16_t secy) {
    display.drawString(40, 0, "hello world");
    display.drawString(40, secy, "nihao shijie");
}


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

    auto lf1 = view::LabeledFrame();
    auto ts1 = view::TextSelector();
    auto lf2 = view::LabeledFrame();
    auto ts2 = view::TextSelector();
    auto lf3 = view::LabeledFrame();
    auto sb3 = view::Seekbar();

    lf1.setTitle("Hello World");

    ts1.addItem("Hello");
    ts1.addItem("Ni hao a");
    ts1.addItem("I like apple");
    ts1.addItem("Thks");
    ts1.addItem("Debug");
    ts1.setOnConfirmListener(
        [&screen, &ts1, &lf2](const size_t idx) {
            Serial.print("Primary Selected: ");
            Serial.print(ts1.itemAt(idx));
            Serial.print(", ");
            Serial.println(idx);
            screen.pushRootView(&lf2);
        });
    lf1.addChild(&ts1);

    lf2.addChild(&ts2);
    lf2.setTitle("Second");
    ts2.addItem("Apple");
    ts2.addItem("Banana");
    ts2.addItem("Orange");
    ts2.addItem("Peach");
    ts2.addItem("Grape");
    ts2.setOnConfirmListener(
        [&screen, &ts2, &lf3](const size_t idx) {
            Serial.print("Secondary Selected: ");
            Serial.print(ts2.itemAt(idx));
            Serial.print(", ");
            Serial.println(idx);
            screen.pushRootView(&lf3);
        });

    lf3.setTitle("Seekbar");
    lf3.addChild(&sb3);
    sb3.setOnChangeListener([&lf3](const int16_t cur) {
        Serial.printf("Seekbar3,now: %d\n", cur);
        lf3.setTitle(String("Seekbar: ") + cur);
    });
    sb3.setOnConfirmListener([&screen](const int16_t cur) {
        Serial.printf("Seekbar3: %d\n", cur);
        screen.popRootView();
    });
    sb3.setMax(100);
    sb3.setMin(-100);
    sb3.setStep(5);

    screen.pushRootView(&lf1);

    scheduler.addSchedule(new sche::SchedulableFromLambda([&screen](sche::mtime_t) {
        display.clear();
        return screen.isAlive();
    }), PRIORITY_HIGH);
    scheduler.addSchedule(new sche::SchedulableFromLambda(
        [&screen, &kb](sche::mtime_t) {
            const auto delta = kb.delta();
            if (delta != 0) {
                screen.dispatchEvent(event::KnobEvent(screen, delta));
            }
            return screen.isAlive();
        }
    ));
    scheduler.addSchedule(new sche::SchedulableButtonEvent(
        BUTTON_PIN, [&screen](const sche::mtime_t pt) {
            screen.dispatchEvent(event::ButtonEvent(screen, static_cast<int>(pt)));
            return screen.isAlive();
        }));
    scheduler.addSchedule(new sche::SchedulableFromLambda([&screen](sche::mtime_t) {
        display.display();
        return screen.isAlive();
    }), PRIORITY_LOW);

    scheduler.mainloop();
}


void loop() {
}
