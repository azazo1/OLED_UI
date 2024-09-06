//
// Created by azazo1 on 2024/9/6.
//

#ifndef TEXTINPUT_H
#define TEXTINPUT_H
#include "View.h"

#define SPACE_STR "  "
#define BACKSPACE_STR "\\b"
#define ITEM_NUM 93
constexpr char items[ITEM_NUM] = {
    '\b', ' ',

    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
    'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',

    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',

    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '_', '+', '=', ':', ';', ',',
    '.', '<', '>', '?', '/', '\\', '|', '[', ']', '{', '}', '~'
};

// 以 _ 结尾的变量都是属性动画的属性不要手动修改.
namespace view {
    /// 文本输入框, 建议将此视图用作一个 root view, 否则容易导致显示空间不足.
    /// 输入框顶部显示当前已经输入的文字, 下部显示软键盘, 包括功能键以及常用的字母.
    ///
    /// 接受按钮和旋钮事件, 短按事件来确认字符或功能的输入, 旋钮调节选择的字符或功能,
    /// 长按确认编辑并返回.
    class TextInput final : public View {
        static constexpr int16_t PADDING = 3;
        static constexpr sche::mtime_t ANIM_DURATION = 100;
        // 用于记录一个选项的占位宽度, 只更新一次.
        int16_t itemWidth = 0;
        // 用于记录选项显示框的位置.
        int16_t innerBoxX = 0;
        int16_t innerBoxY = 0;
        int16_t innerBoxWidth = 0;
        int16_t innerBoxHeight = 0;
        // 通过宽度确认需要显示的列数.
        int16_t column = 0;

        size_t animBatch = 0;

        /// 第一行选项的相对于选项显示框顶部的 y 值.
        int16_t relativeY_ = 0;
        /// 在选项移动动画中, relativeY 的变化目标值, 动画自然结束后, relativeY 和 relativeYTarget 相等.
        int16_t relativeYTarget = 0;
        /// 选中的选项序号, -1 用作初始值用来标志没有切换过选项, 会在 onDraw 中抹除此值..
        int16_t selected = -1;

        int16_t rectX_ = 0;
        int16_t rectY_ = 0;
        int16_t rectWidth_ = 0;

        /// 当前输入的文本.
        String text = String("");

        /// 光标闪烁变化间隔时间.
        const sche::mtime_t cursorFlickeringInterval = 500;
        /// 当前是否显示光标, 不要手动修改此值, 其属于属性动画的变量, 此 View 内部在 Scheduler 中循环切换此值.
        bool showCursor_ = true;
        /// 是否在 Scheduler 中注册了光标闪烁循环.
        bool registeredCursorLoop = false;

        std::function<void(String)> onDone = [](const String &) {
        };

        /// 把所有选项移动指定的有向距离.
        void animMoveItem(int16_t deltaY, sche::Scheduler &scheduler);

        /// 移动选项高亮矩形.
        void animMoveRect(int16_t x, int16_t y, int16_t width, sche::Scheduler &scheduler);

    public:
        TextInput() = default;

        String getText() const;

        void setText(String text);

        void onDraw(int16_t borderX, int16_t borderY, int16_t borderW, int16_t borderH,
                    SSD1306Wire *display, sche::Scheduler *scheduler) override;

        bool dispatchEvent(const event::Event &event) override;

        /// 改变当前高亮的选项, 并显示动画, 如果参数越界, 则取边界.
        void performSelectionChange(int16_t newSelected, SSD1306Wire &display,
                                    sche::Scheduler &scheduler);

        /// 输入框退出时触发.
        void setOnDoneListener(std::function<void(String)> listener);
    };
} // view

#endif //TEXTINPUT_H
