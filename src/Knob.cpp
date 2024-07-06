//
// Created by azazo1 on 2024/7/5.
//

#include "Knob.h"

#include <Arduino.h>
#include <esp32-hal-gpio.h>
#include <future>

id_t seq = 0;


namespace knob {
    std::unordered_map<id_t, std::pair<int, int> > Knob::interruptMap = {};
    std::unordered_map<id_t, int> Knob::deltaMap = {};
    std::unordered_map<id_t, int> Knob::lastState = {};
    hw_timer_t *Knob::timer = nullptr;

    void IRAM_ATTR Knob::routine() {
        for (const auto p: interruptMap) {
            const auto id = p.first;
            const auto pins = p.second;
            const int pinAr = digitalRead(pins.first);
            const int pinBr = digitalRead(pins.second);
            if (!pinAr && pinBr) {
                if (lastState.at(id) == 0) {
                    deltaMap.at(id) += 1;
                    lastState.at(id) = 1;
                }
            } else if (!pinBr && pinAr) {
                if (lastState.at(id) == 0) {
                    deltaMap.at(id) -= 1;
                    lastState.at(id) = -1;
                }
            } else if (pinBr && pinAr) {
                lastState.at(id) = 0;
            }
        }
    }

    Knob::Knob(const int pinA, const int pinB): id(seq++), pinA(pinA), pinB(pinB) {
        deltaMap.insert(std::pair<id_t, int>(id, 0));
        interruptMap.insert(std::pair<id_t, std::pair<int, int> >(
            id, std::pair<int, int>(pinA, pinB)
        ));
        lastState.insert(std::pair<id_t, int>(id, 0));
        pinMode(pinA, INPUT);
        pinMode(pinB, INPUT);
        if (timer == nullptr) {
            timer = timerBegin(0, 80, true); // 80MHz / 80 = 1MHz (也就是 1us)
            timerAttachInterrupt(timer, routine, true);
            timerAlarmWrite(timer, 1 * 1000, true);
            // alarm_value us 触发 routine 一次, 然后自动重装, 使 routine 反复执行.
            timerAlarmEnable(timer);
        }
    }

    int Knob::delta() {
        const int old = deltaMap.at(id);
        deltaMap.at(id) = 0;
        accumulated += old;
        return old;
    }

    int Knob::getAccumulated() {
        delta();
        return accumulated;
    }

    void Knob::deinit() const {
        deltaMap.erase(id);
        interruptMap.erase(id);
        lastState.erase(id);
        detachInterrupt(digitalPinToInterrupt(pinA));
        detachInterrupt(digitalPinToInterrupt(pinB));
        pinMode(pinA, INPUT);
        pinMode(pinB, INPUT);
        if (interruptMap.empty() && timer != nullptr) {
            timerDetachInterrupt(timer);
            timerEnd(timer);
            timer = nullptr;
        }
    }

    Knob::~Knob() {
        deinit();
    }
} // knob
