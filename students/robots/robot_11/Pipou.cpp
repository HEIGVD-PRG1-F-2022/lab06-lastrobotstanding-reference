//
// Created by trixi on 29/11/2022.
//

#include "Pipou.h"
#include <sstream>
#include "iostream"

Pipou::Pipou(size_t width, size_t height, unsigned int energy, unsigned int power) {
    this->width = width;
    this->height = height;
    //this->board[height/2][width/2] = 'X';
    this->energy = energy;
    this->power = power;
    this->robotName = "Pipou Le Fumier";
}

const string &Pipou::getRobotName() const {
    return robotName;
}

string Pipou::action(vector<string> updates) {
    vector<int> nearestEnnemi = {0,0};
    for (auto x :updates) {
        if(x[0] == 'b'){
            for (int i = 0; i < 5; i++) { // y
                for (int j = 0; j < 5; ++j) { // x
                    //// << "i = " << i*5+j << " \n";
                    if(x[i*5+j+6] == 'r'){
                        //// << "robot ennemi en x : " << -2 + j << " y : " << - 2 + i << "\n";

                        if (nearestEnnemi[0] == 0 && nearestEnnemi[1] == 0  ||
                            ((abs(nearestEnnemi[0]) + abs(nearestEnnemi[1])) > (abs(-2 + i) + abs(-2 + j)))){
                            nearestEnnemi[0] = -2 + j;
                            nearestEnnemi[1] = -2 + i;
                        }
                    }
                }
            }
        }

        if(x[0] == 'r'){
            vector<int> ennemiRobot = checkPos(x);
            if (nearestEnnemi[0] == 0 && nearestEnnemi[0] == 0  ||
                ((abs(nearestEnnemi[0] + nearestEnnemi[1])) > (abs(ennemiRobot[0] + ennemiRobot[1])))){
                nearestEnnemi = ennemiRobot;
            }
        }

    }
    // Vérification que la virgule a été trouvée
    //// << "l'ennemi le plus proche est en x : " << nearestEnnemi[0] << " y : " << nearestEnnemi[1] << endl;
    int posxToMove = (nearestEnnemi[0] > 0) ? 1 : (nearestEnnemi[0] < 0) ? -1 : 0;
    int posyToMove = (nearestEnnemi[1] > 0) ? 1 : (nearestEnnemi[1] < 0) ? -1 : 0;
    //// << string( "move " + to_string(posxToMove) + "," + to_string(posyToMove));

    if(nearestEnnemi[0] == 0 && nearestEnnemi[0] == 0){

    }
    return string( "move " + to_string(posxToMove) + "," + to_string(posyToMove));
}

vector<int> Pipou::checkPos(string pos){
    vector<int> position;
    std::size_t commaIndex = pos.find(',');

    if (commaIndex != std::string::npos) {
        // Extraction des sous-chaînes contenant les entiers en utilisant la méthode substr()
        std::string xStr = pos.substr(0, commaIndex); // extrait "1"
        std::string yStr = pos.substr(commaIndex + 1); // extrait "1"

        // Conversion des sous-chaînes en entiers en utilisant std::stringstream et l'opérateur >>
        int x, y;

        std::stringstream(yStr) >> y;
        std::size_t SpaceIndex = pos.find(' ');
        xStr = xStr.substr(SpaceIndex + 1);
        std::stringstream(xStr) >> x;
        position.push_back(x);
        position.push_back(y);
        //// << "x = " << x << " , y = " << y << "\n";
    }
    return position;
}

unsigned int Pipou::getEnergy() const {
    return energy;
}

void Pipou::setEnergy(unsigned int energy) {
    Pipou::energy = energy;
}

unsigned int Pipou::getPower() const {
    return power;
}

void Pipou::setPower(unsigned int power) {
    Pipou::power = power;
}

void Pipou::setRobotName(const string &robotName) {
    Pipou::robotName = robotName;
}

string Pipou::name() {
    return std::string(this->robotName);
}

Pipou::Pipou() {
    this->energy = 1;
    this->power = 1;
    this->height = 1;
    this->width = 1;
}

