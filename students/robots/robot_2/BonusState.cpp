// ================================================================================
// File : BonusState.cpp
// Project name : LABO - LastRobotStanding
// Project members :
// - Florian Duruz, Antoine Leresche
// File created by Florian Duruz, Antoine Leresche
// ================================================================================

//
// Created by Florian Duruz, Antoine Leresche on 14/12/2022.
//

#include "BonusState.h"

Point BonusState::updateState(MapInfo info)
{
    //Check In range
    Point target;
    if(info.getNumBonusInRange() == 0) //CHECK spawned bonus if none are in field of view
    {
        int shortestDistance = std::numeric_limits<int>::max();
        for (const auto& outVisionCoord : info.bonus)
        {
            if(outVisionCoord.mag() < shortestDistance)
            {
                target = outVisionCoord;
            }
        }
    }
    else if(info.getNumBonusInRange() == 1) // If only one in range go for it
    {
        target = info.getInRangeBonus().at(0);
        //std::// << "target "  << target.getX()<<std::endl;
    }
    else if(info.getNumBonusInRange() > 1) // if many check distances
    {
        int shortestDistance = std::numeric_limits<int>::max();
        for (const auto& bonusCoord : info.getInRangeBonus())
        {
            target = bonusCoord.mag() < shortestDistance ? bonusCoord : target;
        }
    }
    return target.normalize();
}
