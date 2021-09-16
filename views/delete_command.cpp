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

#include "delete_command.h"
#include "views/wb_graphics_scene.h"

#include <QGraphicsItem>

DeleteCommand::DeleteCommand(WB_GraphicsScene *graphicsScene, QUndoCommand *parent) : QUndoCommand("Delete", parent), m_scene(graphicsScene) {
    m_items = graphicsScene->selectedItems();
}

void DeleteCommand::undo() {
    for (auto &&i: m_items) {
        m_scene->addItem(i);
        i->setSelected(false);
    }
}

void DeleteCommand::redo() {
    for (auto &&i : m_items) {
        m_scene->removeItem(i);
    }
}
