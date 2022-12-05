//
// Created by Linus Gasser on 04.12.22.
//

#ifndef INEITI_BONUS_H
#define INEITI_BONUS_H

#include "Position.h"

enum class BonusType{
    Energy,
    Power,
};

class Bonus {
public:
    Position pos;
    BonusType type;
    unsigned value;
    explicit Bonus(size_t width = 10, size_t height = 10, BonusType type = BonusType::Energy, unsigned value = 1);
};


#endif//INEITI_BONUS_H
