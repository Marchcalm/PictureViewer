#-------------------------------------------------
#
# Project created by QtCreator 2019-08-09T11:00:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PictureViewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        src/pubcontrols/pub_subwindow.cpp \
        src/uiglobal.cpp \
        src/pubcontrols/pub_label.cpp \
        src/pubcontrols/pub_pushbutton.cpp \
        src/pubcontrols/pub_window.cpp \
        src/main.cpp \
        src/uimodules/appsettins/appsettinswidget.cpp \
        src/uimodules/maintitlebar/maintitlebar.cpp \
        src/mainwindow.cpp \
        src/uimodules/picturescene/picturequickviewbutton.cpp \
        src/uimodules/picturescene/picturescene.cpp \
        src/uimodules/picturescene/picturetoolbar.cpp \
        src/uimodules/picturescene/pixmapitem.cpp

HEADERS += \
        src/pubcontrols/pub_subwindow.h \
        src/uimodules/appsettins/appsettinswidget.h \
        src/uimodules/maintitlebar/maintitlebar.h \
        src/mainwindow.h \
        src/uimodules/picturescene/picturequickviewbutton.h \
        src/uimodules/picturescene/picturescene.h \
        src/uimodules/picturescene/picturetoolbar.h \
        src/uimodules/picturescene/pixmapitem.h \
        src/uiglobal.h \
        src/pubcontrols/pub_label.h \
        src/pubcontrols/pub_pushbutton.h \
        src/pubcontrols/pub_window.h

INCLUDEPATH += src
INCLUDEPATH += src/pubcontrols
INCLUDEPATH += src/uimodules

MBDIR = moc
MOC_DIR += $${MBDIR}
OBJECTS_DIR += $${MBDIR}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
