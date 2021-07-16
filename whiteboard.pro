QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += debug_and_release

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/lib/gcc/x86_64-linux-gnu/9/include/

SOURCES += \
    actions/coloraction.cpp \
    items/whiteboarditemgroup.cpp \
    items/whiteboardpixmapitem.cpp \
    lib/qgraphicsscenestorage.cpp \
    tools/highlighttool.cpp \
    lib/interactiveview.cpp \
    main.cpp \
    mainwindow.cpp \
    actions/penaction.cpp \
    tools/pantool.cpp \
    tools/pentool.cpp \
    tools/pointertool.cpp \
    tools/texttool.cpp \
    tools/zoomtool.cpp \
    views/whiteboardcommands.cpp \
    views/whiteboardgraphicsview.cpp \
    items/whiteboardtextitem.cpp

HEADERS += \
    actions/coloraction.h \
    items/whiteboarditem.h \
    items/whiteboarditemgroup.h \
    items/whiteboardpixmapitem.h \
    lib/qgraphicsscenestorage.h \
    lib/tabletapplication.h \
    tools/highlighttool.h \
    lib/interactiveview.h \
    mainwindow.h \
    actions/penaction.h \
    tools/pantool.h \
    tools/pentool.h \
    tools/pointertool.h \
    tools/texttool.h \
    tools/zoomtool.h \
    views/whiteboardcommands.h \
    views/whiteboardgraphicsview.h \
    items/whiteboardtextitem.h \
    tools/whiteboardtool.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
