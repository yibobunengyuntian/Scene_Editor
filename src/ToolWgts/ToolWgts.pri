FORMS += \
    $$PWD/checkboxmenu.ui \
    $$PWD/errorwgt.ui \
    $$PWD/loadingwgt.ui \
    $$PWD/newprojectwgt.ui \
    $$PWD/setwgt.ui \
    $$PWD/tipwgt.ui \
    $$PWD/titebar.ui \
    $$PWD/viewcube.ui

HEADERS += \
    $$PWD/checkboxmenu.h \
    $$PWD/errorwgt.h \
    $$PWD/lineedit.h \
    $$PWD/loadingwgt.h \
    $$PWD/newprojectwgt.h \
    $$PWD/setwgt.h \
    $$PWD/tipwgt.h \
    $$PWD/titebar.h \
    $$PWD/viewcube.h

SOURCES += \
    $$PWD/checkboxmenu.cpp \
    $$PWD/errorwgt.cpp \
    $$PWD/lineedit.cpp \
    $$PWD/loadingwgt.cpp \
    $$PWD/newprojectwgt.cpp \
    $$PWD/setwgt.cpp \
    $$PWD/tipwgt.cpp \
    $$PWD/titebar.cpp \
    $$PWD/viewcube.cpp

include($$PWD/ImageView/ImageView.pri)
include($$PWD/MultiCombox/MultiCombox.pri)
include($$PWD/QtColorButton/QtColorButton.pri)
include($$PWD/StartWgt/StartWgt.pri)


DISTFILES +=
