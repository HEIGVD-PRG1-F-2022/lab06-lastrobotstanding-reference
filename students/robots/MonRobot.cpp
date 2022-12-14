/*
-----------------------------------------------------------------------------------
Nom du fichier  : MonRobot.cpp
Auteur(s)       : Guillaume Dunant, David Berger
Date creation   : 29.11.2022

Description     : LastRobotStanding

Remarque(s)     : <a completer>

-----------------------------------------------------------------------------------
*/

#include "MonRobot.h"
#include "librobots/Message.h"
#include <iostream>

using namespace std;

MonRobot::MonRobot() {
    str_name = "RoboTest";
}

void MonRobot::setConfig(size_t width_init, size_t height_init, unsigned energy_init,
               unsigned power_init) {
dx = width_init;
dy = height_init;
life = (int)energy_init;
pow = (int)power_init;
};

string MonRobot::action(vector<string> updates) {
    //return fightFirstTarget(updates);
    return attack(1,0);
}

string MonRobot::name() const{
    return str_name;
}

string MonRobot::attack(int dx, int dy) {

    string stringAttack = "attack ";
    stringAttack += to_string(dx);
    stringAttack += ",";
    stringAttack += to_string(dy);
    return stringAttack;
}

string MonRobot::move(int xMove, int yMove) {
    string stringMove = "move ";
    stringMove += to_string(xMove);
    stringMove += ",";
    stringMove += to_string(yMove);
    return stringMove;
}

string MonRobot::goForward() {
    return move(0, 1);
}
string MonRobot::goBackward() {
    return move(0, -1);
}
string MonRobot::goLeft() {
    return move(-1, 0);
}
string MonRobot::goRight() {
    return move(1, 0);
}
string MonRobot::goUpLeft() {
    return move(-1, 1);
}
string MonRobot::goUpRight() {
    return move(1, 1);
}
string MonRobot::goBackLeft() {
    return move(-1, -1);
}
string MonRobot::goBackRight() {
    return move(1, -1);
}

string MonRobot::fightFirstTarget(const vector<string>& updates) {
    vector<Message> msg;
    for(auto s : updates){
        auto board = boardStringToVectors(s);
    }

    return "wait";
}


string MonRobot::enemyNear() {


}

vector<vector<char>> MonRobot::boardStringToVectors(const string& str) {
    if(Message(str).msg == MessageType::UpdateBoard){
        string strBoard = split(str,"d").back();
        vector<vector<char>> board;
        for(unsigned i = 0; i < 25; i++){
            if(i%5 == 0){
                board.emplace_back();
            }
            char c = strBoard[i];
            board.back().push_back(c);
        }
        return board;
    }

    return {};
}


