#ifndef GEOMETRYANALYZER_H
#define GEOMETRYANALYZER_H

#include "coordinate.h"

class GeometryAnalyzer {
public:
    static GeometryType analyzeGeometry(const QList<Coordinate>& coordinates);
    static bool isPolygon(const QList<Coordinate>& coordinates);
    static bool isLine(const QList<Coordinate>& coordinates);
    static double calculateDistance(const Coordinate& coord1, const Coordinate& coord2);
};

#endif // GEOMETRYANALYZER_H
