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

using namespace std;

MonRobot::MonRobot() {
    dx = 0;
    dy = 0;
    life = 0;
    pow = 0;
    str_name = "Dave et Gui";
}

void MonRobot::setConfig(size_t width_init, size_t height_init, unsigned energy_init,
                         unsigned power_init) {
    dx = width_init;
    dy = height_init;
    life = (int) energy_init;
    pow = (int) power_init;
};

string MonRobot::action(vector<string> updates) {
    //Transforme la vision du robot en vector<vector<char>>
    vector<vector<char>> board;
    for (auto &s: updates) {
        board = boardStringToVectors(s);
        if(!board.empty()){
            break;
        }
    }
    vector<vector<int>> bonusCoordinates = objectNear(board, 'B');
    vector<vector<int>> enemyCoordinates = objectNear(board, 'R');
    if (!bonusCoordinates.empty()) {
        return move(bonusCoordinates.front().front(), bonusCoordinates.front().back());    //goToBonus(bonusCoordinates);
    } else {
        if (!enemyCoordinates.empty()) {
            if (life < 5) {
                return fleeRobot(enemyCoordinates);
            } else {
                return fightFirstTarget(enemyCoordinates);
            }
        } else {
            return goBackRight();
        }
    }

    return "wait";//fightFirstTarget(board);
}

string MonRobot::name() const {
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

string MonRobot::fightFirstTarget(const vector<vector<int>> &enemyCoordinates) {
    return attack(enemyCoordinates.front().front(), enemyCoordinates.front().back());
}


vector<vector<int>> MonRobot::objectNear(const vector<vector<char>> &board, char objectSymbol) {
    vector<vector<int>> objectCoordinates;
    if (objectSymbol == 'R' || objectSymbol == 'B') {
        //S'il y a un ennemi ou un bonus aux alentours du robot
        for (size_t y = 0; y < 5; y++) {
            for (size_t x = 0; x < 5; x++) {
                if (board.at(y).at(x) == objectSymbol) {
                    objectCoordinates.emplace_back(vector<int>{(int) (x - 2), (int) (y - 2)});
                }
            }
        }
    }
    return objectCoordinates;
}


vector<vector<char>> MonRobot::boardStringToVectors(const string &str) {
    if (Message(str).msg == MessageType::UpdateBoard) {
        string strBoard = split(str, " ", 2).back();
        vector<vector<char>> board;
        for (unsigned i = 0; i < 25; i++) {
            if (i % 5 == 0) {
                board.emplace_back();
            }
            char c = strBoard[i];
            board.back().push_back(c);
        }
        return board;
    }

    return {};
}

string MonRobot::fleeRobot(const std::vector<std::vector<int>> &enemyCoordinates) {

    vector<int> coordNearestRobot(2);
    int coordResult = 5; //need a number higher than 2 + 2

    for (auto coord : enemyCoordinates) {
        int x = coord.front(), y = coord.back();
        if (x != 0 || y != 0) {
            if (abs(x) + abs(y) < coordResult) {
                coordResult = abs(x) + abs(y);
                coordNearestRobot.front() = x;
                coordNearestRobot.back() = y;       //relatives coordinates
            }
        }
    }
    return move(coordNearestRobot.front(), coordNearestRobot.back());
}

