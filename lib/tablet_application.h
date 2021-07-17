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
