//
// Created by emily on 08.12.2022.
//
#include "MyRobot.h"
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

void MyRobot::setConfig(size_t width_init, size_t height_init, unsigned energy_init,
                        unsigned power_init) {
    width = width_init;
    height = height_init;
    energy = energy_init;
    power = power_init;
    countRound = 0;
}

[[nodiscard]] string MyRobot::name() const {
    return "Mentally Challenged Robot";
}

string MyRobot::action(std::vector<std::string> updates) {
    countRound++;
    std::vector<Direction> robots, boni;
    Direction directionDamage;
    for (const auto &update: updates) {
        Message m(update);
        switch (m.msg) {
            case MessageType::UpdateBoard:
                robots.insert(robots.begin(), m.robots.begin(), m.robots.end());
                boni.insert(boni.begin(), m.boni.begin(), m.boni.end());
                break;
            case MessageType::UpdateDamage:
                energy -= m.energy;
                break;
            case MessageType::UpdateEnergy:
                energy += m.energy;
                break;
            case MessageType::UpdatePower:
                power += m.power;
                break;
            case MessageType::UpdateBonus:
                if (!isDuplicate(m.boni.at(0), boni))
                    boni.insert(boni.begin(), m.boni.begin(), m.boni.end());
                break;
                // answer from radar
            case MessageType::UpdateRobot:
                if (!isDuplicate(m.robots.at(0), robots))
                    robots.insert(robots.begin(), m.robots.begin(), m.robots.end());
                break;
        }
    }

    /*
    //start of Ander's maybe not so smart idea
    double bonusScore = 0.0;
    Direction closestBonus;
    if(!boni.empty())
    {
        bonusScore = getClosest(boni).mag();
        closestBonus = getClosest(boni);
    }
    double robotScore = 0.0;
    Direction closestRobot;
    if(!robots.empty())
    {
        robotScore = getClosest(robots).mag();
        closestRobot = getClosest(robots);
    }
    double energyScore = double(energy) ;
    double powerScore = double(power);
    // let's sum all this up to evaluate the general behaviour of the robot
        //summing all this up may not be the correct idea
    double sumScore;
    // it would be a good idea to figure out what is the max amount realistically possible for @sumScore
    // bonusScore depends on the distance of the bonus and should be at most equal to Direction(gridWidth/2,gridHeight/2).mag
    // same for robotScore
    // energyScore  will span from 1 to something like 10 or 20, so let's approximate to 15
    // same behaviour for powerscore, from 1 to something like 6, let's approximate to 3
    // this leaves us with max values to clamp or calculations
    double weightBonusScore = 20.0;
    double weightRobotScore = 20.0;
    double weightEnergyScore = 15.00 ;
    double weightPowerScore = 3.00;
    bonusScore  = clamp(bonusScore,0.0,20.0)/20.0;
    robotScore  = clamp(robotScore, 0.0, 20.0)/20.0;
    energyScore = clamp(energyScore, 0.0, 15.0)/15.0;
    powerScore  = clamp(powerScore,0.0, 6.0)/6.0;
    // bonusScore combined with energyScore should push the robot forward (pursue bonus
    // robotScore combined with powerscore should push the robot forward (pursue or attack
    // energyScore should refrain the robot from pursuing or attacking
    // powerScore should push the robot to pursue or attack
    sumScore = (robotScore + powerScore) - (bonusScore + energyScore);

    *//*
    switch (int(sumScore))
    {

    }
    *//*

    switch (energy)
    {
        case 0 ... 3:
            //priority 1 : don't die -> flee from any robots
            //priority 2 : regain energy -> go to bonus
            if(bonusScore > robotScore)
            {
                moveTo(closestBonus);
            }
            else
            {
                moveTo(closestRobot.neg());
            }
            break;
        case 4 ... 6:
            //priority 1 : regain energy -> go to bonus
            //priority 2 : gain power -> go to bonus
            //priority 3 : don't die -> flee from any robots
            //priority 4 : pursue enemies
            break;
        case 7 ... 10:
            //priority 1 : pursue enemies
            //priority 2 : gain power -> go to bonus
            //priority 3 : regain energy -> go to bonus
            break;
        default:
            break;
    }
    // end of Ander's really dumb idea
*/
    // algorithm
    //is enemy robot fleeing ?

    //pursue it if so
    double dClosestRobot = getClosest(robots).mag();
    double dClosestBonus = getClosest(boni).mag();
    countRound++;
    if (dClosestRobot <= 1 and dClosestRobot != 0) { //If a robot is in the attack range, attack
        return Message::actionAttack(getClosest(robots));
    }
    if (countRound >= 8 and
        energy > 4) { //After an amount of time, do a radar unless the robot have a low energy, just runaway
        countRound = 0;
        return Message::actionRadar();
    }
    if (target.mag() == 0.0) {
        return wander();
    }
    if (energy < 4) //Energy Low
    {
        if (3 * dClosestBonus < dClosestRobot) { //Try to get a bonus if the robot is not too close
            Direction target(getClosest(boni));
            return moveTo(target);
        } else { //Runaway
            Direction escapeDirection(getClosest(robots));
            escapeDirection = escapeDirection.neg();
            return moveTo(escapeDirection);
        }
    } else if (energy < 7 and energy >= 4) {//Medium Energy TO CHECK
        if (dClosestRobot < dClosestBonus and power > 3) {
            return moveTo(getClosest(robots).unitary());
        }
        if (dClosestBonus < dClosestRobot) {
            return moveTo(getClosest(boni).unitary());
        }
        return wander();
    } else if (energy > 7) { //High Energy
        if ((2 * dClosestBonus < dClosestRobot and power < 4)) {
            return moveTo(getClosest(boni).unitary());
        } else if (dClosestRobot < dClosestBonus) {
            if (countRound < 8) {
                return moveTo(getClosest(robots).unitary());
            }
            return Message::actionRadar();
        } else { //Cas où le robot et le bonus sont équidistants
            return wander();
        }


        if (power > 4) {
            if (dClosestBonus < dClosestRobot) //If robot closer than bonus : chase robot
            {
                if (countRound == 8) //Every 8 rounds, do a radar
                {
                    target = getClosest(robots).unitary();
                }
                return moveTo(target);
            } else if (dClosestBonus > dClosestRobot) //If bonus closer than robot : go to bonus
            {
                target = getClosest(boni).unitary();
            } else // bonus as close as robot
            {

            }
            //Go to Robot instead of bonus
        } else if (power > 4 and dClosestRobot > 2) {
            if (countRound == 6) {
                countRound = 0;
                return Message::actionRadar();
            } else {
                return wander();
            }
        } else {
            //Go to bonus over robots
            if (countRound == 5) {
                countRound = 0;
                Direction d(0, 0);
                if (target == d) {
                    return wander();
                }
                return moveTo(target);
                /*target = getClosest(boni).unitary();
                    return moveTo(target);*/
            }
        }
        // return move to target.unitary ???
    } else {
        return wander();
    }
}

//finalize go to according to currrent target
/*
     * Damage Multiplier Map
        1 1 1 1 1
        1 2 2 2 1
        1 2 R 2 1
        1 2 2 2 1
        1 1 1 1 1
     */

string MyRobot::wander() {
    // wander off randomly
    int randX = -1 + (std::rand() % (1 - (-1) + 1));
    int randY = -1 + (std::rand() % (1 - (-1) + 1));
    Direction d(randX, randY);
    return Message::actionMove(d);
}

string MyRobot::moveTo(Direction target) {
    return Message::actionMove(target);
}

Direction MyRobot::getClosest(vector<Direction> directions) {
    if (directions.empty()) {
        return Direction(0, 0);
    } else {
        Direction min = directions.at(0);
        for (auto i: directions) {
            if (i.mag() < min.mag()) {
                min = i;
            }
        }
        return min;
    }
}

//si duplicate, remove, else, keep
bool MyRobot::isDuplicate(Direction direction, vector<Direction> directions) {
    for (auto i: directions) {
        if (direction == i) {
            return true;
        }
    }
    return false;
}