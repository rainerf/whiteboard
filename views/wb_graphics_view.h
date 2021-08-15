// This file is part of Whiteboard.
//
// Whiteboard is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Whiteboard is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Whiteboard.  If not, see <https://www.gnu.org/licenses/>.

#ifndef WHITEBOARDGRAPHICSVIEW_H
#define WHITEBOARDGRAPHICSVIEW_H

#include <QUndoStack>

#include "lib/interactive_view.h"
#include "tools/highlight_tool.h"
#include "tools/pen_tool.h"
#include "tools/pointer_tool.h"
#include "tools/select_tool.h"
#include "tools/text_tool.h"
#include "tools/zoom_tool.h"
#include "tools/pan_tool.h"

class WB_GraphicsScene;

class WB_GraphicsView : public InteractiveView {
    Q_OBJECT

public:
    WB_GraphicsView(QWidget *parent);

    QUndoStack * getUndoStack() {
        return &undoStack;
    }

    WB_GraphicsScene *scene() {
        return m_scene;
    }

public slots:
    void setColor(QColor const &color);
    void setPenThickness(int thickness);

    void setPenTool();
    void setTextTool();
    void setHighlightTool();
    void setPointerTool();
    void setZoomTool();
    void setPanTool();
    void setSelectTool();

    void deleteSelectedItems();
    void debugDumpAllItems();

    void setFont(QFont const &font);
    void setFontSize(int size);

    void paste();
    void copy();

    void selectAll();
    void saveToFile(QString const &filename);
    void loadFromFile(QString const &filename);
    void clear();

signals:
    void toolInUse(bool);

private:
    void tabletEvent(QTabletEvent *event) override;

    WB_GraphicsScene *m_scene;

    bool m_deviceDown = false;

    PenTool penTool;
    TextTool textTool;
    HighlightTool highlightTool;
    PointerTool pointerTool;
    ZoomTool zoomTool;
    PanTool panTool;
    SelectTool selectTool;
    ToolBase *currentTool = &penTool;

    QUndoStack undoStack;
};

#endif // WHITEBOARDGRAPHICSVIEW_H
