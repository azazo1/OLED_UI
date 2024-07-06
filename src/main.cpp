#include <Arduino.h>
#include <propotion_mapping/LinearMapping.h>
#include <sche/SequenceSchedulable.h>
#include <view/Rect.h>

#include "sche/RectTransformation.h"
#include "sche/Schedulable.h"
#include "sche/SchedulableFromLambda.h"
#include "sche/Scheduler.h"
#include "sche/ButtonEvent.h"
#include "SSD1306Wire.h"
#include "Knob.h"

#define KNOB_PIN_A GPIO_NUM_32
#define KNOB_PIN_B GPIO_NUM_35
#define BUTTON_PIN 23

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
    unsigned long pressedTime = 0;

    view::Rect rect(0, 0, 10, 10, 1000, &linear_mapping, &display);

    display.init();
    display.setBrightness(50);
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_24);

    scheduler.addSchedule(new sche::SchedulableFromLambda([](sche::mtime_t) {
        display.clear();
        return true;
    }), PRIORITY_HIGH); {
        // subScheduler 不能在外面使用.
        auto *subScheduler = new sche::Scheduler();
        subScheduler->setRemain(true);
        scheduler.addSchedule(subScheduler, PRIORITY_PLAIN);
        rect.setScheduler(subScheduler);
        rect.init();
    }

    scheduler.addSchedule(new sche::ButtonEvent(
        BUTTON_PIN,
        [&pressedTime, &rect](const sche::mtime_t pt) {
            pressedTime = pt;
            if (pressedTime > 1000) {
                rect.update(0, 0, 10, 10);
            } else if (pressedTime > 200) {
                // 矩形移动到右侧
                rect.update(-1, -1, 20, 30);
            } else {
                // 矩形移动到左侧
                rect.update(100, 30, -1, -1);
            }
            Serial.printf("Pressed: %lu\n", pt);
            return true;
        }
    ));
    scheduler.addSchedule(new sche::SchedulableFromLambda(
        [&pressedTime](const sche::mtime_t dt) {
            String s = "Btn: ";
            s.concat(pressedTime);
            display.drawString(0, 0, s);
            return true;
        }));
    scheduler.addSchedule(new sche::SchedulableFromLambda([](sche::mtime_t) {
        display.display();
        return true;
    }), PRIORITY_LOW);

    scheduler.mainloop();
}


void loop() {
}
