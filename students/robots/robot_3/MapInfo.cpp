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
    numRobotsInRange = 0;
    numBonusInRange = 0;
    numRobotDetected = 0;
}

void MapInfo::registerInRangeObjects() {
    for (size_t y = 0; y < currentBoard.size(); ++y) {
        for (size_t x = 0; x < currentBoard.at(y).size(); ++x) {
            auto value = currentBoard.at(y).at(x);
            if (value == "R") {
                inRangeRobots.emplace_back(long(x - radiusCheck), long(y - radiusCheck));
                numRobotsInRange += 1;
            } else if (value == "B") {
                inRangeBonus.emplace_back(long(x - radiusCheck), long(y - radiusCheck));
                numBonusInRange += 1;
            }
        }
    }
}

//when robot move, we must update the new coord
void MapInfo::updateInformations() {
    reset();
    registerInRangeObjects();
    if (lastMove == Point2D(0, 0)) return;

    updateBonus(lastMove);
    lastMove = Point2D(0, 0);
}

// ========================================================================================================
//BONUS RELATED
// ========================================================================================================

void MapInfo::updateBonusOnMap(const Point2D &enemyPosition) {
    auto founder = std::find(bonus.begin(), bonus.end(), enemyPosition);
    if (founder == bonus.end()) return;
    std::remove(bonus.begin(), bonus.end(), enemyPosition);
}

void MapInfo::updateBonus(const Point2D &move) {
    std::for_each(bonus.begin(), bonus.end(), [move](auto &item) { item -= move; });
    for (auto &i: bonus)//Update if bonus is still alive(base on fild of view)
    {
        if (i.mag() > 2) continue;

        //OUR robot take the bonus
        bool underRobot = i == Point2D(0, 0);
        //ENEMY take the bonus
        bool pickByEnemy = std::find(inRangeBonus.begin(), inRangeBonus.end(), i) != inRangeBonus.end();

        if (!(underRobot || pickByEnemy)) continue;
        std::remove(bonus.begin(), bonus.end(), i);
        //MUST CHANGE STATE TO SEARCH
    }
}

void MapInfo::addBonus(const Point2D &coord) {
    bool alreadyExist = std::any_of(bonus.begin(), bonus.end(), [coord](const auto &item) { return item == coord; });
    if (alreadyExist) return;
    bonus.push_back(coord);
}

void MapInfo::setLastMove(const Point2D &move) {
    lastMove = move;
}
