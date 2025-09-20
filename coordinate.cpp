#include "coordinate.h"
#include <QJsonObject>

QJsonObject Coordinate::toJson() const {
    QJsonObject obj;
    obj["широта"] = QString::number(latitude, 'f', 8);
    obj["долгота"] = QString::number(longitude, 'f', 8);
    obj["оригинальный_текст"] = originalText;

    // Формат в текстовом виде
    QString formatStr;
    switch (format) {
    case CoordinateFormat::Decimal: formatStr = "десятичные_градусы"; break;
    case CoordinateFormat::DMS: formatStr = "градусы_минуты_секунды"; break;
    case CoordinateFormat::DM: formatStr = "градусы_минуты"; break;
    case CoordinateFormat::D: formatStr = "градусы"; break;
    default: formatStr = "неизвестно";
    }
    obj["формат"] = formatStr;

    obj["контекст_предложения"] = contextSentence;
    obj["название_координаты"] = coordinateName;
    obj["позиция_в_тексте"] = positionInText;
    obj["корректна"] = isValid();
    obj["статус"] = isValid() ? "корректна" : "некорректна";
    return obj;
}

bool Coordinate::isValid() const {
    return latitude >= -90 && latitude <= 90 &&
           longitude >= -180 && longitude <= 180;
}
