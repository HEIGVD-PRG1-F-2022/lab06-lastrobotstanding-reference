// ================================================================================
// File : Robot.cpp
// Project name : LABO - LastRobotStanding
// Project members :
// - Florian Duruz, Antoine Leresche
// File created by Florian Duruz on 29/11/2022.
// ================================================================================

#include "MeinRobot.h"

int indexOf(const std::vector<std::string> &instructions, const std::string &val) {
    for (size_t i = 0; i < instructions.size(); ++i) {
        if (instructions.at(i) == val) {
            return i;
        }
    }
    return -1;
}

// ===================================================================
// Retrieve 2D Coord from 1D Coord
// ===================================================================
Point2D getXY(size_t index, size_t width) {
    size_t y = index / width;
    size_t x = index - (y * width);
    return Point2D(x, y);
}

// ===================================================================
// Register BOARD
// ===================================================================
void MeinRobot::setBoard(const std::string &info) {
    for (size_t i = 0; i < info.length(); ++i) {
        Point2D coord = getXY(i, SEARCH_WIDTH);
        auto x = static_cast<size_t>(coord.getX());
        auto y = static_cast<size_t>(coord.getY());
        board.at(y).at(x) = info.at(i);
    }
    board.at(SEARCH_RADIUS).at(SEARCH_RADIUS) = "X";

    mapInfo.currentBoard = board;
}

void MeinRobot::setDamage(const std::string &info) {
    auto args = split(info, ",");
    Point2D bullyPoint(stoi(args.at(0)), stoi(args.at(1)));
    aggressors.push_back(bullyPoint);
    energy -= unsigned(stoi(args.at(2)));
}

void MeinRobot::addEnergy(const std::string &info) {
    energy += unsigned(stoi(info));
}

void MeinRobot::addPower(const std::string &info) {
    power += unsigned(stoi(info));
}

void MeinRobot::addBonus(const std::string &info) {
    std::vector<std::string> args = split(info, ",");
    Point2D position(long(stoi(args.at(0))), long(stoi(args.at(1))));
    mapInfo.bonus.push_back(position);
}

void MeinRobot::addRobot(const std::string &info) {
    std::vector<std::string> args = split(info, ",");
    enemyTarget = Point2D(long(stoi(args.at(0))), long(stoi(args.at(1))));
    mapInfo.updateBonusOnMap(enemyTarget);
    numEnemyAlive += 1;
}

void MeinRobot::gameEngineUpdate(std::vector<std::string> updates) {
    for (const auto &update: updates) {

        //Retrieve Action Type
        std::vector<std::string> action = split(update, " ", 2);
        //Get Action related
        auto actionType = (RecievedActionType) indexOf(INSTRUCTIONS, action.at(0));
        std::string actionInfo = action.at(1);

        switch (actionType)
        {
            case RecievedActionType::BOARD:
                setBoard(actionInfo);
                break;
            case RecievedActionType::DAMAGE:
                setDamage(actionInfo);
                break;
            case RecievedActionType::ENERGY:
                addEnergy(actionInfo);
                break;
            case RecievedActionType::POWER:
                addPower(actionInfo);
                break;
            case RecievedActionType::BONUS:
                addBonus(actionInfo);
                break;
            case RecievedActionType::ROBOT:
                addRobot(actionInfo);
                break;
        }
    }

    target -= lastMove;
    mapInfo.updateInformations(board);
    bool targetExist = mapInfo.updateBonus(lastMove);
    target = targetExist ? target : Point2D(0,0);
    lastMove = Point2D(0,0);
    //UPDATE STATE
    updateStateMachine();
}


std::string MeinRobot::action(std::vector<std::string> updates) {
    gameEngineUpdate(updates);

    std::string action = strategy();
    resetValues();
    return action;
}

std::string MeinRobot::name() const {
    return "LE_FROMAGER";
}

void MeinRobot::setConfig(size_t width, size_t height, unsigned int energy, unsigned int power) {
    this->width = width;
    this->height = height;
    this->energy = energy;
    this->power = power;

    state = STATE_MACHINE::SEARCH;
    mapInfo = MapInfo(width, height, SEARCH_RADIUS);

    const std::string baseVal = " ";
    board.resize(SEARCH_WIDTH);
    for (auto &y: board) {
        y.resize(SEARCH_WIDTH);
        std::fill(y.begin(), y.end(), baseVal);
    }
}

