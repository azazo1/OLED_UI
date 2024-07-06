//
// Created by azazo1 on 2024/7/6.
//

#include "View.h"

namespace view {
    const uint8_t *View::font = ArialMT_Plain_10;
    int16_t View::lineHeight = ArialMT_Plain_10[1];

    void View::setFont(const uint8_t *newFont) {
        font = newFont;
        lineHeight = newFont[1];
    }
} // view
