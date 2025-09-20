#ifndef EXTRACTIONRESULT_H
#define EXTRACTIONRESULT_H

#include <QList>
#include <QJsonObject>
#include "coordinate.h"

struct ExtractionResult {
    QList<Coordinate> coordinates;
    GeometryType geometryType;
    QString error;

    QJsonObject toJson() const;
    bool hasError() const;
};

#endif // EXTRACTIONRESULT_H
