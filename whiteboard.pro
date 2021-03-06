QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets svg

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
    actions/color_action.cpp \
    actions/pen_action.cpp \
    items/wb_item_group.cpp \
    items/wb_pixmap_item.cpp \
    items/wb_svg_item.cpp \
    items/wb_text_item.cpp \
    lib/interactive_view.cpp \
    lib/qgraphicsscene_dump.cpp \
    lib/qgraphicsscene_storage.cpp \
    main.cpp \
    tools/highlight_tool.cpp \
    tools/pan_zoom_tool.cpp \
    tools/pen_tool.cpp \
    tools/pointer_tool.cpp \
    tools/select_tool.cpp \
    tools/text_tool.cpp \
    core/delete_command.cpp \
    core/import_export_support.cpp \
    core/paste_command.cpp \
    core/wb_graphics_scene.cpp \
    core/wb_graphics_view.cpp \
    ui/wb_mainwindow.cpp \
    ui/wb_paint_window.cpp

HEADERS += \
    actions/color_action.h \
    actions/pen_action.h \
    items/wb_item.h \
    items/wb_item_group.h \
    items/wb_pixmap_item.h \
    items/wb_svg_item.h \
    items/wb_text_item.h \
    lib/interactive_view.h \
    lib/qgraphicsscene_dump.h \
    lib/qgraphicsscene_storage.h \
    lib/tablet_application.h \
    tools/highlight_tool.h \
    tools/pan_zoom_tool.h \
    tools/pen_tool.h \
    tools/pointer_tool.h \
    tools/select_tool.h \
    tools/text_tool.h \
    tools/tool_base.h \
    core/delete_command.h \
    core/import_export_support.h \
    core/paste_command.h \
    core/wb_graphics_scene.h \
    core/wb_graphics_view.h \
    ui/wb_mainwindow.h \
    ui/wb_paint_window.h

FORMS += \
    ui/wb_paint_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

RC_ICONS = icons/appico.ico
