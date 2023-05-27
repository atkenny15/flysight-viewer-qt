add_library(QCustomPlot
    QCustomPlot/qcustomplot.cpp
)

target_link_libraries(QCustomPlot
    Qt5::Widgets
)

add_executable(FlysightViewer
    resource.qrc

    mainwindow.cpp
    main.cpp

    configdialog.ui
    configdialog.cpp

    acroform.cpp
    acroscoring.cpp
    flareform.cpp
    flarescoring.cpp
    performanceform.cpp
    performancescoring.cpp
    ppcform.cpp
    ppcscoring.cpp
    speedform.cpp
    speedscoring.cpp
    wideopendistanceform.cpp
    wideopendistancescoring.cpp
    wideopenspeedform.cpp
    wideopenspeedscoring.cpp

    config.cpp
    nav.cpp
    ubx.cpp
    tone.cpp

    dataview.h
    dataview.cpp
    geographicutil.cpp
    importworker.cpp
    mapview.cpp
    orthoview.cpp
    playbackview.cpp
    ppcupload.cpp
    logbookview.cpp
    scoringview.cpp
    simulationview.cpp
    videoview.cpp

    mapcore.h
    plotvalue.h

    colorcombobox.cpp
    common.cpp
    dataplot.cpp
    datapoint.cpp
    genome.cpp
    liftdragplot.cpp
    scoringmethod.cpp
    windplot.cpp
)

target_include_directories(FlysightViewer PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../include
)

target_link_libraries(FlysightViewer
    flysight::flysight
    QCustomPlot
    Qt5::Widgets
    Qt5::Sql
    Qt5::WebEngineWidgets
    VLCQtCore
    VLCQtQml
    VLCQtWidgets
)
