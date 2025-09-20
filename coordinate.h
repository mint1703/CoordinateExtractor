#ifndef COORDINATE_H
#define COORDINATE_H

#include <QString>
#include <QJsonObject>

enum class CoordinateFormat {
    Decimal,
    DMS,
    DM,
    D,
    Unknown
};

enum class GeometryType {
    SinglePoint,
    Line,
    Polygon,
    Unknown
};

struct Coordinate {
    double latitude;
    double longitude;
    QString originalText;
    CoordinateFormat format;
    QString contextSentence;
    QString coordinateName;
    int positionInText;

    QJsonObject toJson() const;
    bool isValid() const;
};

#endif // COORDINATE_H
