#include "Game.cpp"
#include "RobotRandom.cpp"
#include "RobotSmart.cpp"
#include <iostream>
#include <vector>

using namespace std;

void game() {
    Game G;
    for (int i = 0; i < 4; i++) {
        G.addRobot(new RobotRandom());
        G.addRobot(new RobotSmart());
    }
    auto winner = G.play();
    if (winner == nullptr){
        cout << "The game ended in a draw" << endl;
    } else {
        cout << "Winner is: " << winner->name() << endl;
    }
}

void testGame(){
    Game G;
    for (int i = 0; i < 1; i++) {
        G.addRobot(new RobotRandom());
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
//    game();
//    testPosition();
//    testGame();
//    testRobotState();
    testMessage();
    return 0;
}
