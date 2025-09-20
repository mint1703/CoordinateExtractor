#include "textprocessor.h"
#include "coordinateparser.h"
#include "coordinatevalidator.h"
#include "geometryanalyzer.h"
#include <QRegularExpression>

ExtractionResult TextProcessor::processText(const QString& text) {
    ExtractionResult result;

    CoordinateParser parser;
    result.coordinates = parser.parseText(text);

    for (Coordinate& coord : result.coordinates) {
        coord.contextSentence = extractContextSentence(text, coord.positionInText);
        coord.coordinateName = extractCoordinateName(coord.contextSentence, coord);

        if (!CoordinateValidator::validateCoordinate(coord)) {
            CoordinateValidator::validateAndCorrect(coord);
        }
    }

    QList<Coordinate> validCoords;
    for (const Coordinate& coord : result.coordinates) {
        if (coord.isValid()) {
            validCoords.append(coord);
        }
    }
    result.geometryType = GeometryAnalyzer::analyzeGeometry(validCoords);

    return result;
}

QString TextProcessor::extractContextSentence(const QString& text, int position) {
    if (position < 0 || position >= text.length()) {
        return "";
    }

    int start = qMax(0, position - 100);
    int end = qMin(text.length(), position + 100);

    QString sentence = text.mid(start, end - start).trimmed();

    if (start > 0) sentence = "..." + sentence;
    if (end < text.length()) sentence = sentence + "...";

    if (sentence.length() > 200) {
        sentence = sentence.left(200) + "...";
    }

    return sentence;
}

QString TextProcessor::extractCoordinateName(const QString& context, const Coordinate& coord) {
    QString cleanContext = context;
    cleanContext.remove(coord.originalText);

    QRegularExpression namePattern(R"((точка|point|место|location|координат[аы]|coord|вершин[аы]|угол|мыс|скала|бухта|залив)\s+([A-Za-zА-Яа-я0-9]+[A-Za-zА-Яа-я]))",
                                   QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match = namePattern.match(cleanContext);
    if (match.hasMatch()) {
        QString name = match.captured(2);
        if (name.length() > 1 && !name.toInt()) {
            return name;
        }
    }

    QRegularExpression nameAfterPattern(R"((координат[аы]|расположен[аы]?|находится|это)\s+([A-Za-zА-Яа-я]+[A-Za-zА-Яа-я]))",
                                        QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch matchAfter = nameAfterPattern.match(cleanContext);
    if (matchAfter.hasMatch()) {
        QString name = matchAfter.captured(2);
        if (name.length() > 1 && !name.toInt()) {
            return name;
        }
    }

    return "";
}
