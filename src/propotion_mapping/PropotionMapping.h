//
// Created by azazo1 on 2024/7/6.
//

#ifndef PROPOTIONMAPPING_H
#define PROPOTIONMAPPING_H

namespace propmap {
    class PropotionMapping {
    public:
        /**
         * 值映射.
         * @param propotion [0, 1] 区间内的值.
         * @return 映射后, [0, 1] 区间内的值.
         */
        virtual double mapping(double propotion) const = 0;

        virtual ~PropotionMapping() = default;
    };
} // propmap

#endif //PROPOTIONMAPPING_H
