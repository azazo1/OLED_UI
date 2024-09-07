# ESP32 OLED UI 实现

这是一个比较简陋的在 ESP32 + SSD1306 上实现的事件驱动的用户交互界面.

主要功能有:

1. 使用了类似协程的思想进行任务的调度(我把其命名为 Schedule), 并以此实现视图的动画, 外设的输入处理.
2. 能基于事件驱动来进行 UI 交互.
3. 视图内能实现动画效果, 可自定义动画曲线.

此 UI 目前实现了一下视图(View):

- Switch: 开关.
- LabeledFrame: 带标题的视图容器.
- TextSelector: 文本选择器.
- Seekbar: 拖动条.
- TextView: 文本.
- TextInput: 文本输入框.

实现了以下事件:

- 旋转编码器事件: 记录了旋钮的转动格数, 有正负两个方向.
- 按钮事件: 按钮释放时触发, 记录了按下按钮的时间长短.

使用到的库:

- [esp8266-oled-ssd1306](https://github.com/ThingPulse/esp8266-oled-ssd1306)