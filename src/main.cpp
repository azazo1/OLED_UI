#include <Arduino.h>
#include <propotion_mapping/LinearMapping.h>
#include <sche/SequenceSchedulable.h>
#include <view/LabeledFrame.h>
#include <view/Rect.h>
#include <view/Screen.h>
#include <view/TextView.h>

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
    view::Screen screen(&display);
    screen.attachToScheduler(&scheduler);
    const auto labeledFrame = new view::LabeledFrame();
    const auto insideLabeledFrame = new view::LabeledFrame();
    const auto textView = new view::TextView();
    insideLabeledFrame->setTitle("Inside");
    textView->setText("Ni Hao");
    labeledFrame->setTitle("Hello World");
    labeledFrame->addChild(insideLabeledFrame);
    insideLabeledFrame->addChild(textView);
    screen.setRootView(labeledFrame);

    display.init();
    display.setBrightness(50);
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_24);

    scheduler.addSchedule(new sche::SchedulableFromLambda([](sche::mtime_t) {
        display.clear();
        return true;
    }), PRIORITY_HIGH);
    scheduler.addSchedule(new sche::SchedulableFromLambda([](sche::mtime_t) {
        display.display();
        return true;
    }), PRIORITY_LOW);

    scheduler.mainloop();
}


void loop() {
}
