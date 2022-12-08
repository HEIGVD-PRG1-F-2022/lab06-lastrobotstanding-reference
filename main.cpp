#include "Game.cpp"
#include "RobotDiagonal.cpp"
#include "RobotSmart.cpp"
#include "RobotWait.cpp"
#include <iostream>
#include "robots/all.cpp"

using namespace std;

void game() {
    Game G;
    if (true){
        G.addRobot(new RobotDiagonal());
        G.addRobot(new RobotSmart());
        G.addRobot(new RobotWait());
        for (const auto &robot: students()){
            G.addRobot(robot);
        }
    } else {
        for (int i = 0; i < 3; i++) {
            G.addRobot(new RobotDiagonal());
            G.addRobot(new RobotSmart());
            G.addRobot(new RobotWait());
        }
    }
    auto winner = G.play();
    cout << Display::DString(Display::Color::YELLOW);
    if (winner == nullptr){
        cout << "The game ended in a draw" << endl;
    } else {
        cout << "Winner is: " << winner->getName() << endl;
    }
    Display::DString().resetColor().print();
}

void testGame(){
    Game G;
    for (int i = 0; i < 1; i++) {
        G.addRobot(new RobotDiagonal());
        G.addRobot(new RobotSmart());
    }
    G.waitListToArena();
    for (int i = 0; i < 10; i++){
        G.actionAttack();
        G.actionMove();
        G.actionRadar();
        G.display_debug();
        G.createBonus();
        G.checkBonus();
        G.sendUpdates();
    }
}

int main() {
    game();
//    testPosition();
//    testGame();
//    testRobotState();
    testMessage();
    return 0;
}
