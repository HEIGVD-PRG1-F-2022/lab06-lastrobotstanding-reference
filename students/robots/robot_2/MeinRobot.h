// ================================================================================
// File : Robot.h
// Project name : LABO - LastRobotStanding
// Project members :
// - Florian Duruz, Antoine Leresche
// File created by Florian Duruz on 29/11/2022.
// ================================================================================


#ifndef LASTROBOTSTANDING_MEINROBOT_H
#define LASTROBOTSTANDING_MEINROBOT_H

#include "librobots/Robot.h"
#include "librobots/Message.h"
#include "Point.h"
#include "MapInfo.h"
#include "BonusState.h"
#include <algorithm>
#include <vector>
#include <string>
//#include <map>

const size_t SEARCH_RADIUS = 2;
const size_t SEARCH_WIDTH = 2 * SEARCH_RADIUS + 1;

const std::vector<std::string> INSTRUCTIONS
{
    "board",
    "damage",
    "energy",
    "power",
    "bonus",
    "robot",
    //"move",
    //"attack",
    //"wait",
    //"radar"
};

enum class RecievedActionType : unsigned
{
    BOARD   = 0,
    DAMAGE  = 1,
    ENERGY  = 2,
    POWER   = 3,
    BONUS   = 4,
    ROBOT   = 5,
};

enum class STATE_MACHINE
{
    KILL,
    SEARCH,
    RETRIEVE,
};

class MeinRobot: public Robot
{
private:
    STATE_MACHINE state = STATE_MACHINE::SEARCH;

    size_t width = 0;
    size_t height = 0;
    unsigned energy = 0;
    unsigned power = 0;

    std::vector<Point> aggressors; //robots who attack us this frame
    Point target = Point(-1,-1);

    BonusState bonusState;
    MapInfo mapInfo;

    //board
    std::vector<Point> bonus;
    std::vector<std::vector<std::string>> board;

    std::string strategy();

    void setBoard(const std::string& cells);
    void setDamage(const std::string& info);
    void addEnergy(const std::string& info);
    void addPower(const std::string& info);
    void addBonus(const std::string& info);

    void registerAction(std::vector<std::string> updates);
    void resetValues();

    std::string move(const Point &direction);
public:
    MeinRobot()= default;
    void setConfig(size_t width, size_t height, unsigned energy,unsigned power) override;

    bool test() { return true;};

    // Une étape dans le jeu: les ‘updates’ sont des informations sur 
    // l’environnement du robot. Le robot retourne un ou plusieurs string
    // pour indiquer ses actions.
    std::string action(std::vector<std::string> updates) override;
    // Retourne le nom de ce robot.
    [[nodiscard]] std::string name() const override;
};


#endif //LASTROBOTSTANDING_MEINROBOT_H
