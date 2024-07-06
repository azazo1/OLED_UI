//
// Created by azazo1 on 2024/7/6.
//

#ifndef LINEARMAPPING_H
#define LINEARMAPPING_H
#include "PropotionMapping.h"

namespace propmap {
    class LinearMapping final : public PropotionMapping {
        /**
         * 线性映射.
         *
         * y = x
         */
        double mapping(double propotion) const override;
    };
} // propmap

#endif //LINEARMAPPING_H
