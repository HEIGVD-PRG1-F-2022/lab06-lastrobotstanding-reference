//
// Created by flori on 07/12/2022.
//

#include "MapInfo.h"

MapInfo::MapInfo(size_t width, size_t height, size_t radiusCheck)
{
    this->width = width;
    this->height = height;
    this->radiusCheck = radiusCheck;
}

MapInfo::MapInfo()
{
    this->width = 0;
    this->height = 0;
}

void MapInfo::reset()
{
    inRangeBonus.clear();
    inRangeRobots.clear();
    numRobotsInRange = 0;
    numBonusInRange = 0;
}

void MapInfo::registerInRangeObjects()
{
    reset();
    for (size_t y = 0; y < currentBoard.size(); ++y)
    {
        for (size_t x = 0; x < currentBoard.at(y).size(); ++x)
        {
            auto value = currentBoard.at(y).at(x);
            if(value == "R")
            {
                inRangeRobots.emplace_back(long(x-radiusCheck), long(y-radiusCheck));
                numRobotsInRange += 1;
            }
            else if(value == "B")
            {
                inRangeBonus.emplace_back(long(x-radiusCheck), long(y-radiusCheck));
//                std::cout << "bonus at : " << inRangeBonus.at(0) << std::endl;
                numBonusInRange += 1;
            }
        }
    }
}

//when robot move, we must update the new coord
void MapInfo::updateInformations()
{
    registerInRangeObjects();
    if(lastMove == Point(0,0)) return;
    updateBonus(lastMove);
    lastMove = Point(0,0);
}

//BONUS RELATED

void MapInfo::updateBonus(const Point &move)
{
    std::for_each(bonus.begin(),bonus.end(), [move](auto& item){item -= move;});
    //std::vector<Point> bonusCoords = mapInfo.getInRangeCoords(EntityType::Bonus);

    for(auto &i : bonus) //Update if bonus is still alive(base on fild of view)
    {
        if(i.mag() > 2) continue;
        bool underRobot = i == Point(0,0);
        bool pickByEnemy = std::find(inRangeBonus.begin(), inRangeBonus.end(), i) != inRangeBonus.end();

        if(!(underRobot || pickByEnemy)) continue;
        std::remove(bonus.begin(), bonus.end(),i);
    }

}

void MapInfo::addBonus(const Point &coord)
{
    bool alreadyExist = std::any_of(bonus.begin(), bonus.end(),[coord](const auto& item){return item == coord;});
    if(alreadyExist) return;
    bonus.push_back(coord);
}

void MapInfo::setLastMove(const Point &move)
{
    lastMove = move;
}


