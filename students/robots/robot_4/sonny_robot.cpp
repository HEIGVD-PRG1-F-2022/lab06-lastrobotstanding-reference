/*
-----------------------------------------------------------------------------------
Project Name    : lab06-lastrobotstanding-comme_tu_veux
File's Name     : sonny_robot.cpp
Author          :   Aellen Quentin
                    Atasever Mehmet
                    Salamin Chloé
Creation Date   : 29.11.2022
Description     :
Remarque(s)     :
Compiler        : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
*/

#include <iostream>
#include <librobots/Message.h>

#include "sonny_robot.h"
#include "action.h"

using namespace std;

string SonnyRobot::action(vector<string> updates) {
    for (const string &update: updates) {
        vector<string> actionParameters = split(update, " ", 2);

        string action = actionParameters.at(0);
        string parameters;
        if (actionParameters.size() == 2) {
            parameters = actionParameters.at(1);
        }

        switch (Action::resolveAction(action)) {
            case Action::NOTDEFINED:
                break;
            case Action::MOVE:
                break;
            case Action::ATTACK:
                break;
            case Action::WAIT:
                break;
            case Action::BONUS:
                break;
            case Action::Name::BOARD:
                internalMap = fromStringToMap(parameters);
                break;
            case Action::Name::DAMAGE:
                vector<string> damageInfo = split(parameters, ",", 3);
                attacker = Point(damageInfo.at(0), damageInfo.at(1));
                energy -= (unsigned) stoi(damageInfo.at(2));
                break;
        }
    }

    target = targetToLock();
    target -= getCenterMap();

    ++counter;

    return "move " + (string) target.normalize();
}

/*
 * 	Cherche Bonus : si robot plus proche de bonus regarde si il peut attack sinon fuite
 * 	attack robot seulement si energy suffisante
 * 	move diagonale
 */

/*  string action = " ";

    if(BONUS){
        if (!robot){
            cout << "go to Bonus";
        } else{
            if (posBonus - posSonny < posBonus - posRobot){
                cout << "go to Bonus";
            } else if(energy > 5 + nbrRound){
                cout << "ATTACK" << endl;
            }else{
                cout << "MOVE" << endl;
            }
        }

   if (robot){
       if(energy > 5 + nbrRound){
           cout << "ATTACK" << endl;
       }else{
           cout << "MOVE" << endl;
       }
   }

   else{
            action = "move -2,-2";
    }

    return action;
}*/


void SonnyRobot::setConfig(size_t width, size_t height, unsigned int energy, unsigned int power) {
    this->mapWidth = width;
    this->mapHeight = height;
    this->energy = energy;
    this->power = power;

    this->map= vector(mapHeight, vector<string>(mapWidth, ""));
}

string SonnyRobot::name() const {
    return this->ROBOT_NAME;
}

Point SonnyRobot::targetToLock() {

    Point sonny = getCenterMap();
    Point shortestPoint(0, 0);

    for (size_t y = 0; y < internalMap.size(); ++y) {
        for (size_t x = 0; x < internalMap.at(y).size(); ++x) {
            string object = internalMap.at(y).at(x);
            if (!object.empty()) {
                Point current(x, y);
                if (sonny.distance(current) < sonny.distance(shortestPoint)) {
                    shortestPoint = current;
                    switch(object.at(0)){
                        case 'R':
                            if (energy < 15){
                                Point d = current.normalize();
                                d.x *= -1;
                                d.y *= -1;
                                shortestPoint = d;
                            }
                            break;
                    }
                }
            }
        }
    }
    return shortestPoint;
}

vector<vector<string>> SonnyRobot::fromStringToMap(const std::string map) {
    vector<vector<string>> mapVector;

    for (size_t y = 0; y < FIELD_OF_VIEW; ++y) {
        vector<string> line;
        for (size_t x = 0; x < FIELD_OF_VIEW; ++x) {
            int offset = y * (FIELD_OF_VIEW);
            string car = map.substr(x + offset, 1);
            line.push_back(car == " " ? "" : car);
        }
        mapVector.push_back(line);
    }
    return mapVector;
}

Point SonnyRobot::getCenterMap() {
    int x = (mapWidth - 1) / 2;
    int y = (mapHeight - 1) / 2;
    return {x, y};
}

