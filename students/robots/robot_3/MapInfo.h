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

#include <algorithm>
#include <string>
#include <vector>
//#include "MeinRobot.h"
#include "Point2D.h"


class MapInfo {
    //Use by radar
    size_t numRobotDetected = 0;

    std::vector<Point2D> inRangeRobots;
    std::vector<Point2D> inRangeBonus;

    size_t radiusCheck = 0;
    size_t width = 0;
    size_t height = 0;

    void updateBonus(const Point2D &lastMove);

public:
    Point2D lastMove;

    std::vector<std::vector<std::string>> currentRadar;
    std::vector<std::vector<std::string>> currentBoard;

    std::vector<Point2D> bonus;
    std::vector<Point2D> enemyCoords;

    MapInfo();
    MapInfo(size_t width, size_t height, size_t radiusCheck);

    [[nodiscard]] std::vector<Point2D> getInRangeRobots() const { return inRangeRobots; }
    [[nodiscard]] std::vector<Point2D> getInRangeBonus() const { return inRangeRobots; }

    //void setLastMove(const Point2D &move);
    void updateInformations(const std::vector<std::vector<std::string>>& board);
    void reset();
    void registerInRangeObjects(const std::vector<std::vector<std::string>>& board);
    void updateBonusOnMap(const Point2D &enemyPosition);

    void addBonus(const Point2D &coord);
};

#endif//LASTROBOTSTANDING_MAPINFO_H
