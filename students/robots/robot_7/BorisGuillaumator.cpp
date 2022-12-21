#include <map>
#include <cstring>
#include <random>

#include "BorisGuillaumator.h"

using namespace std;

int BorisGuillaumator::coordToDirection(int x) {
    return x > 0 ? 1 : x == 0 ? 0 : -1;
}

vector<string> BorisGuillaumator::split(string str, const char *delim) {
    char *pch;
    vector<string> res;
    pch = strtok(str.data(), delim);

    while (pch != nullptr) {
        res.emplace_back(pch);
        pch = strtok(nullptr, delim);
    }
    return res;
}

void BorisGuillaumator::setConfig(size_t width_p, size_t height_p, unsigned energy_p, unsigned power_p) {
    width = width_p;
    height = height_p;
    energy = energy_p;
    power = power_p;
}

string BorisGuillaumator::action(vector<string> updates) {

    //convert to map for have key->value style
    map<string, string> updatesMap;
    vector<string> radarInfos;
    for (const string &el: updates) {
        string head = split(el, " ")[0];
        if (head.size() + 1 > updates.size()) {
            if (head == "robot") {
                radarInfos.push_back(el.substr(head.size() + 1));
            } else {
                updatesMap[head] = el.substr(head.size() + 1);
            }
        }
    }

    if (updatesMap.contains("energy")) {
        energy += stoi(updatesMap["energy"]);
    }

    if (updatesMap.contains("power")) {
        power += stoi(updatesMap["power"]);
    }

    if (updatesMap.contains("damage")) {
        vector<string> params = split(updatesMap["damage"], ",");
        energy -= stoi(params.at(2));
        return energy > 17 && abs(stoi(params.at(0)) % 2) == 1 && abs(stoi(params.at(1)) % 2) == 1
               ? "move " + to_string(coordToDirection(stoi(params.at(0)))) + "," +
                 to_string(coordToDirection(stoi(params.at(1))))
               : "attack " + params.at(0) + ',' + params.at(1);
    }

    // to pass the robot from neutral to agressive just uncomment the next block
    /*if (updatesMap["board"].find('R') != string::npos) {
        int x, y;
        x = (int) (updatesMap["board"].find('R')) % 5 - 2;
        y = updatesMap["board"].find('R') == 25 ? 2 : (int) updatesMap["board"].find('R') / 5 - 2;
        return "attack " + to_string(x) + "," + to_string(y);
    }*/

    if ((updatesMap.contains(
            "board") // normally always contains "board" but, to dodge access exception if not contains it, we test it anyway
         && updatesMap["board"].find('B') != string::npos)) {
        int x, y;
        x = (int) (updatesMap["board"].find('B')) % 5 - 2;
        y = updatesMap["board"].find('B') == 25 ? 2 : (int) updatesMap["board"].find('B') / 5 - 2;
        return "move " + to_string(coordToDirection(x)) + "," + to_string(coordToDirection(y));
    }

    if (updatesMap.contains("bonus")) {
        int x, y;
        x = stoi(split(updatesMap["bonus"], ",")[0]);
        y = stoi(split(updatesMap["bonus"], ",")[1]);
        return "move " + to_string(coordToDirection(x)) + "," + to_string(coordToDirection(y));
    } else {
        random_device rd;
        uniform_int_distribution<int> val(-1, 1);
        return "move " + to_string(val(rd)) + "," + to_string(val(rd));
    }
}

string BorisGuillaumator::name() const {
    return this->borisGuillaumatorName;
}