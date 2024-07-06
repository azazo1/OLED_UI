//
// Created by azazo1 on 2024/7/6.
//

#include "RectTransformation.h"

#include <utility>

namespace sche {
    RectTransformation::RectTransformation(
        const int16_t startX, const int16_t endX,
        const int16_t startY, const int16_t endY,
        const int16_t startWidth, const int16_t endWidth,
        const int16_t startHeight, const int16_t endHeight,
        const mtime_t durationMillis,
        PainterType painter
    ): startX(startX), endX(endX),
       startY(startY), endY(endY),
       startWidth(startWidth), endWidth(endWidth),
       startHeight(startHeight), endHeight(endHeight),
       durationMillis(durationMillis),
       painter(std::move(painter)) {
    }

    bool RectTransformation::schedule(const mtime_t deltaTime) {
        passedTime += deltaTime;
        const double passedRatio = 1.0 * passedTime / durationMillis;
        const auto x = static_cast<int16_t>(startX + (endX - startX) * passedRatio);
        const auto y = static_cast<int16_t>(startY + (endY - startY) * passedRatio);
        const auto w = static_cast<int16_t>(startWidth + (endWidth - startWidth) * passedRatio);
        const auto h = static_cast<int16_t>(startHeight + (endHeight - startHeight) * passedRatio);
        painter(x, y, w, h);
        return passedTime < durationMillis;
    }
} // sche
