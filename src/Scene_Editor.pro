QT       += core gui
QT       += concurrent
QT       += winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

RC_FILE = $$PWD/icon.rc


msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}

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
    main.cpp \
    mainwgt.cpp \
    renderwgt.cpp \

HEADERS += \
    mainwgt.h \
    plc.h \
    renderwgt.h \

FORMS += \
    mainwgt.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#---------------------------------custom-------------------
include(PropertyManager/PropertyManager.pri)
include(Camera/Camera.pri)
include(Light/Light.pri)
include(Manipulator/Manipulator.pri)
include(Operator/Operator.pri)
include(ToolWgts/ToolWgts.pri)
include(Configfile/Configfile.pri)

RESOURCES += \
    res.qrc



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/assimp/lib/ -lassimp-vc140-mt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/assimp/lib/ -lassimp-vc140-mtd
else:unix: LIBS += -L$$PWD/assimp/lib/ -lassimp-vc140-mt

INCLUDEPATH += $$PWD/assimp/include
DEPENDPATH += $$PWD/assimp/include
