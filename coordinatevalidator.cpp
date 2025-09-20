#include "coordinatevalidator.h"

bool CoordinateValidator::validateCoordinate(const Coordinate& coord) {
    return validateLatitude(coord.latitude) && validateLongitude(coord.longitude);
}

bool CoordinateValidator::validateLatitude(double lat) {
    return lat >= -90.0 && lat <= 90.0;
}

bool CoordinateValidator::validateLongitude(double lon) {
    return lon >= -180.0 && lon <= 180.0;
}

QString CoordinateValidator::validateAndCorrect(Coordinate& coord) {
    if (!validateLatitude(coord.latitude)) {
        if (coord.latitude < -90) coord.latitude = -90;
        else if (coord.latitude > 90) coord.latitude = 90;
        return "Latitude corrected to valid range";
    }

    if (!validateLongitude(coord.longitude)) {
        if (coord.longitude < -180) coord.longitude = -180;
        else if (coord.longitude > 180) coord.longitude = 180;
        return "Longitude corrected to valid range";
    }

    return "Coordinate is valid";
}
