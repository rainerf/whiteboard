#ifndef COLORACTION_H
#define COLORACTION_H

#include <QAction>
#include <QColor>
#include <QPixmap>

class ColorAction : public QAction {
    Q_OBJECT

public:
    ColorAction(QColor const &color, QObject *parent = nullptr);

signals:
    void colorSelected(QColor const &color);

private:
    QColor m_color;

    QPixmap createIcon();
};

#endif // COLORACTION_H
