QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    csvbuilder.cpp \
    csvparser.cpp \
    date.cpp \
    filter.cpp \
    invoice.cpp \
    main.cpp \
    mainwindow.cpp \
    offer.cpp \
    payment.cpp \
    penaltiesCalulator.cpp \
    program.cpp \
    reportformer.cpp

HEADERS += \
    csvbuilder.h \
    csvparser.h \
    date.h \
    filter.h \
    invoice.h \
    mainwindow.h \
    offer.h \
    payment.h \
    penaltiesCalulator.h \
    report.h \
    reportformer.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
