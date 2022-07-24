QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/drawCharactersWidget.cpp \
    src/drawEditWidget.cpp \
    src/drawGlyphsWidget.cpp \
    src/editWidget.cpp \
    src/fontWidget.cpp \
    src/glyph.cpp \
    src/glyphsWidget.cpp \
    src/highlighter.cpp \
    src/ioFontGode.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/settings.cpp

HEADERS += \
    include/drawCharactersWidget.h \
    include/drawEditWidget.h \
    include/drawGlyphsWidget.h \
    include/editWidget.h \
    include/fontWidget.h \
    include/glyph.h \
    include/glyphsWidget.h \
    include/highlighter.h \
    include/ioFontGode.h \
    include/mainwindow.h \
    include/settings.h

FORMS += \
    ui/editWidget.ui \
    ui/fontWidget.ui \
    ui/glyphsWidget.ui \
    ui/mainwindow.ui \
    ui/settings.ui

TRANSLATIONS += \
    AdafruitFontCreator_ru_RU.ts

INCLUDEPATH += include
CONFIG   += rtti
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
