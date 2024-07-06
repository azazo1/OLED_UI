#include <Arduino.h>
#include <sche/SchedulableButtonEvent.h>
#include "event/ButtonEvent.h"
#include <propotion_mapping/LinearMapping.h>
#include <view/LabeledFrame.h>
#include <view/Screen.h>
#include <event/KnobEvent.h>
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

    auto primaryLabeledFrame = view::LabeledFrame();
    auto primaryTextSelector = view::TextSelector();
    auto secondaryLabeledFrame = view::LabeledFrame();
    auto secondaryTextselector = view::TextSelector();

    primaryLabeledFrame.setTitle("Hello World");

    primaryTextSelector.addItem("Hello");
    primaryTextSelector.addItem("Ni hao a");
    primaryTextSelector.addItem("I like apple");
    primaryTextSelector.addItem("Thks");
    primaryTextSelector.addItem("Debug");
    primaryTextSelector.setOnConfirmListener(
        [&screen, &primaryTextSelector, &secondaryLabeledFrame](const size_t idx) {
            Serial.print("Primary Selected: ");
            Serial.print(primaryTextSelector.itemAt(idx));
            Serial.print(", ");
            Serial.println(idx);
            screen.pushRootView(&secondaryLabeledFrame);
        });
    primaryLabeledFrame.addChild(&primaryTextSelector);

    secondaryLabeledFrame.addChild(&secondaryTextselector);
    secondaryLabeledFrame.setTitle("Second");
    secondaryTextselector.addItem("Apple");
    secondaryTextselector.addItem("Banana");
    secondaryTextselector.addItem("Orange");
    secondaryTextselector.addItem("Peach");
    secondaryTextselector.addItem("Grape");
    secondaryTextselector.setOnConfirmListener(
        [&screen, &secondaryTextselector](const size_t idx) {
            Serial.print("Secondary Selected: ");
            Serial.print(secondaryTextselector.itemAt(idx));
            Serial.print(", ");
            Serial.println(idx);
            screen.popRootView();
        });

    screen.pushRootView(&primaryLabeledFrame);

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
