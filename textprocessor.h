#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <QString>
#include "coordinate.h"
#include "extractionresult.h"

class TextProcessor {
public:
    static ExtractionResult processText(const QString& text);

private:
    static QString extractContextSentence(const QString& text, int position);
    static QString extractCoordinateName(const QString& context, const Coordinate& coord);
};

#endif // TEXTPROCESSOR_H
