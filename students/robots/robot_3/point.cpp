/*
-----------------------------------------------------------------------------------
Project Name    : lab06-lastrobotstanding-comme_tu_veux
File's Name     : Point3.cpp
Author          :   Aellen Quentin
                    Atasever Mehmet
                    Salamin Chloé
Creation Date   : 02.12.2022
Description     :
Remarque(s)     :
Compiler        : Mingw-w64 g++ 11.2.0
-----------------------------------------------------------------------------------
*/

#include "point.h"
#include "librobots/Message.h"
#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

Point3 Point3::fromStrToPoint3(const string &coords, const string &sep) {
    vector<string> points = split(coords, ",", 2);

    int x, y;
    stringstream xCoord(points.at(0));
    xCoord >> x;
    stringstream yCoord(points.at(1));
    yCoord >> y;

    //return a new Point3 object without segmentation fault
    return {x, y};
}

Point3::Point3() {
    this->x = 0;
    this->y = 0;
}

Point3::operator string() const {
    return to_string(x) + "," + to_string(y);
}

bool Point3::operator==(Point3 coords) const {
    return this->x == coords.x && this->y == coords.y;
}

Point3 &Point3::operator+=(const Point3 &rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}

Point3 operator+(Point3 lhs, const Point3 &rhs) {
    lhs += rhs;
    return lhs;
}

Point3 &Point3::wrap(Point3 &coords, int min, int max) {
    int wrapSize = max - min + 1;
    int valueX = coords.x, valueY = coords.y;

    if (valueX < min) {
        valueX += wrapSize * ((min - valueX) / wrapSize + 1);
    }

    if (valueY < min) {
        valueY += wrapSize * ((min - valueY) / wrapSize + 1);
    }
    coords.x = min + (valueX - min) % wrapSize;
    coords.y = min + (valueY - min) % wrapSize;

    return coords;
}

int Point3::distance(Point3 coords) const{
    return (int) sqrt(pow(this->x - coords.x, 2) + pow(this->y - coords.y, 2));
}


