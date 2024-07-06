//
// Created by azazo1 on 2024/7/6.
//

#ifndef RECT_H
#define RECT_H
#include <cstdint>
#include <SSD1306Wire.h>
#include <propotion_mapping/PropotionMapping.h>
#include <sche/Scheduler.h>


namespace view {
    class Rect {
        const sche::mtime_t transitionTime;
        sche::Scheduler *scheduler = nullptr;
        const propmap::PropotionMapping *mapping;
        int16_t curX;
        int16_t curY;
        int16_t curW;
        int16_t curH;
        bool alive = true;
        SSD1306Wire *const display;
        size_t transitionBatch = 0;

    public:
        /**
         * 从不可持有对象(可安排项)到可持有对象的第一步尝试, 创建一个可以调整位置的矩形.
         * @param startX 初始 x 坐标.
         * @param startY 初始 y 坐标.
         * @param startWidth 初始宽度.
         * @param startHeight 初始高度.
         * @param transitionTime 过渡动画时间.
         * @param mapping 比例映射.
         * @param display OLED 显示屏.
         */
        Rect(int16_t startX, int16_t startY,
             int16_t startWidth, int16_t startHeight,
             sche::mtime_t transitionTime,
             const propmap::PropotionMapping *mapping, SSD1306Wire *display);

        /**
         * 初始化 Rect 的伴随可安排项(绘制矩形), 普通优先级.
         * @note 不放在构造函数是因为此方法可以用来自定义位置, 需要保证 scheduler 字段不为空.
         */
        void init() const;

        void setScheduler(sche::Scheduler* scheduler);

        /**
         * 设置新的值并伴随动画.
         * @note 四个参数给负数表示沿用当前值.
         */
        void update(int16_t newX, int16_t newY, int16_t newW, int16_t newH);

        /**
         * 标志生命周期的结束, 可安排项在 Scheduler 里自动销毁.
         */
        void destroy();

        ~Rect();
    };
} // view

#endif //RECT_H
