add_library(GeographicLib
    GeographicLib/Accumulator.cpp
    GeographicLib/AlbersEqualArea.cpp
    GeographicLib/AzimuthalEquidistant.cpp
    GeographicLib/CassiniSoldner.cpp
    GeographicLib/CircularEngine.cpp
    GeographicLib/DMS.cpp
    GeographicLib/Ellipsoid.cpp
    GeographicLib/EllipticFunction.cpp
    GeographicLib/GARS.cpp
    GeographicLib/Geocentric.cpp
    GeographicLib/GeoCoords.cpp
    GeographicLib/Geodesic.cpp
    GeographicLib/GeodesicExact.cpp
    GeographicLib/GeodesicExactC4.cpp
    GeographicLib/GeodesicLine.cpp
    GeographicLib/GeodesicLineExact.cpp
    GeographicLib/Geohash.cpp
    GeographicLib/Geoid.cpp
    GeographicLib/Georef.cpp
    GeographicLib/Gnomonic.cpp
    GeographicLib/GravityCircle.cpp
    GeographicLib/GravityModel.cpp
    GeographicLib/LambertConformalConic.cpp
    GeographicLib/LocalCartesian.cpp
    GeographicLib/MagneticCircle.cpp
    GeographicLib/MagneticModel.cpp
    GeographicLib/Math.cpp
    GeographicLib/MGRS.cpp
    GeographicLib/NormalGravity.cpp
    GeographicLib/OSGB.cpp
    GeographicLib/PolarStereographic.cpp
    GeographicLib/PolygonArea.cpp
    GeographicLib/Rhumb.cpp
    GeographicLib/SphericalEngine.cpp
    GeographicLib/TransverseMercator.cpp
    GeographicLib/TransverseMercatorExact.cpp
    GeographicLib/Utility.cpp
    GeographicLib/UTMUPS.cpp
)

target_include_directories(GeographicLib PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../include
)

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
    GeographicLib
    QCustomPlot
    Qt5::Widgets
    Qt5::Sql
    Qt5::WebEngineWidgets
    VLCQtCore
    VLCQtQml
    VLCQtWidgets
)
