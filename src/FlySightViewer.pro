#-------------------------------------------------
#
# Project created by QtCreator 2013-02-02T15:04:16
#
#-------------------------------------------------

CONFIG += conan_basic_setup
include(conanbuildinfo.pri)

QT       += core gui printsupport webenginewidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000
DEFINES += _USE_MATH_DEFINES

TARGET = FlySightViewer
TEMPLATE = app

SOURCES += main.cpp \
    acroform.cpp \
    acroscoring.cpp \
    colorcombobox.cpp \
    config.cpp \
    mainwindow.cpp \
    dataplot.cpp \
    dataview.cpp \
    nav.cpp \
    simulationview.cpp \
    tone.cpp \
    ubx.cpp \
    waypoint.cpp \
    datapoint.cpp \
    configdialog.cpp \
    mapview.cpp \
    common.cpp \
    videoview.cpp \
    windplot.cpp \
    liftdragplot.cpp \
    scoringview.cpp \
    genome.cpp \
    orthoview.cpp \
    playbackview.cpp \
    ppcform.cpp \
    speedform.cpp \
    scoringmethod.cpp \
    ppcscoring.cpp \
    speedscoring.cpp \
    wideopenspeedform.cpp \
    wideopendistanceform.cpp \
    wideopendistancescoring.cpp \
    wideopenspeedscoring.cpp \
    geographicutil.cpp \
    importworker.cpp \
    logbookview.cpp \
    performancescoring.cpp \
    performanceform.cpp \
    flareform.cpp \
    flarescoring.cpp \
    ppcupload.cpp \
    QCustomPlot/qcustomplot.cpp

HEADERS  += mainwindow.h \
    acroform.h \
    acroscoring.h \
    colorcombobox.h \
    config.h \
    datapoint.h \
    dataplot.h \
    dataview.h \
    mapcore.h \
    nav.h \
    simulationview.h \
    tone.h \
    ubx.h \
    waypoint.h \
    plotvalue.h \
    configdialog.h \
    mapview.h \
    common.h \
    videoview.h \
    windplot.h \
    liftdragplot.h \
    scoringview.h \
    genome.h \
    orthoview.h \
    playbackview.h \
    ppcform.h \
    speedform.h \
    scoringmethod.h \
    ppcscoring.h \
    speedscoring.h \
    performancescoring.h \
    performanceform.h \
    wideopenspeedform.h \
    wideopendistanceform.h \
    wideopendistancescoring.h \
    wideopenspeedscoring.h \
    geographicutil.h \
    importworker.h \
    logbookview.h \
    flareform.h \
    flarescoring.h \
    ppcupload.h \
    QCustomPlot/qcustomplot.h

FORMS    += mainwindow.ui \
    acroform.ui \
    configdialog.ui \
    simulationview.ui \
    videoview.ui \
    scoringview.ui \
    playbackview.ui \
    ppcform.ui \
    getuserdialog.ui \
    speedform.ui \
    performanceform.ui \
    wideopenspeedform.ui \
    wideopendistanceform.ui \
    logbookview.ui \
    flareform.ui

win32 {
    RC_ICONS = FlySightViewer.ico
} else {
    ICON = FlySightViewer.icns
}

RESOURCES += \
    resource.qrc

INCLUDEPATH += ../include
INCLUDEPATH += ../include/GeographicLib

win32 {
    LIBS += -L../lib
    CONFIG(debug, debug|release) {
        LIBS += -lVLCQtCored -lVLCQtQmld -lVLCQtWidgetsd
    } else {
        LIBS += -lVLCQtCore -lVLCQtQml -lVLCQtWidgets
    }
} else:macx {
    QMAKE_LFLAGS += -F../macx
    LIBS         += -framework VLCQtCore
    LIBS         += -framework VLCQtQml
    LIBS         += -framework VLCQtWidgets
} else:unix {
    LIBS += -L/usr/local/lib
    LIBS += -lVLCQtCore -lVLCQtQml -lVLCQtWidgets
}

unix {
    CONFIG+=use_lld_linker
}

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
}
