// ================================================================================
// File : Point.cpp
// Project name : LABO - LastRobotStanding
// Project members :
// - Florian Duruz, Antoine Leresche
// File created by Florian Duruz on 30/11/2022.
// ================================================================================

#include "Point.h"

Point operator+(Point lhs, const Point& rhs)
{
    lhs += rhs;
    return lhs;
}

Point operator-(Point lhs, const Point& rhs)
{
    lhs -= rhs;
    return lhs;
}

bool operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

Point& Point::operator+=(const Point& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Point& Point::operator-=(const Point &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

double Point::mag() const
{
    return hypot(x,y);
}

double Point::dist(const Point &rhs) const
{
    return (rhs - *this).mag();
}

std::ostream& operator<<(std::ostream& lhs, const Point& rhs)
{
    lhs << rhs.x << ',' << rhs.y;
    return lhs;
}

long int Point::getX() const
{
    return x;
}

long int Point::getY() const
{
    return y;
}

Point Point::normalize() const
{
    long int xnorm = x == 0 ? 0 : (1/sqrt(x*x) * x);
    long int ynorm = y == 0 ? 0 : (1/sqrt(y*y) * y);
    return Point(xnorm, ynorm);
}
/*
Point& Point::operator=(const Point &rhs)
{
    x = rhs.x;
    y = rhs.y;
}
*/