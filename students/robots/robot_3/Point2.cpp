// ================================================================================
// File : Point2.cpp
// Project name : LABO - LastRobotStanding
// Project members :
// - Florian Duruz, Antoine Leresche
// File created by Florian Duruz on 30/11/2022.
// ================================================================================

#include "Point2.h"

Point2 operator+(Point2 lhs, const Point2& rhs)
{
    lhs += rhs;
    return lhs;
}

Point2 operator-(Point2 lhs, const Point2& rhs)
{
    lhs -= rhs;
    return lhs;
}

bool operator==(const Point2& lhs, const Point2& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

Point2& Point2::operator+=(const Point2& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Point2& Point2::operator-=(const Point2 &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

double Point2::mag() const
{
    return hypot(x,y);
}

double Point2::dist(const Point2 &rhs) const
{
    return (rhs - *this).mag();
}

std::ostream& operator<<(std::ostream& lhs, const Point2& rhs)
{
    lhs << rhs.x << ',' << rhs.y;
    return lhs;
}

long int Point2::getX() const
{
    return x;
}

long int Point2::getY() const
{
    return y;
}

Point2 Point2::normalize() const
{
    long int xnorm = x == 0 ? 0 : (1/sqrt(x*x) * x);
    long int ynorm = y == 0 ? 0 : (1/sqrt(y*y) * y);
    return Point2(xnorm, ynorm);
}
