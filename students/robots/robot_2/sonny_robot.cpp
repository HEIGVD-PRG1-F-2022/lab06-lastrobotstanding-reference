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
        if (update.empty()) {
            continue;
        }

        vector<string> actionParameters = split(update, " ", 2);

        string action = actionParameters.at(0);
        string parameters;
        if (actionParameters.size() == 2) {
            parameters = actionParameters.at(1);
        }

        switch (Action::resolveAction(action)) {
            case Action::Name::BOARD:// << "BOARD" << endl;
                break;
            case Action::Name::DAMAGE:
                //// << "DAMAGE" << parameters << endl;
                break;
            case Action::Name::MOVE:// << "MOVE" << endl;
                break;
            case Action::Name::ATTACK:// << "ATTACK" << endl;
                break;
            case Action::Name::WAIT:// << "WAIT" << endl;
                break;
            case Action::Name::BONUS:// << "BONUS" << endl;
                break;
            default:
                break;
        }
        updates.erase(find(updates.begin(), updates.end(), update));
    }


    return "attack 0,-2";
}

    /*
     * 	Cherche Bonus : si robot plus proche de bonus regarde si il peut attack sinon fuite
     * 	attack robot seulement si energy suffisante
     * 	move diagonale
     */

/*  string action = " ";

    if(BONUS){
        if (!robot){// << "go to Bonus";
        } else{
            if (posBonus - posSonny < posBonus - posRobot){// << "go to Bonus";
            } else if(energy > 5 + nbrRound){// << "ATTACK" << endl;
            }else{// << "MOVE" << endl;
            }
        }

   if (robot){
       if(energy > 5 + nbrRound){// << "ATTACK" << endl;
       }else{// << "MOVE" << endl;
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
}

string SonnyRobot::name() const {
    return this->ROBOT_NAME;
}
