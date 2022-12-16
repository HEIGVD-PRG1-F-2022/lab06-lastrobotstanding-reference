// ================================================================================
// File : MapInfo.h
// Project name : LABO - LastRobotStanding
// Project members :
// - Florian Duruz
// File created by Florian Duruz
// ================================================================================

//
// Created by Florian Duruz on 07/12/2022.
//

#ifndef LASTROBOTSTANDING_MAPINFO_H
#define LASTROBOTSTANDING_MAPINFO_H

#include "Point.h"
#include "vector"
#include "string"
#include "algorithm"


class MapInfo
{
    Point lastMove;

    std::vector<Point> inRangeRobots;
    std::vector<Point> inRangeBonus;

    size_t radiusCheck = 0;
    size_t width = 0;
    size_t height = 0;

    size_t numRobotsInRange = 0;
    size_t numBonusInRange = 0;

    void updateBonus(const Point& lastMove);
public:
    std::vector<std::vector<std::string>> previousRadar;
    std::vector<std::vector<std::string>> currentRadar;

    std::vector<std::vector<std::string>> previousBoard;
    std::vector<std::vector<std::string>> currentBoard;

    std::vector<Point> bonus;
    std::vector<Point> enemyCoords;

    MapInfo();
    MapInfo(size_t width, size_t height, size_t radiusCheck);

    [[nodiscard]] size_t getNumRobotsInRange() const {return numRobotsInRange;}
    [[nodiscard]] size_t getNumBonusInRange() const {return numBonusInRange;}

    [[nodiscard]] std::vector<Point> getInRangeRobots()const {return inRangeRobots;}
    [[nodiscard]] std::vector<Point> getInRangeBonus()const {return inRangeRobots;}

    void setLastMove(const Point& move);
    void updateInformations();
    void reset();
    void registerInRangeObjects();

    void addBonus(const Point& coord);
};

#endif //LASTROBOTSTANDING_MAPINFO_H
