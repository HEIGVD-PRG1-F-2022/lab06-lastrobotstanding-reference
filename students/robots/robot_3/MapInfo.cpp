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
    numRobotDetected = 0;
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
    //if (lastMove == Point2D(0, 0)) return;

    //updateBonus(lastMove);
    //lastMove = Point2D(0, 0);
}

// ========================================================================================================
// BONUS RELATED
// ========================================================================================================

void MapInfo::updateBonusOnMap(const Point2D &enemyPosition) {
    auto founder = std::find(bonus.begin(), bonus.end(), enemyPosition);
    if (founder == bonus.end()) return;
    bonus.erase(std::remove(bonus.begin(), bonus.end(), enemyPosition), bonus.end());
}

void MapInfo::updateBonus(const Point2D &move) {
    std::for_each(bonus.begin(), bonus.end(), [move](auto &item) { item -= move; });

    bonus.erase( std::remove_if( bonus.begin(), bonus.end(),
    [this](const auto &item)
    {
        bool underRobot = item == Point2D(0, 0);
        bool pickByEnemy = std::find(inRangeBonus.begin(), inRangeBonus.end(), item) != inRangeBonus.end();
        return underRobot || pickByEnemy;
    } ), bonus.end() );
/*
    for (auto &i: bonus)//Update if bonus is still alive(base on fild of view)
    {
        if (i.mag() > 2) continue;
        //OUR robot take the bonus
        bool underRobot = (i == Point2D(0, 0));
        //ENEMY take the bonus
        bool pickByEnemy = std::find(inRangeBonus.begin(), inRangeBonus.end(), i) != inRangeBonus.end();

        if (!(underRobot || pickByEnemy)) continue;
        bonus.erase(std::remove(bonus.begin(), bonus.end(), i), bonus.end());
        //std::remove(bonus.begin(), bonus.end(), i);
    }
    */
}

void MapInfo::addBonus(const Point2D &coord) {
    bonus.clear();
    bool alreadyExist = std::any_of(bonus.begin(), bonus.end(), [coord](const auto &item) { return item == coord; });
    if (alreadyExist) return;
    bonus.push_back(coord);
}
/*
void MapInfo::setLastMove(const Point2D &move) {
    lastMove = move;
}
*/