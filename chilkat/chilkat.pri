SOURCES += $$files($$PWD/*.cpp, true)
HEADERS += $$files($$PWD/*.h, true)
FORMS   += $$files($$PWD/*.ui, true)
RESOURCES += \
    $$files($$PWD/*.qrc, true)
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/include
