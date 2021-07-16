#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include <QFont>
#include <QColor>

#include "whiteboardtool.h"
#include "items/whiteboardtextitem.h"

class TextTool : public WhiteBoardTool {
public:
    class AddCommand;

    void setColor(QColor const &color);
    void setFont(QFont const &font);
    void setFontSize(int size);

    void handleTabletPress(WhiteBoardGraphicsView &view, QTabletEvent &event);
    void handleTabletMove(WhiteBoardGraphicsView &view, QTabletEvent &event);

    TextTool operator=(TextTool &) = delete;
    TextTool(TextTool &) = delete;
    TextTool() = default;

private:
    AddCommand *m_currentCommand;

    QColor m_color = QColor(200, 200, 200);
    QFont m_font;
    int m_fontSize = 8;
};
#endif // TEXTTOOL_H
