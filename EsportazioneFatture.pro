QT += quick xml xmlpatterns

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

RESOURCES += qml.qrc \
    assets.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(classes/classes.pri)
include(models/models.pri)

HEADERS += \
    custom.h


# Chilkat
#-------------------------------------------------
include(chilkat/chilkat.pri)
win32: LIBS += -L$$PWD/./ -llibchilkat-9.5.0
LIBS += -lwsock32
LIBS += -lws2_32
#-------------------------------------------------


# QXlsx code for Application Qt project
#-------------------------------------------------
# Repository: https://github.com/QtExcel/QXlsx
QXLSX_PARENTPATH=$$PWD/qxlsx/         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=$$PWD/qxlsx/header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=$$PWD/qxlsx/source/  # current QXlsx source path is ./source/
include (qxlsx/qxlsx.pri)
#-------------------------------------------------


INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32{
RC_ICONS = assets/img/ae.ico
}
