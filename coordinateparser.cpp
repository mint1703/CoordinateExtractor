#include "coordinateparser.h"
#include <cmath>
#include <QDebug>

CoordinateParser::CoordinateParser() {
    initializePatterns();
}

void CoordinateParser::initializePatterns() {
    // 1. Десятичные градусы с знаками
    patterns.append({QRegularExpression(R"((-?\d+\.\d+)\s+(-?\d+\.\d+))"), CoordinateFormat::Decimal});

    // 2. Десятичные с латинскими направлениями
    patterns.append({QRegularExpression(R"(([NS]?\d+\.\d+)\s*([EW]?\d+\.\d+))", QRegularExpression::CaseInsensitiveOption), CoordinateFormat::Decimal});

    // 3. Десятичные с кириллическими направлениями
    patterns.append({QRegularExpression(R"((\d+\.\d+)[СЮ]\s+(\d+\.\d+)[ВЗ])", QRegularExpression::CaseInsensitiveOption), CoordinateFormat::Decimal});
    patterns.append({QRegularExpression(R"((\d+\.\d+)[СС]\.?ш\.?\s+(\d+\.\d+)[ВЗ]\.?д\.?)", QRegularExpression::CaseInsensitiveOption), CoordinateFormat::Decimal});

    // 4. Градусы-минуты с направлениями
    patterns.append({QRegularExpression(R"((\d+)-(\d+\.?\d*)([NS])\s+(\d+)-(\d+\.?\d*)([EW]))", QRegularExpression::CaseInsensitiveOption), CoordinateFormat::DM});

    // 5. Универсальный DMS/DM паттерн
    patterns.append({QRegularExpression(R"((\d+)°\s*(\d+\.?\d*)[′']?\s*(\d*\.?\d*)[″\"]?\s*([NSСЮ]?)\s*[,;]?\s*(\d+)°\s*(\d+\.?\d*)[′']?\s*(\d*\.?\d*)[″\"]?\s*([EWЗВ]?))", QRegularExpression::CaseInsensitiveOption), CoordinateFormat::DMS});

    // 6. Компактные форматы
    patterns.append({QRegularExpression(R"((\d{4,6}[NS])\s+(\d{5,7}[EW]))", QRegularExpression::CaseInsensitiveOption), CoordinateFormat::D});

    // 7. Десятичные с запятыми
    patterns.append({QRegularExpression(R"((-?\d+,\d+°)\s*[,;]?\s*(-?\d+,\d+°))", QRegularExpression::CaseInsensitiveOption), CoordinateFormat::Decimal});
}

QList<Coordinate> CoordinateParser::parseText(const QString& text) {
    QList<Coordinate> coordinates;
    QSet<QString> uniqueCoordinates;

    for (const auto& patternInfo : patterns) {
        QRegularExpressionMatchIterator it = patternInfo.pattern.globalMatch(text);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            Coordinate coord = parseMatch(match, patternInfo.format, text, match.capturedStart());

            // Проверяем на дубликаты по позиции и тексту
            QString coordKey = QString("%1_%2").arg(coord.positionInText).arg(coord.originalText);
            if (coord.isValid() && !uniqueCoordinates.contains(coordKey)) {
                coordinates.append(coord);
                uniqueCoordinates.insert(coordKey);
            }
        }
    }

    return coordinates;
}

Coordinate CoordinateParser::parseMatch(const QRegularExpressionMatch& match, CoordinateFormat format, const QString& text, int position) {
    Coordinate coord;
    coord.originalText = match.captured();
    coord.positionInText = position;
    coord.format = format;

    try {
        switch (format) {
        case CoordinateFormat::Decimal: {
            if (match.capturedTexts().size() >= 3) {
                QString latStr = match.captured(1);
                QString lonStr = match.captured(2);

                coord.latitude = parseDecimal(latStr, getDirection(latStr, true));
                coord.longitude = parseDecimal(lonStr, getDirection(lonStr, false));
            }
            break;
        }
        case CoordinateFormat::DMS: {
            if (match.capturedTexts().size() >= 9) {
                QString latDeg = match.captured(1);
                QString latMin = match.captured(2);
                QString latSec = match.captured(3);
                QString latDir = match.captured(4);

                QString lonDeg = match.captured(5);
                QString lonMin = match.captured(6);
                QString lonSec = match.captured(7);
                QString lonDir = match.captured(8);

                coord.latitude = parseDMS(latDeg, latMin, latSec, latDir);
                coord.longitude = parseDMS(lonDeg, lonMin, lonSec, lonDir);
            }
            break;
        }
        case CoordinateFormat::DM: {
            if (match.capturedTexts().size() >= 7) {
                QString latDeg = match.captured(1);
                QString latMin = match.captured(2);
                QString latDir = match.captured(3);

                QString lonDeg = match.captured(4);
                QString lonMin = match.captured(5);
                QString lonDir = match.captured(6);

                coord.latitude = parseDM(latDeg, latMin, latDir);
                coord.longitude = parseDM(lonDeg, lonMin, lonDir);
            }
            break;
        }
        case CoordinateFormat::D: {
            break;
        }
        default:
            break;
        }
    } catch (...) {
    }

    return coord;
}

double CoordinateParser::parseDMS(const QString& degrees, const QString& minutes, const QString& seconds, const QString& direction) {
    double deg = degrees.toDouble();
    double min = minutes.isEmpty() ? 0 : minutes.toDouble();
    double sec = seconds.isEmpty() ? 0 : seconds.toDouble();

    double decimal = deg + min/60.0 + sec/3600.0;

    QString upperDir = direction.toUpper();
    if (upperDir.contains("S") || upperDir.contains("Ю")) {
        decimal = -decimal;
    }
    if (upperDir.contains("W") || upperDir.contains("З")) {
        decimal = -decimal;
    }

    return decimal;
}

double CoordinateParser::parseDM(const QString& degrees, const QString& minutes, const QString& direction) {
    double deg = degrees.toDouble();
    double min = minutes.toDouble();

    double decimal = deg + min/60.0;

    if (direction.contains("S", Qt::CaseInsensitive) ||
        direction.contains("Ю", Qt::CaseInsensitive)) {
        decimal = -decimal;
    }
    if (direction.contains("W", Qt::CaseInsensitive) ||
        direction.contains("З", Qt::CaseInsensitive)) {
        decimal = -decimal;
    }

    return decimal;
}

double CoordinateParser::parseDecimal(const QString& value, const QString& direction) {
    QString cleanValue = value;
    cleanValue.remove(QRegularExpression("[^0-9.,-]"));
    cleanValue.replace(',', '.');

    double decimal = cleanValue.toDouble();

    QString upperDir = direction.toUpper();

    if (upperDir == "S" || upperDir == "Ю") {
        decimal = -decimal;
    }
    else if (upperDir == "W" || upperDir == "З") {
        decimal = -decimal;}

    return decimal;
}

QString CoordinateParser::getDirection(const QString& text, bool isLatitude) const {
    QString upperText = text.toUpper();

    if (isLatitude) {
        if (upperText.contains('N') || upperText.contains("С")) {
            return "N";
        }
        if (upperText.contains('S') || upperText.contains("Ю")) {
            return "S";
        }
    } else {
        if (upperText.contains('E') || upperText.contains("В")) {
            return "E";
        }
        if (upperText.contains('W') || upperText.contains("З")) {
            return "W";
        }
    }

    return "";
}
