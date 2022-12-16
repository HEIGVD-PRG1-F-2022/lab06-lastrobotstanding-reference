/*
-----------------------------------------------------------------------------------
Project Name    : lab06-lastrobotstanding-comme_tu_veux
File's Name     : action.cpp
Author          :   Aellen Quentin
                    Atasever Mehmet
                    Salamin Chloé
Creation Date   : 02.12.2022
Description     :
Remarque(s)     :
Compiler        : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
*/

#include "action.h"

using namespace std;

Action::Name Action::resolveAction(std::string &action) {
    if (action == BOARD_STRING)
        return Action::Name::BOARD;
    if (action == DAMAGE_STRING)
        return Action::Name::DAMAGE;
    if (action == MOVE_STRING)
        return Action::Name::MOVE;
    if (action == ATTACK_STRING)
        return Action::Name::ATTACK;
    if (action == WAIT_STRING)
        return Action::Name::WAIT;

    return Action::Name::NOTDEFINED;
}

std::string Action::generateDamage(Point3 coords, unsigned int energy) {
    return DAMAGE_STRING + " " + (std::string) coords + "," + std::to_string(energy);
}
