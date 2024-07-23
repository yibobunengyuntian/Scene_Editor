FORMS += \
    $$PWD/propertyswgt.ui\
    $$PWD/nodeeditwgt.ui \
    $$PWD/nodestreewgt.ui

HEADERS += \
    $$PWD/propertymanager.h \
    $$PWD/propertyswgt.h\
    $$PWD/nodeeditwgt.h \
    $$PWD/nodestreewgt.h

SOURCES += \
    $$PWD/propertymanager.cpp \
    $$PWD/propertyswgt.cpp\
    $$PWD/nodeeditwgt.cpp \
    $$PWD/nodestreewgt.cpp

include($$PWD/Nodes/Nodes.pri)
include($$PWD/Components/Components.pri)
include($$PWD/UndoCommand/UndoCommand.pri)
