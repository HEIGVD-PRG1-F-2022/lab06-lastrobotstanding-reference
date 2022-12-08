//
// Created by david on 29.11.2022.
//

#include "MonRobot.h"

using namespace std;

MonRobot::MonRobot() {
}

void MonRobot::setConfig(size_t width_init, size_t height_init, unsigned energy_init,
               unsigned power_init) {
dx = width_init;
dy = height_init;
life = (int)energy_init;
pow = (int)power_init;
};

string MonRobot::action(vector<string> updates) {
    return "Action";

}

string MonRobot::name() const{
    return str_name;
}

string MonRobot::attack(int xMove, int yMove) {

    string stringAttack = "attack ";
    stringAttack += to_string(xMove);
    stringAttack += ",";
    stringAttack += to_string(yMove);

}

string MonRobot::move(int xMove, int yMove) {
    string stringMove = "move ";
    stringMove += to_string(xMove);
    stringMove += ",";
    stringMove += to_string(yMove);

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




string MonRobot::enemyNear() {


}


