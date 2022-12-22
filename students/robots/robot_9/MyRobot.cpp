//
// Created by emily on 08.12.2022.
//
#include "MyRobot.h"
#include <iostream>

using namespace std;

void MyRobot::setConfig(size_t width_init, size_t height_init, unsigned energy_init,
                        unsigned power_init)
{
    width = width_init;
    height = height_init;
    energy = energy_init;
    power = power_init;
    lastPower = power;
    countRound = 0;
}

[[nodiscard]] string MyRobot::name() const
{
    return "Mentally Challenged Robot: ";
}

string MyRobot::action(std::vector<std::string> updates)
{
    countRound++;
    std::vector<Direction> robots, boni;
    Direction directionDamage;
    for (const auto &update: updates)
    {
        Message m(update);
        switch (m.msg)
        {
            case MessageType::UpdateBoard:
                setMemoryList(m.robots, robotsList);
                setMemoryList(m.boni, boniList);
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
                setMemoryList(m.boni, boniList);
                //if (!isDuplicate(m.boni.at(0), boni))
                //    boni.insert(boni.begin(), m.boni.begin(), m.boni.end());
                break;
            case MessageType::UpdateRobot:
                setMemoryList(m.robots, robotsList);
                //if (!isDuplicate(m.robots.at(0), robots))
                //    robots.insert(robots.begin(), m.robots.begin(), m.robots.end());
                break;
        }
    }
    Direction ClosestRobot = getClosest(robotsList);
    Direction ClosestRobotInverse = ClosestRobot.neg();
    Direction ClosestBonus = getClosest(boniList);
    double dClosestRobot = ClosestRobot.mag();
    double dClosestBonus = ClosestBonus.mag();

    int radarThreshold = 0;
    switch (energy)
    {
        case 1 ... 3:
            radarThreshold = 32;
            break;
        case 4 ... 7:
            radarThreshold = 16;
            break;
        default:
            radarThreshold = 8;
            break;
    }
    if (countRound >= radarThreshold and robots.empty())
    {
        countRound = 0;
        robotsList.clear();
        return Message::actionRadar();
    }

    if (power != lastPower)
    {
        boniList.clear();
        lastPower = power;
    }
    if (robotsList.empty())
    {
        return Message::actionRadar();
    }
    else
    {
        if(dClosestRobot * 2.0 < dClosestBonus)
        {
            if(ClosestRobot.mag() <= 2.82843)
            {
                return Message::actionAttack(ClosestRobot.neg());
            }
            return Message::actionMove(ClosestRobot.unitary().neg());
        }
        else
        {
            return Message::actionMove(ClosestBonus.unitary());
        }
    }


/*
    // hold up
    {
        if (target.mag() < 2.0 and target == ClosestRobot)
        {
            return Message::actionAttack(target);
        } else
        {
            if (energy > 7)
            {
                //energy high
                if (power > 3)
                {
                    //power high
                    if (dClosestRobot != 0 and dClosestRobot < 3)
                    {
                        return Message::actionAttack(ClosestRobot);
                    }
                    target = ClosestRobot;
                } else if (power >= 2)
                {
                    //power mid
                    if (dClosestRobot <= dClosestBonus)
                    {
                        target = ClosestRobot;
                    } else
                    {
                        target = ClosestBonus;
                    }
                } else
                {
                    // power low
                    if (dClosestRobot >= 2 * dClosestBonus)
                    {
                        target = ClosestBonus;
                    } else
                    {
                        target = ClosestRobot;
                    }
                }
            } else if (energy < 7 and energy >= 4)
            {
                //energy mid

                if (power > 3)
                {
                    //power high
                    if (dClosestRobot <= dClosestBonus)
                    {
                        target = ClosestRobot;
                    } else
                    {
                        target = ClosestBonus;
                    }
                } else if (power >= 2)
                {
                    //power mid
                    target = ClosestBonus;
                } else
                {
                    // power low
                    target = ClosestBonus;
                }
            } else
            {
                // energy low

                if (power > 3)
                {
                    //power high
                    if (dClosestRobot > 2 * dClosestBonus)
                    {
                        target = ClosestBonus;
                    } else
                    {
                        target = ClosestRobotInverse;
                    }
                } else
                {
                    // power low
                    if (dClosestRobot > 3 * dClosestBonus)
                    {
                        target = ClosestBonus;
                    } else
                    {
                        target = ClosestRobotInverse;
                    }
                }
            }
        }
        return moveTowards(target);
    }
    // hold down
    */
}

void MyRobot::setMemoryList(vector<Direction> listShortTerm, vector<Direction> &listLongTerm)
{
    for (auto i: listShortTerm)
    {
        if (isDuplicate(i, listLongTerm))
        {
            // robot list of the current turn contains an element i that is already present in the robot's long term memory
        } else
        {
            listLongTerm.push_back(i);
        }
    }
    for(auto i: listLongTerm)
    {
        if(isMissing(i,listShortTerm))
        {
            if(i.mag() <= 2.82843)
            {
                std::remove(listLongTerm.begin(), listLongTerm.end(),i);
            }
        }
    }
}

string MyRobot::moveTowards(Direction target)
{
    return Message::actionMove(target.unitary());
}

Direction MyRobot::getRandomDirection()
{
    int randX = 0;
    int randY = 0;
    do
    {
        randX = -1 + (std::rand() % (1 - (-1) + 1));
        randY = -1 + (std::rand() % (1 - (-1) + 1));
    } while (randX == 0 and randY == 0);
    Direction d(randX * 10, randY * 10);
    return d;
}

Direction MyRobot::getClosest(vector<Direction> directions)
{
    if (directions.empty())
    {
        return getRandomDirection();
    } else
    {
        vector<double> magnitudes = {};
        Direction min = directions.at(0);
        for (auto i: directions)
        {
            magnitudes.push_back(i.mag());
        }
        double smallestMag = *min_element(magnitudes.begin(), magnitudes.end());
        return directions.at(getIndex(magnitudes, smallestMag));
    }
}

int MyRobot::getIndex(vector<double> v, double K)
{
    auto it = find(v.begin(), v.end(), K);

    int index = it - v.begin();
    return index;
}

//si duplicate, remove, else, keep
bool MyRobot::isDuplicate(Direction direction, vector<Direction> directions)
{
    for (auto i: directions)
    {
        if (direction == i)
        {
            return true;
        }
    }
    return false;
}

bool MyRobot::isMissing(Direction direction, vector<Direction> directions)
{
    if (std::find(directions.begin(), directions.end(), direction) != directions.end())
    {
        return false;
    } else
    {
        return true;
    }
}