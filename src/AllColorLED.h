//
// Created by azazo1 on 2024/7/7.
//

#ifndef ALLCOLORLED_H
#define ALLCOLORLED_H

namespace acled {
    class AllColorLED {
        int rPin;
        int gPin;
        int bPin;
    public:
        /// 颜色是按照共阴极的全色 LED 来的.
        AllColorLED(int rPin, int gPin, int bPin);

        void setR(unsigned char r) const;

        void setG(unsigned char g) const;

        void setB(unsigned char b) const;

        void setRGB(unsigned char r, unsigned char g, unsigned char b) const;

        ~AllColorLED();
    };
} // acled

#endif //ALLCOLORLED_H
