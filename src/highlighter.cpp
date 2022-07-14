#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    typeFormat.setForeground(Qt::darkGreen);
//    keywordFormat.setFontWeight(QFont::Bold);
    QStringList typesPatterns;
    typesPatterns << "\\bchar\\b"
                    << "\\bdouble\\b" << "\\bint\\b" << "\\blong\\b"
                    << "\\bshort\\b" << "\\bsigned\\b" << "\\bunsigned\\b"
                    << "\\bvoid\\b" << "\\bbool\\b"
                    << "\\bint16_t\\b" << "\\buint16_t\\b"
                    << "\\bint32_t\\b" << "\\buint32_t\\b"
                    << "\\bint8_t\\b" << "\\buint8_t\\b";

    foreach (const QString &pattern, typesPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = typeFormat;
        highlightingRules.append(rule);
    }

    keywordFormat.setForeground(Qt::darkBlue);
//    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bconst\\b"<< "\\bexplicit\\b" << "\\bvolatile\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bconst\\b"
                    << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bsignals\\b" << "\\bslots\\b" << "\\bstatic\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bvirtual\\b";

    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classTypeFormat.setForeground(Qt::darkBlue);
    classTypeFormat.setFontWeight(QFont::Bold);
    QStringList classPatterns;
    classPatterns << "\\bclass\\b" << "\\bstruct\\b" << "\\bunion\\b" << "\\benum\\b";

    foreach (const QString &pattern, classPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = classTypeFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::gray);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::gray);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                    + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
