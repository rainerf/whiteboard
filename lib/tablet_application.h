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

#ifndef TABLETAPPLICATION_H
#define TABLETAPPLICATION_H

#include <QApplication>

// inspired by https://stackoverflow.com/a/33525420/1683161

class TabletApplication : public QApplication {
    Q_OBJECT
public:
    TabletApplication(int& argv, char** argc): QApplication(argv,argc){}
    bool event(QEvent* event){
        if (event->type() == QEvent::TabletEnterProximity || event->type() == QEvent::TabletLeaveProximity) {
            emit sendTabletActive(event->type() == QEvent::TabletEnterProximity);
            return true;
        }
        return QApplication::event(event);
}
signals:
    void sendTabletActive(bool active);
};

#endif // TABLETAPPLICATION_H
