#include "extractionresult.h"
#include <QJsonArray>

QJsonObject ExtractionResult::toJson() const {
    QJsonObject obj;

    QJsonArray coordsArray;
    for (const Coordinate& coord : coordinates) {
        coordsArray.append(coord.toJson());
    }
    obj["координаты"] = coordsArray;

    QString geometryTypeStr;
    switch (geometryType) {
    case GeometryType::SinglePoint: geometryTypeStr = "одиночные_точки"; break;
    case GeometryType::Line: geometryTypeStr = "линия"; break;
    case GeometryType::Polygon: geometryTypeStr = "замкнутый_полигон"; break;
    default: geometryTypeStr = "неизвестно";
    }
    obj["тип_геометрии"] = geometryTypeStr;

    obj["ошибка"] = error;

    return obj;
}

bool ExtractionResult::hasError() const {
    return !error.isEmpty();
}
