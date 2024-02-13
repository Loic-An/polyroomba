#include "coord.hpp"

Coord Coord::operator+(const Coord &other)
{
    return Coord(x + other.x, y + other.y);
}
Coord Coord::operator+(const float distance)
{
}