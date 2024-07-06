//
// Created by azazo1 on 2024/7/6.
//

#ifndef RECTTRANSFORMATION_H
#define RECTTRANSFORMATION_H
#include <cstdint>
#include <Schedulable.h>
#include <Scheduler.h>

namespace sche {
    typedef std::function<void(int16_t, int16_t, int16_t, int16_t)> PainterType;

    class RectTransformation final : public Schedulable {
        const int16_t startX;
        const int16_t endX;
        const int16_t startY;
        const int16_t endY;
        const int16_t startWidth;
        const int16_t endWidth;
        const int16_t startHeight;
        const int16_t endHeight;
        const mtime_t durationMillis;
        const PainterType painter;

        mtime_t passedTime = 0;

    public:
        RectTransformation(
            int16_t startX, int16_t endX,
            int16_t startY, int16_t endY,
            int16_t startWidth, int16_t endWidth,
            int16_t startHeight, int16_t endHeight,
            mtime_t durationMillis,
            PainterType painter
        );

        bool schedule(mtime_t deltaTime) override;
    };
} // sche

#endif //RECTTRANSFORMATION_H
