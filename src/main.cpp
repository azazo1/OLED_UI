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

#define FONT_DATA ArialMT_Plain_16

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

    display.init();
    display.setBrightness(50);
    display.flipScreenVertically();
    view::View::setFont(FONT_DATA);
    display.setFont(FONT_DATA);

    const auto labeledFrame = new view::LabeledFrame();
    labeledFrame->setTitle("Hello World");

    const auto textSelector = new view::TextSelector();
    textSelector->addItem("Hello");
    textSelector->addItem("Ni hao a");
    textSelector->addItem("I like apple");
    textSelector->addItem("Thks");
    textSelector->addItem("Debug");
    textSelector->setOnConfirmListener([](const String &text) {
        Serial.println("Selected: " + text);
    });

    labeledFrame->addChild(textSelector);

    screen.setRootView(labeledFrame);

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
