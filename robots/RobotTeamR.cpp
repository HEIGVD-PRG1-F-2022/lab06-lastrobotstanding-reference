/**
 * @author : Rachel Tranchida & Rafael Dousse
 * @date : 29.11.2022
 * @file : RobotTeamR
 * @project : Refactor
 */
#include "RobotTeamR.h"
#include "librobots/Message.h"


using namespace std;


string RobotTeamR::action(vector<string> updates) {
    Action currentAction;
    int dxOppositeRobot;
    int dyOppositeRobot;
    int damageReceived;
    string responseAction = " ";
    vector<string> paramsDamage;

    for (const auto &info: updates) {
        size_t compteur = 0;
        size_t compteur2 = 0;
        currentAction = findAction(info);

        vector<string> params = split(info, " ", 1);
        cout << "\nAction : " << currentAction << endl;
        for (const auto &i: params) {
            cout << "Param " << compteur << " = " << i;
            ++compteur;
            cout << endl;
        }


        switch (currentAction) {

            case NOTHING:
                break;
            case BOARD:
                //cout << "Board";
                break;
            case DAMAGE:
                paramsDamage = split(params.at(1), ",");
                for (const auto &i: paramsDamage) {
                    cout << "ParamDamage " << compteur2 << " = " << i;
                    ++compteur2;
                    cout << endl;
                }
                dxOppositeRobot = stoi(paramsDamage.at(0));
                dyOppositeRobot = stoi(paramsDamage.at(1));

                damageReceived = stoi(paramsDamage.at(2));
                cout << "energy avant attack: " << energy << endl;
                energy -= damageReceived;
                cout << "energy apres attack: " << energy << endl;
                cout << "dxOppositeRobot = " << dxOppositeRobot << endl;
                cout << "dyOppositeRobot = " << dyOppositeRobot << endl;
                //cout << "damage received = " << damageReceived << endl;


                if (energy <= 4) {
                    dxOppositeRobot = -dxOppositeRobot;
                    dyOppositeRobot = -dyOppositeRobot;
                    responseAction = "move " + to_string(dxOppositeRobot) + "," + to_string(dyOppositeRobot);

                    return responseAction;
                } else {
                    responseAction = "attack " + to_string(dxOppositeRobot) + "," + to_string(dyOppositeRobot);

                    return responseAction;
                }

                break;
            case BONUS:
                cout << "bonus";

                break;
        }

    }


    return "";
/*    for (const auto &update: updates) {
        size_t compteur = 0;
        vector<string> params = split(update, " ");
        cout << "Test param 0 : " << params.at(0) << endl;
        cout << "Test param 1 : " << params.at(1) << endl;
        cout << "Update recu : " << updates.at(0) << endl;
        cout << "Update recu : " << updates.at(1) << endl;
        cout << "update dans updates : " << update << endl;
        for (auto i: params) {
            cout << "Param " << compteur << " = " << i;
            ++compteur;
            cout << endl;
        }

    }

    return " ";*/

}

string RobotTeamR::name() const {
    return "RobotTeamR";
}

RobotTeamR::Action RobotTeamR::findAction(std::string action) {

    for (const auto &i: ACTIONS) {
        if (action.find(i.actionName) != string::npos) {
            return i.action;
        }
    }

    return NOTHING;
}


void RobotTeamR::setConfig(size_t width, size_t height, unsigned int energy, unsigned int power) {
    this->width = width;
    this->height = height;
    this->energy = energy;
    this->power = power;

}

void updateDamage(string damage) {

}
