#ifndef COORDINATEVALIDATOR_H
#define COORDINATEVALIDATOR_H

#include "coordinate.h"

class CoordinateValidator {
public:
    static bool validateCoordinate(const Coordinate& coord);
    static bool validateLatitude(double lat);
    static bool validateLongitude(double lon);
    static QString validateAndCorrect(Coordinate& coord);
};

#endif // COORDINATEVALIDATOR_H
