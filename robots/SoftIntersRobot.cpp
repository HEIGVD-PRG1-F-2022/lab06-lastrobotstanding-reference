//
// Created by bioan on 29.11.2022.
//

#include "SoftIntersRobot.h"

using namespace std;

string SoftIntersRobot::action(vector<string> updates) {

    return updates.at(0);
}

string SoftIntersRobot::name() const {
    return "A";
}

void SoftIntersRobot::setConfig(size_t width, size_t height, unsigned energy, unsigned power) {


}