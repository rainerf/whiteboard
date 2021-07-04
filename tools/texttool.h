#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include <QFont>
#include <QColor>
#include <QGraphicsTextItem>

#include "whiteboardtool.h"

class TextTool : public WhiteBoardTool {
public:
    void setColor(QColor const &color);
    void setFont(QFont const &font);
    void setFontSize(int size);

    void handleTabletPress(WhiteBoardGraphicsView &view, QTabletEvent &event);
    void handleTabletMove(WhiteBoardGraphicsView &view, QTabletEvent &event);

    TextTool operator=(TextTool &) = delete;
    TextTool(TextTool &) = delete;
    TextTool() = default;

private:
    QColor m_color = QColor(200, 200, 200);
    QGraphicsTextItem *m_textItem;
    QFont m_font;
    int m_fontSize = 8;
};
#endif // TEXTTOOL_H
