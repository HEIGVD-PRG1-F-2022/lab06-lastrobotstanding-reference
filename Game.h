//
// Created by Linus Gasser on 14.12.22.
//

#ifndef REFERENCEGAME_GAME_H
#define REFERENCEGAME_GAME_H

#include <algorithm>
#include <libdio/display.h>
#include <librobots/Bonus.h>
#include <librobots/Message.h>
#include <librobots/RobotState.h>
#include <vector>

using namespace std;

class Game {
    const unsigned ENERGY_INIT = 10;
    const unsigned POWER_INIT = 1;
    const unsigned BONUS_RECURRENCE = 100;
    const unsigned IDLE_LIMIT = 200;
    const unsigned MAX_ROUNDS = 10000;

    vector<Robot *> waitList;
    vector<RobotState> robots;
    vector<Position> radar;
    vector<Position> positions;
    vector<Bonus> boni;
    unsigned bonusTimeout = 1;
    size_t side = 0;
    unsigned round = 0, idle = 0;

    long robotsAlive();
    size_t largestRobotAlive();

    void waitListToArena();

    void actionAttack();

    void actionMove();

    void actionRadar();

    void createBonus();

    void checkBonus();

    void sendUpdates();

    void displayRobotsStats(size_t x, size_t y);

    void display();

public:
    Game() = default;

    void addRobot(Robot *r);

    RobotState *play(bool show = true);
};


#endif//REFERENCEGAME_GAME_H
