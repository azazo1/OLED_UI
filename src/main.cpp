#include <Arduino.h>
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
    unsigned long pressedTime = 0;

    display.init();
    display.setBrightness(50);
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_24);

    scheduler.addSchedule(new sche::SchedulableFromLambda([](sche::mtime_t) {
        display.clear();
        return true;
    }), PRIORITY_HIGH);
    // scheduler.addSchedule(new sche::SchedulableFromLambda([&kb](sche::mtime_t) {
    // drawString(static_cast<int16_t>(kb.getAccumulated()));
    // return true;
    // }), PRIORITY_PLAIN);
    scheduler.addSchedule(new sche::RectTransformation(
        0, 100,
        0, 30,
        10, 30,
        10, 40,
        5000,
        [](const int16_t x, const int16_t y, const int16_t w, const int16_t h) {
            display.fillRect(x, y, w, h);
        }
    ));
    scheduler.addSchedule(new sche::ButtonEvent(
        BUTTON_PIN,
        [&pressedTime](const sche::mtime_t pt) {
            pressedTime = pt;
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
