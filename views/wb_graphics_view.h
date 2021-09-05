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
        return &m_undoStack;
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
    void cut();

    void selectAll();
    void saveToFile(QString const &filename);
    void loadFromFile(QString const &filename);
    void clear();

signals:
    void toolInUse(bool);
    void fileModified(bool);

private:
    void tabletEvent(QTabletEvent *event) override;

    WB_GraphicsScene *m_scene;

    bool m_deviceDown = false;

    PenTool m_penTool;
    TextTool m_textTool;
    HighlightTool m_highlightTool;
    PointerTool m_pointerTool;
    ZoomTool m_zoomTool;
    PanTool m_panTool;
    SelectTool m_selectTool;
    ToolBase *m_currentTool = &m_penTool;

    QUndoStack m_undoStack;
};

#endif // WHITEBOARDGRAPHICSVIEW_H
