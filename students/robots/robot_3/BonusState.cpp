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

//MUST CACHE CURRENT TARGET SO WE CHANGE STATE HERE
//1) FIND CURRENT TARGET if fale => Change to search
Point2D BonusState::updateState(MapInfo &info) {
    //Check In range
    //std::cout << info.bonus.size() << std::endl;
    Point2D target = Point2D(0,0);
    if(info.bonus.empty() && info.getInRangeBonus().empty())
    {
        return target;
    }

    if (info.getInRangeBonus().empty()){ //CHECK spawned bonus if none are in field of view
        double shortestDistance = std::numeric_limits<double>::max();
        for (const auto &outVisionCoord: info.bonus) {
            double distance = outVisionCoord.mag();
            if(distance < shortestDistance ) {
                target = outVisionCoord;
                shortestDistance = distance;
            }
        }
    }
    else if (info.getInRangeBonus().size() == 1){ // If only one in range go for it
        target = info.getInRangeBonus().front();
    }
    else if (info.getInRangeBonus().size() > 1){ // if many check distances
        double shortestDistance = std::numeric_limits<double>::max();
        for (const auto &bonusCoord: info.getInRangeBonus()) {
            double distance = bonusCoord.mag();
            if(distance < shortestDistance ) {
                target = bonusCoord;
                shortestDistance = distance;
            }
        }
    }
    return target;
}
