#
# VLC-Qt Simple Player
# Copyright (C) 2015 Tadej Novak <tadej@tano.si>
#

TARGET      = simple-player
TEMPLATE    = app
CONFIG 	   += c++11

QT         += widgets network multimedia

SOURCES    += main.cpp \
    MainWindow.cpp

HEADERS    += MainWindow.h

FORMS      += MainWindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Qt/Qt5.7.0/5.7/mingw53_32/lib/ -lsfml-window
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Qt/Qt5.7.0/5.7/mingw53_32/lib/ -lsfml-window-d
else:unix: LIBS += -L$$PWD/../../../../Qt/Qt5.7.0/5.7/mingw53_32/lib/ -lsfml-window

INCLUDEPATH += $$PWD/../../../../Qt/Qt5.7.0/5.7/mingw53_32/include
DEPENDPATH += $$PWD/../../../../Qt/Qt5.7.0/5.7/mingw53_32/include
