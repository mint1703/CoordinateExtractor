
#include "geometryanalyzer.h"
#include <cmath>
#include <QVector>
#include <QPointF>

GeometryType GeometryAnalyzer::analyzeGeometry(const QList<Coordinate>& coordinates) {
    if (coordinates.isEmpty()) {
        return GeometryType::Unknown;
    }

    if (coordinates.size() == 1) {
        return GeometryType::SinglePoint;
    }

    if (isPolygon(coordinates)) {
        return GeometryType::Polygon;
    }

    if (isLine(coordinates)) {
        return GeometryType::Line;
    }

    return GeometryType::SinglePoint;
}

bool GeometryAnalyzer::isPolygon(const QList<Coordinate>& coordinates) {
    if (coordinates.size() < 3) {
        return false;
    }

    const Coordinate& first = coordinates.first();
    const Coordinate& last = coordinates.last();

    double distance = calculateDistance(first, last);
    return distance < 0.001;
}

bool GeometryAnalyzer::isLine(const QList<Coordinate>& coordinates) {
    if (coordinates.size() < 2) {
        return false;
    }

    return !isPolygon(coordinates);
}

double GeometryAnalyzer::calculateDistance(const Coordinate& coord1, const Coordinate& coord2) {
    double lat1 = coord1.latitude * M_PI / 180.0;
    double lon1 = coord1.longitude * M_PI / 180.0;
    double lat2 = coord2.latitude * M_PI / 180.0;
    double lon2 = coord2.longitude * M_PI / 180.0;

    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    double a = sin(dlat/2) * sin(dlat/2) +
               cos(lat1) * cos(lat2) *
                   sin(dlon/2) * sin(dlon/2);

    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    return 6371 * c * 1000;
}
