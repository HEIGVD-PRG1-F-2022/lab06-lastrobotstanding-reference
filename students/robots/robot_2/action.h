/*
-----------------------------------------------------------------------------------
Project Name    : lab06-lastrobotstanding-comme_tu_veux
File's Name     : action.h
Author          :   Aellen Quentin
                    Atasever Mehmet
                    Salamin Chloé
Creation Date   : 02.12.2022
Description     :
Remarque(s)     :
Compiler        : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
*/
#ifndef ROBOT_ACTION_H
#define ROBOT_ACTION_H

#include <string>
#include "point.h"

const std::string BOARD_STRING = "board";
const std::string DAMAGE_STRING = "damage";
const std::string MOVE_STRING = "move";
const std::string ATTACK_STRING = "attack";
const std::string WAIT_STRING = "wait";

class Action {
public:
    enum Name {
        NOTDEFINED,
        BOARD,
        DAMAGE,
        MOVE,
        ATTACK,
        WAIT,
        BONUS
    };

    static Name resolveAction(std::string &action);

    static std::string generateDamage(Point coords, unsigned int energy);
};

#endif //ROBOT_ACTION_H
