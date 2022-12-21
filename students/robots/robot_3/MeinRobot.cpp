// ================================================================================
// File : Robot.cpp
// Project name : LABO - LastRobotStanding
// Project members :
// - Florian Duruz, Antoine Leresche
// File created by Florian Duruz on 29/11/2022.
// ================================================================================

#include "MeinRobot.h"
//#include "libdio/display.h"

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
        int x = coord.getX();
        int y = coord.getY();
        board.at(y).at(x) = info.at(i);
    }
    board.at(SEARCH_WIDTH / 2).at(SEARCH_WIDTH / 2) = "X";

    mapInfo.currentBoard = board;
    /*
    Display::init();
    Display::DString display(Display::Color::PINK);
    Display::systemClearScreen();
    std::cout << Display::displayGrid(mapInfo.currentBoard, true);
*/
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
    int numEnemyAliveThisFrame = 0;
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

    //UPDATE STATE
    switch (state) {
        case STATE_MACHINE::SEARCH:
            numEnemyAlive = numEnemyAliveThisFrame;
            if (mapInfo.bonus.empty()) {
                state = numEnemyAlive == 1 ? STATE_MACHINE::KILL : STATE_MACHINE::SEARCH;
                target = enemyTarget;
            } else {
                state = STATE_MACHINE::RETRIEVE;
            }
            break;
        case STATE_MACHINE::RETRIEVE:
            if (target == Point2D(0, 0)) {
                state = mapInfo.bonus.size() == 0 ? STATE_MACHINE::SEARCH : STATE_MACHINE::RETRIEVE;
            }
            break;
        case STATE_MACHINE::KILL:
            if (target == Point2D(0, 0) || mapInfo.bonus.size() == 0) {
                state = STATE_MACHINE::SEARCH;
            }
            break;
    }
}

std::string MeinRobot::action(std::vector<std::string> updates) {
    gameEngineUpdate(updates);
    mapInfo.updateInformations();
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
    bonusState = BonusState();

    const std::string baseVal = " ";
    board.resize(SEARCH_WIDTH);
    for (auto &y: board) {
        y.resize(SEARCH_WIDTH);
        std::fill(y.begin(), y.end(), baseVal);
    }
}

void MeinRobot::resetValues() {
    aggressors.clear();
    std::for_each(board.begin(), board.end(), [](std::vector<std::string> &value) { std::fill(value.begin(), value.end(), " "); });
}

std::string MeinRobot::move(const Point2D &direction) {
    size_t x = static_cast<size_t>(direction.getX()) + 2;
    size_t y = static_cast<size_t>(direction.getY()) + 2;
    std::string targetMessage = std::to_string(x).append(",").append(std::to_string(y));

    bool isEnemyInTheWay = board.at(y).at(x) == "R";
    mapInfo.setLastMove(isEnemyInTheWay ? Point2D() : direction);

    return (isEnemyInTheWay ? "attack " : "move ") + targetMessage;
}

//===================================================================================================
// SECTION : STRATEGY
//===================================================================================================
std::string MeinRobot::strategy() {
    Point2D direction;
    std::string newAction = " ";
    switch (state) {
        case STATE_MACHINE::SEARCH:
            newAction = "radar";
            break;
        case STATE_MACHINE::RETRIEVE:
            target = bonusState.updateState(mapInfo);
            newAction = target == Point2D(0, 0) ? move(Point2D(1, 1)) : move(target);
            break;
        case STATE_MACHINE::KILL:
            newAction = move(target);
            target += mapInfo.lastMove;
            break;
    }
    return newAction;
}
