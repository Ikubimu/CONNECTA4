QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
# Ruta relativa para encontrar el archivo .lib
LIBS += -L$$PWD/lib -lConnect4Lib

# Ruta relativa para encontrar los archivos de cabecera
INCLUDEPATH += $$PWD/lib/

SOURCES += \
    Forgot_Password.cpp \
    Login.cpp \
    Register.cpp \
    gameboard.cpp \
    main.cpp \
    mainwindow.cpp \
    rankingtable.cpp \
    userswidget.cpp

HEADERS += \
    Forgot_Password.h \
    Login.h \
    Register.h \
    gameboard.h \
    mainwindow.h \
    rankingtable.h \
    userswidget.h

FORMS += \
    gameboard.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
