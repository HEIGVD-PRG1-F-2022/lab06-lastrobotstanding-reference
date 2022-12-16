// ================================================================================
// File : Robot.cpp
// Project name : LABO - LastRobotStanding
// Project members :
// - Florian Duruz, Antoine Leresche
// File created by Florian Duruz on 29/11/2022.
// ================================================================================

#include "MeinRobot.h"
//#include "libdio/display.h"

int indexOf(const std::vector<std::string>& instructions, const std::string& val)
{
    for (size_t i = 0; i < instructions.size(); ++i)
    {
        if(instructions.at(i) == val)
        {
            return i;
        }
    }
    return -1;
}

// ===================================================================
// Retrieve 2D Coord from 1D Coord
// ===================================================================
Point getXY(size_t index , size_t width)
{
    size_t y = index / width;
    size_t x = index - (y * width);
    return Point(x,y);
}
// ===================================================================
// Register BOARD
// ===================================================================
void MeinRobot::setBoard(const std::string& cells)
{
    for (size_t i = 0; i < cells.length(); ++i)
    {
        Point coord = getXY(i,SEARCH_WIDTH);
        auto x = static_cast<size_t>(coord.getX());
        auto y = static_cast<size_t>(coord.getY());
        board.at(y).at(x) = cells.at(i);
    }
    board.at(SEARCH_WIDTH/2).at(SEARCH_WIDTH/2) = "X";

    mapInfo.currentBoard = board;
/*
    Display::init();
    Display::DString display(Display::Color::PINK);
    Display::systemClearScreen();
// << Display::displayGrid(mapInfo.currentBoard, true);
*/
}

void MeinRobot::setDamage(const std::string& info)
{
    auto args = split(info, ",");

    Point bullyPoint(stoi(args.at(0)),stoi(args.at(1)));
    aggressors.push_back(bullyPoint);
// << "aggressor at " << aggressors.front() << std::endl;
    energy -= unsigned(stoi(args.at(2)));
}

void MeinRobot::addEnergy(const std::string &info)
{
    energy += unsigned(stoi(info));
}

void MeinRobot::addPower(const std::string &info)
{
    power += unsigned(stoi(info));
}

void MeinRobot::addBonus(const std::string &info)
{
    auto args = split(info, ",");
    Point position(long(stoi(args.at(1))), long(stoi(args.at(2))));
    bonus.emplace_back(unsigned(stoi(args.at(1))), unsigned(stoi(args.at(2)))) ;
}



void MeinRobot::registerAction(std::vector<std::string> updates)
{
    for(const auto& update : updates)
    {
        //Retrieve Action Type
        std::vector<std::string> action = split(update, " ", 2);
        //Get Action related
        auto actionType = (RecievedActionType)indexOf(INSTRUCTIONS,action.at(0));
        std::string actionInfo = action.at(1);
        switch (actionType) // MUST UPDATE STATE MACHINE
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
                break;
        }
    }
}

std::string MeinRobot::action(std::vector<std::string> updates)
{
    //mapInfo.
    registerAction(updates);
    mapInfo.updateInformations();
    std::string action = strategy();
    resetValues();
    return action;
}

std::string MeinRobot::name() const
{
    return "CatFromager";
}

void MeinRobot::setConfig(size_t width, size_t height, unsigned int energy, unsigned int power)
{
    this->width = width;
    this->height = height;
    this->energy = energy;
    this->power = power;

    state = STATE_MACHINE::SEARCH;
    mapInfo = MapInfo(width, height, SEARCH_RADIUS);
    bonusState = BonusState();

    const std::string baseVal = " ";
    board.resize(SEARCH_WIDTH);
    for (auto& y : board)
    {
        y.resize(SEARCH_WIDTH);
        std::fill(y.begin(), y.end(),baseVal);
    }
}

void MeinRobot::resetValues()
{
    aggressors.clear();
    std::for_each(board.begin(),board.end(), [](std::vector<std::string>& value) {
        std::fill(value.begin(), value.end(), " ");});
}

std::string MeinRobot::move(const Point &direction)
{
    std::string targetMessage = std::to_string(direction.getX()).append(",").append(std::to_string(direction.getY()));
    mapInfo.setLastMove(direction);
// << std::endl << "move "  << direction<<std::endl;
    return "move " + targetMessage;
}

//===================================================================================================
// SECTION : STRATEGY
//===================================================================================================
std::string MeinRobot::strategy()
{
    Point target;
    std::string newAction = " ";
    switch (state)
    {
        case STATE_MACHINE::SEARCH:
            target = bonusState.updateState(mapInfo);
            break;
        case STATE_MACHINE::KILL:
            break;
        case STATE_MACHINE::RETRIEVE:
            //Condition In : target != -1,-1
            //Condition out : Not Register In Radar Anymore
            break;
    }
    newAction = target == Point(0,0) ? move(Point(1,1)) : move(target);
    return newAction;
}







