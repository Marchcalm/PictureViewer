#-------------------------------------------------
#
# Project created by QtCreator 2019-08-09T11:00:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PictureViewer
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

CONFIG(debug, debug|release) {
    DESTDIR = debug
} else {
    DESTDIR = release
}
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/ui
RCC_DIR = tmp/rcc

INCLUDEPATH += \
        mainwindow \
        common/widgets

SOURCES += \
        common/widgets/pub_subwindow.cpp \
        common/widgets/pub_label.cpp \
        common/widgets/pub_pushbutton.cpp \
        common/widgets/pub_window.cpp \
        mainwindow/appsettins/appsettingsbodywidget.cpp \
        mainwindow/appsettins/appsettingsbodywidget/asb_basicsettingswidget.cpp \
        mainwindow/appsettins/appsettingsbodywidget/asb_powerpointwidget.cpp \
        mainwindow/appsettins/appsettingstabbar.cpp \
        mainwindow/appsettins/appsettinswidget.cpp \
        mainwindow/picturescene/picturequickviewbutton.cpp \
        mainwindow/picturescene/picturescene.cpp \
        mainwindow/picturescene/picturetoolbar.cpp \
        mainwindow/uiglobal.cpp \
        mainwindow/maintitlebar.cpp \
        mainwindow/mainwindow.cpp \
        mainwindow/main.cpp

HEADERS += \
        common/widgets/pub_subwindow.h \
        common/widgets/pub_label.h \
        common/widgets/pub_pushbutton.h \
        common/widgets/pub_window.h \
        mainwindow/appsettins/appsettingsbodywidget.h \
        mainwindow/appsettins/appsettingsbodywidget/asb_basicsettingswidget.h \
        mainwindow/appsettins/appsettingsbodywidget/asb_powerpointwidget.h \
        mainwindow/appsettins/appsettingstabbar.h \
        mainwindow/appsettins/appsettinswidget.h \
        mainwindow/picturescene/picturequickviewbutton.h \
        mainwindow/picturescene/picturescene.h \
        mainwindow/picturescene/picturetoolbar.h \
        mainwindow/uiglobal.h \
        mainwindow/maintitlebar.h \
        mainwindow/mainwindow.h

RC_ICONS = resource/applogo.ico

copy_rcc.commands = "xcopy /y /i $$shell_path($$PWD)\\resource\\\*.rcc $$shell_path($$DESTDIR)\\skin"
QMAKE_EXTRA_TARGETS += copy_rcc
POST_TARGETDEPS += copy_rcc
