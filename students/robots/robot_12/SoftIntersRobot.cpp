#include "SoftIntersRobot.h"

using namespace std;

string SoftIntersRobot::action(vector<string> updates) {
    const string BOARD = "board ";
    vector<vector<char>> infoBoard(5, vector<char>(5, ' '));
    string updateBoard;
    Direction dirMove(0, 0);
    for (const auto &message: updates) {
        updateBoard = message;
        size_t pos = updateBoard.find(BOARD);
        if (pos != string::npos) {
            updateBoard.erase(pos, BOARD.length());
        }
        for (size_t itString = 0; itString < updateBoard.length(); itString++) {
            infoBoard.at(itString / 5).at(itString % 5) = updateBoard.at(itString);
        }
    }
    if (hp < 10){
        for (const auto &row : infoBoard){
            for (const auto &cell : row){
                if (cell == 'R'){
                    dirMove = Direction(0,1);
                } else if (cell == 'B'){
                    dirMove = Direction(1, 0);
                }
            }
        }
    } else {
        dirMove = Direction(1, 1);
    }


    return Message::actionMove(dirMove);
}

string SoftIntersRobot::name() const {
    return NAME_ROBOT;
}

void SoftIntersRobot::setConfig(size_t width, size_t height, unsigned energy, unsigned power) {
    largeurBoard = width;
    hauteurBoard = height;
    hp = energy;
    dmgPower = power;
}
