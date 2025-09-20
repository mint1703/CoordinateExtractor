#ifndef COORDINATEPARSER_H
#define COORDINATEPARSER_H

#include <QString>
#include <QRegularExpression>
#include "coordinate.h"

class CoordinateParser {
public:
    CoordinateParser();
    QList<Coordinate> parseText(const QString& text);

private:
    struct RegexPattern {
        QRegularExpression pattern;
        CoordinateFormat format;
    };

    QList<RegexPattern> patterns;

    void initializePatterns();
    Coordinate parseMatch(const QRegularExpressionMatch& match, CoordinateFormat format, const QString& text, int position);
    double parseDMS(const QString& degrees, const QString& minutes, const QString& seconds, const QString& direction);
    double parseDM(const QString& degrees, const QString& minutes, const QString& direction);
    double parseDecimal(const QString& value, const QString& direction);
    QString getDirection(const QString& text, bool isLatitude) const;
};

#endif // COORDINATEPARSER_H
