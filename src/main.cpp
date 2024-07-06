#include <Arduino.h>
#include <RectTransformation.h>

#include "Scheduler.h"
#include "Schedulable.h"
#include "SchedulableFromLambda.h"
#include "SSD1306Wire.h"
#include "Knob.h"

#define KNOB_PIN_A GPIO_NUM_32
#define KNOB_PIN_B GPIO_NUM_35

auto display = SSD1306Wire(0x3C, SDA, SCL);
knob::Knob *kb = nullptr;
sche::Scheduler *scheduler = nullptr;

int rectWidth = 128;
int rectHeight = 16;

void drawString(const int16_t secy) {
    display.drawString(40, 0, "hello world");
    display.drawString(40, secy, "nihao shijie");
}


void setup() {
    Serial.begin(9600);
    kb = new knob::Knob(KNOB_PIN_A, KNOB_PIN_B);
    scheduler = new sche::Scheduler();
    display.init();
    display.setBrightness(50);
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_24);
    scheduler->addSchedule(new sche::SchedulableFromLambda([](sche::mtime_t) {
        display.clear();
        return true;
    }), PRIORITY_HIGH);
    scheduler->addSchedule(new sche::SchedulableFromLambda([](sche::mtime_t) {
        drawString(static_cast<int16_t>(kb->getAccumulated()));
        return true;
    }), PRIORITY_PLAIN);
    scheduler->addSchedule(new sche::RectTransformation(
        0, 100,
        0, 30,
        10, 30,
        10, 40,
        5000,
        [](const int16_t x, const int16_t y, const int16_t w, const int16_t h) {
            display.fillRect(x, y, w, h);
        }
    ));
    scheduler->addSchedule(new sche::SchedulableFromLambda([](sche::mtime_t) {
        display.display();
        return true;
    }), PRIORITY_LOW);
}


void loop() {
    scheduler->mainloop();
}
