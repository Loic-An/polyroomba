#include <array>
#include <math.h>

typedef std::array<short, 2> coord_t;
class Coord
{
public:
    Coord() : norme(0), x(0), y(0), angle(0){};
    Coord(float distance, float angle) : norme(distance), x(distance * cos(angle)), y(distance * sin(angle)), angle(angle){};
    Coord(float x, float y) : norme(sqrt(x * x + y * y)), x(x), y(y), angle(x /){};
    Coord operator+(const Coord &obj);
    Coord operator+(const float distance);
    const float angle;
    const float norme;
    const float x;
    const float y;
};