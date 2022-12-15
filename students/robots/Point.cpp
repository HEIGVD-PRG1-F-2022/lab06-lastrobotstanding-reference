/*
-----------------------------------------------------------------------------------
Project Name    : lab06-lastrobotstanding-comme_tu_veux
File's Name     : Point.cpp
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
#include <vector>
#include <sstream>

using namespace std;

Point Point::fromStrToPoint(const string &coords, const string &sep) {
    vector<string> points = split(coords, ",", 2);

    int x, y;
    stringstream xCoord(points.at(0));
    xCoord >> x;
    stringstream yCoord(points.at(1));
    yCoord >> y;

    //return a new Point object without segmentation fault
    return {x, y};
}

Point::Point() {
    this->x = 0;
    this->y = 0;
}

Point::operator string() const {
    return to_string(x) + "," + to_string(y);
}

bool Point::operator==(Point coords) const {
    return this->x == coords.x && this->y == coords.y;
}

Point &Point::operator+=(const Point &lhs) {
    this->x += lhs.x;
    this->y += lhs.y;
    return *this;
}

Point operator+(Point &lhs, const Point &rhs) {
    lhs += rhs;
    return lhs;
}

Point &Point::wrap(Point &coords, int min, int max) {
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


