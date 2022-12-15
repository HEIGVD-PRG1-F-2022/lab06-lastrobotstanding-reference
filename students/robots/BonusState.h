// ================================================================================
// File : BonusState.h
// Project name : LABO - LastRobotStanding
// Project members :
// - Florian Duruz
// File created by Florian Duruz
// ================================================================================

//
// Created by Florian Duruz on 14/12/2022.
//

#ifndef LASTROBOTSTANDING_BONUSSTATE_H
#define LASTROBOTSTANDING_BONUSSTATE_H

#include "vector"
#include "string"
#include "limits"
#include "MapInfo.h"

class BonusState
{

public:
    BonusState(){}

    Point updateState(MapInfo info);
};


#endif //LASTROBOTSTANDING_BONUSSTATE_H