void MeinRobot::resetValues() {
    numEnemyAlive = 0;
    aggressors.clear();
    for (auto& i : board) {
        std::fill(i.begin(), i.end()," ");
    }
}

//===================================================================================================
// SECTION : MOVE
//===================================================================================================

std::string MeinRobot::move(const Point2D &direction) {
    Point2D dirNormalize = direction.normalize();
    bool isEnemyInTheWay = board.at(dirNormalize.getY() + SEARCH_RADIUS).at(dirNormalize.getX() + SEARCH_RADIUS) == "R";

    Point2D escapeMove = Point2D(dirNormalize.getY() ,dirNormalize.getX());
    escapeMove = escapeMove.getX() == escapeMove.getY() ? Point2D(escapeMove.getY(),0) : escapeMove;
    lastMove = isEnemyInTheWay ? escapeMove : dirNormalize;
    return "move " + (isEnemyInTheWay ? escapeMove.toString() : dirNormalize.toString());

    //lastMove = dirNormalize;
    //return "move " + dirNormalize.toString();
}

//===================================================================================================
// SECTION : STATE MACHINE
//===================================================================================================
void MeinRobot::updateStateMachine() {
    switch (state) {
        case STATE_MACHINE::SEARCH:
            if (mapInfo.bonus.empty()) {
                state = numEnemyAlive == 1 ? STATE_MACHINE::KILL : STATE_MACHINE::SEARCH;
                target = enemyTarget;
            } else {
                state = STATE_MACHINE::RETRIEVE;
                calculateClosestBonus(mapInfo);
            }
            break;
        case STATE_MACHINE::RETRIEVE:
            calculateClosestBonus(mapInfo);
            if (target == Point2D(0, 0)) {
                state = mapInfo.bonus.empty() ? STATE_MACHINE::SEARCH : STATE_MACHINE::RETRIEVE;
                calculateClosestBonus(mapInfo);
            }
            break;
        case STATE_MACHINE::KILL:
            if (target == Point2D(0, 0) && mapInfo.getInRangeRobots().empty()) {
                if(mapInfo.getInRangeRobots().empty()){
                    state = STATE_MACHINE::SEARCH;
                }
                else{
                    target = mapInfo.getInRangeRobots().front();
                }
            }
            break;
    }
}

//===================================================================================================
// SECTION : STATE MACHINE : BONUS
//===================================================================================================

void MeinRobot::calculateClosestBonus(const MapInfo& info) {

    if(info.bonus.empty() && info.getInRangeBonus().empty())
    {
        target = Point2D(0,0);
    }
    else if (!info.bonus.empty()){ //CHECK spawned bonus if none are in field of view
        double shortestDistance = std::numeric_limits<double>::max();
        for (const auto &outVisionCoord: info.bonus) {
            double distance = outVisionCoord.mag();
            if(distance < shortestDistance ) {
                target = outVisionCoord;
                shortestDistance = distance;
            }
        }
    }
    else
    {
        double shortestDistance = std::numeric_limits<double>::max();
        for (const auto &bonusCoord: info.getInRangeBonus()) {
            double distance = bonusCoord.mag();
            if(distance < shortestDistance ) {
                target = bonusCoord;
                shortestDistance = distance;
            }
        }
    }
}

//===================================================================================================
// SECTION : STRATEGY
//===================================================================================================
std::string MeinRobot::strategy() {
    //Point2D direction;
    std::string newAction = " ";
    switch (state) {
        case STATE_MACHINE::SEARCH:
            if(!aggressors.empty()){
                Point2D aggressorPosition = aggressors.front();
                newAction = move(Point2D(-aggressorPosition.getX(),-aggressorPosition.getY()));
            }
            else{
                newAction = "radar";
                mapInfo.bonus.clear();
            }
            break;
        case STATE_MACHINE::RETRIEVE:
            newAction = move(target);
            break;
        case STATE_MACHINE::KILL:
            target = mapInfo.getInRangeRobots().empty() ? target : mapInfo.getInRangeRobots().front();
            newAction = move(target);
            break;
    }
    return newAction;
}
