//
// Created by bioan on 29.11.2022.
//

#include "SoftIntersRobot.h"

using namespace std;

string SoftIntersRobot::action(vector<string> updates) {

    return Message::actionMove(Direction(1, 1));
}

string SoftIntersRobot::name() const {
    return "SoftIntersRobot";
}

void SoftIntersRobot::setConfig(size_t width, size_t height, unsigned energy, unsigned power) {


}

void SoftIntersRobot::robotChanges(const vector<std::string> &updates) {

}
