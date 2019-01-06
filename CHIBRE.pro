###############################################################################
#      CHIBRE / JASS COUNTER AND STATS                                        #
#                                                                             #
#   Author: Lazare Girardin, 2018                                             #
#            files: mainwindow, main, chibreclaculator   .h .cpp .ui          #
#                                                                             #
#                                                                             #
# UI design mostly come from Juergen Skrotzky                                 #
# all **frameless** and **Dark** files are from him.                          #
# Sources: https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle  #
#                                                                             #
###############################################################################

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

INCLUDEPATH +="framelesswindow"

TARGET      =  QtFramelessWindowDarkStyle
TEMPLATE    =  app

SOURCES     += main.cpp\
               mainwindow.cpp \
               framelesswindow/framelesswindow.cpp \
               framelesswindow/windowdragger.cpp \
               DarkStyle.cpp \
               chibrecalculator.cpp \
    qcustomplot.cpp


HEADERS     += mainwindow.h \
               framelesswindow/framelesswindow.h \
               framelesswindow/windowdragger.h \
               DarkStyle.h \
               chibrecalculator.h \
    qcustomplot.h


FORMS       += mainwindow.ui \
               framelesswindow/framelesswindow.ui

RESOURCES   += darkstyle.qrc \
               framelesswindow.qrc
