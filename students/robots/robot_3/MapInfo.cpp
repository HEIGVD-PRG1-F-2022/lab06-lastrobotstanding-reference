//
// Created by flori on 07/12/2022.
//

#include "MapInfo.h"

MapInfo::MapInfo(size_t width, size_t height, size_t radiusCheck) {
    this->width = width;
    this->height = height;
    this->radiusCheck = radiusCheck;
}

MapInfo::MapInfo() {
    this->width = 0;
    this->height = 0;
}

void MapInfo::reset() {
    inRangeBonus.clear();
    inRangeRobots.clear();
}

void MapInfo::registerInRangeObjects(const std::vector<std::vector<std::string>>& board) {
    for (size_t y = 0; y < board.size(); ++y) {
        for (size_t x = 0; x < board.at(y).size(); ++x) {
            auto value = board.at(y).at(x);
            Point2D itemPosition = Point2D(long(x - radiusCheck), long(y - radiusCheck));
            if (value == "R") {
                inRangeRobots.push_back(itemPosition);
                updateBonusOnMap(itemPosition);
            } else if (value == "B") {
                inRangeBonus.push_back(itemPosition);
            }
        }
    }
}

//when robot move, we must update the new coord
void MapInfo::updateInformations(const std::vector<std::vector<std::string>>& board) {
    reset();
    registerInRangeObjects(board);
}

// ========================================================================================================
// BONUS RELATED
// ========================================================================================================

void MapInfo::updateBonusOnMap(const Point2D &enemyPosition) {
    auto founder = std::find(bonus.begin(), bonus.end(), enemyPosition);
    if (founder == bonus.end()) return;
    //bonus.erase(std::remove(bonus.begin(), bonus.end(), enemyPosition), bonus.end());
    bonus.erase(std::find(bonus.begin(), bonus.end(), enemyPosition), bonus.end());
}

bool MapInfo::updateBonus(const Point2D &move) {
    std::for_each(bonus.begin(), bonus.end(), [move](auto &item) { item -= move; });

    bool underRobot = false;
    bool pickByEnemy = false;

    bonus.erase( std::remove_if( bonus.begin(), bonus.end(),
    [this, &underRobot, &pickByEnemy](auto &item)
    {
        underRobot = item == Point2D(0, 0);
        pickByEnemy = false;
        for(auto i : inRangeRobots)
        {
            Point2D relativPosition = Point2D(i.getX() - radiusCheck , i.getY() - radiusCheck);
            if(relativPosition != item) continue;
            pickByEnemy = true;
        }
        //bool pickByEnemy = std::find(inRangeRobots.begin(), inRangeRobots.end(), item) != inRangeRobots.end();
        return underRobot || pickByEnemy;
    } ), bonus.end() );
    return !(underRobot || pickByEnemy);
}