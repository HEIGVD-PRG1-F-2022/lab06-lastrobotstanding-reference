// ================================================================================
// File : Point.h
// Project name : LABO - LastRobotStanding
// Project members :
// - Florian Duruz
// File created by Florian Duruz on 30/11/2022.
// ================================================================================

#ifndef LASTROBOTSTANDING_POINT_H
#define LASTROBOTSTANDING_POINT_H

#include <iostream>
#include <cassert>
#include <cmath>

class Point
{
    long int x = 0;
    long int y = 0;

    //friend Point operator=(Point& lhs, const Point& rhs);
    friend Point operator+(Point lhs, const Point& rhs);
    friend Point operator-(Point lhs, const Point& rhs);
    friend bool operator==(const Point& lhs, const Point& rhs);
    friend std::ostream& operator<<(std::ostream& lhs, const Point& rhs);

public:
    Point(): x(0), y(0){};
    Point(long int x, long int y): x(x), y(y){};

    Point& operator+=(const Point& rhs);
    Point& operator-=(const Point& rhs);
    //Point& operator=( const Point& rhs);
    double mag() const;

    double dist(const Point& rhs) const;

    long int getX() const;
    long int getY() const;

    Point normalize() const;
};

#endif //LASTROBOTSTANDING_POINT_H
