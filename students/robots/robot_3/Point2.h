// ================================================================================
// File : Point2.h
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

class Point2
{
    long int x = 0;
    long int y = 0;

    //friend Point2 operator=(Point2& lhs, const Point2& rhs);
    friend Point2 operator+(Point2 lhs, const Point2& rhs);
    friend Point2 operator-(Point2 lhs, const Point2& rhs);
    friend bool operator==(const Point2& lhs, const Point2& rhs);
    friend std::ostream& operator<<(std::ostream& lhs, const Point2& rhs);

public:
    Point2(): x(0), y(0){};
    Point2(long int x, long int y): x(x), y(y){};

    Point2& operator+=(const Point2& rhs);
    Point2& operator-=(const Point2& rhs);
    //Point2& operator=( const Point2& rhs);
    double mag() const;

    double dist(const Point2& rhs) const;

    long int getX() const;
    long int getY() const;

    Point2 normalize() const;
};

#endif //LASTROBOTSTANDING_POINT_H
