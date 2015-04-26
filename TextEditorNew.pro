#-------------------------------------------------
#
# Project created by QtCreator 2015-04-18T18:31:24
#
#-------------------------------------------------
QT += widgets

#TARGET = TextEditorNew
#TEMPLATE = app


SOURCES += main.cpp\
        notepad.cpp

HEADERS  += notepad.h

#FORMS    += notepad.ui

RESOURCES += application.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/application
INSTALLS += target
