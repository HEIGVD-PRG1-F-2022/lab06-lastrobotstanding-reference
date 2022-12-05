//
// Created by Linus Gasser on 04.12.22.
//

#include "Bonus.h"

Bonus::Bonus(size_t width, size_t height, BonusType type, unsigned value): type(type), value(value){
    pos = Position::random(unsigned(width), unsigned(height), width, height);
}
