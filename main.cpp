#include "Game.h"
#include "robot/RobotSmart.h"
#include "robot/RobotsSimple.h"
#include "students/robots/all.cpp"
#include <iostream>

using namespace std;

void game() {
    Game G;
    G.addRobot(new RobotDiagonal());
    G.addRobot(new RobotDiagonal());
    G.addRobot(new RobotDiagonal());
    G.addRobot(new RobotSmart());
    G.addRobot(new RobotSmart());
    G.addRobot(new RobotWait());
    for (const auto &robot: students()) {
        G.addRobot(robot);
    }
    auto winner = G.play();
    cout << Display::DString(Display::Color::YELLOW);
    if (winner == nullptr) {
        cout << "The game ended in a draw" << endl;
    } else {
        cout << "Winner is: " << winner->getName() << endl;
    }
    Display::DString().resetColor().print();
}

int main() {
    game();
    return 0;
}
