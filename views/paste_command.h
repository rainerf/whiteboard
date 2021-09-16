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

#ifndef PASTE_COMMAND_H
#define PASTE_COMMAND_H

#include <QUndoCommand>

class WB_GraphicsScene;
class QGraphicsItem;

class PasteCommand : public QUndoCommand {
public:
    explicit PasteCommand(WB_GraphicsScene *graphicsScene, QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    QList<QGraphicsItem*> m_items;
    WB_GraphicsScene *m_scene;
};

#endif // PASTE_COMMAND_H
