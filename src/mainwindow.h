/***************************************************************************
**                                                                        **
**  FlySight Viewer                                                       **
**  Copyright 2018 Michael Cooper, Klaus Rheinwald                        **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>. **
**                                                                        **
****************************************************************************
**  Contact: Michael Cooper                                               **
**  Website: http://flysight.ca/                                          **
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dataplot.h"
#include "datapoint.h"
#include "dataview.h"
#include <QLabel>
#include <QMainWindow>
#include <QMap>
#include <QSet>
#include <QSqlDatabase>
#include <QStack>
#include <QVector>
#ifndef Q_MOC_RUN
#    include <flysight/track.hh>
#endif

class MapView;
class QCPRange;
class QCustomPlot;
class ScoringMethod;
class ScoringView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    typedef enum { Pan, Zoom, Measure, Zero, Ground, Course } Tool;

    typedef enum { Actual, Optimal } WindowMode;

    typedef enum { Time, Distance, HorizontalSpeed, VerticalSpeed } OptimizationMode;

    typedef enum {
        PPC,
        Speed,
        Performance,
        WideOpenSpeed,
        WideOpenDistance,
        Flare,
        Acro,
        smLast
    } ScoringMode;

    typedef enum { Automatic, Fixed } GroundReference;

    typedef enum { Default, SetStart, SetEnd } MapMode;

    typedef QVector<DataPoint> DataPoints;

    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

    PlotValue::Units units() const { return m_units; }

    void setRange(double lower, double upper, bool immediate = false);
    auto rangeLower() const { return mZoomLevel.rangeLower; }
    auto rangeUpper() const { return mZoomLevel.rangeUpper; }

    void setZero(double t);
    void setGround(double t);
    void setCourse(double t);

    void setTrackGround(QString trackName, double ground);
    void setTrackWindSpeed(QString trackName, double windSpeed);
    void setTrackWindDir(QString trackName, double windDIr);

    void setTool(Tool tool);
    Tool tool() const { return mTool; }

    double markStart() const { return mMarkStart; }
    double markEnd() const { return mMarkEnd; }
    bool markActive() const { return mMarkActive; }

    void setRotation(double rotation);
    double rotation() const { return m_viewDataRotation; }

    int waypointSize() const { return m_waypoints.size(); }
    const DataPoint& waypoint(int i) const { return m_waypoints[i]; }

    double getDistance(const DataPoint& dp1, const DataPoint& dp2);
    double getBearing(const DataPoint& dp1, const DataPoint& dp2);

    void setMark(double start, double end);
    void setMark(double mark);
    void clearMark();

    void setMediaCursor(double mediaCursor);
    double mediaCursor() const { return mMediaCursor; }

    void mediaCursorAddRef(QObject* parent);
    void mediaCursorRemoveRef(QObject* parent);
    int mediaCursorRef() const { return mMediaCursorRef.size(); }

    void pauseMedia();

    DataPoint interpolateDataT(double t);

    void setWindowMode(WindowMode mode);
    WindowMode windowMode() const { return mWindowMode; }

    double mass() const { return m_mass; }
    double planformArea() const { return m_planformArea; }

    double minDrag() const { return m_minDrag; }
    double minLift() const { return m_minLift; }
    double maxLift() const { return m_maxLift; }
    double maxLD() const { return m_maxLD; }

    int simulationTime() const { return m_simulationTime; }

    void setMinDrag(double minDrag);
    void setMaxLift(double maxLift);
    void setMaxLD(double maxLD);

    DataPlot* plotArea() const;

    void setLineThickness(double width);
    double lineThickness() const { return mLineThickness; }

    void setWind(double windE, double windN);
    void getWind(QString trackName, double* windE, double* windN);
    void getWindSpeedDirection(QString trackName, double* windSpeed, double* windDirection);
    bool windAdjustment() const { return mWindAdjustment; }

    double getQNE(void) const { return mFixedReference; }

    void setScoringMode(ScoringMode mode);
    ScoringMode scoringMode() const { return mScoringMode; }
    ScoringMethod* scoringMethod(int i) const { return mScoringMethods[i]; }

    void prepareDataPlot(DataPlot* plot);
    void prepareMapView(MapView* plot);

    bool updateReference(double lat, double lon);

    void importFromDatabase(const QString& uniqueName);
    void importFromCheckedTrack(const QString& uniqueName);

    void setTrackName(const QString& trackName);
    QString trackName() const { return mTrackName; }

    void setSelectedTracks(QVector<QString> tracks);
    void setTrackDescription(const QString& trackName, const QString& description);

    QString trackDescription(const QString& trackName);
    QDateTime trackStartTime(const QString& trackName);

    void setTrackChecked(const QString& trackName, bool checked);
    bool trackChecked(const QString& trackName) const;

    QString databasePath() const { return mDatabasePath; }

    void setMapMode(MapMode newMapMode);
    MapMode mapMode() const { return mMapMode; }

    const std::optional<flysight::Track>& track() const noexcept { return m_track; }

protected:
    void hideEvent(QHideEvent* event);
    void showEvent(QShowEvent* event);
    void closeEvent(QCloseEvent* event);

private slots:
    void on_actionImport_triggered();
    void on_actionImportFolder_triggered();

    void on_actionElevation_triggered();
    void on_actionVerticalSpeed_triggered();
    void on_actionHorizontalSpeed_triggered();
    void on_actionTotalSpeed_triggered();
    void on_actionDiveAngle_triggered();
    void on_actionCurvature_triggered();
    void on_actionGlideRatio_triggered();
    void on_actionHorizontalAccuracy_triggered();
    void on_actionVerticalAccuracy_triggered();
    void on_actionSpeedAccuracy_triggered();
    void on_actionNumberOfSatellites_triggered();
    void on_actionAcceleration_triggered();
    void on_actionAccForward_triggered();
    void on_actionAccRight_triggered();
    void on_actionAccDown_triggered();
    void on_actionAccMagnitude_triggered();
    void on_actionTotalEnergy_triggered();
    void on_actionEnergyRate_triggered();
    void on_actionLift_triggered();
    void on_actionDrag_triggered();
    void on_actionCourse_triggered();
    void on_actionCourseRate_triggered();
    void on_actionCourseAccuracy_triggered();

    void on_actionPan_triggered();
    void on_actionZoom_triggered();
    void on_actionMeasure_triggered();
    void on_actionZero_triggered();
    void on_actionGround_triggered();
    void on_actionSetCourse_triggered();
    void on_actionWind_triggered();

    void on_actionTime_triggered();
    void on_actionDistance2D_triggered();
    void on_actionDistance3D_triggered();

    void on_actionImportGates_triggered();
    void on_actionPreferences_triggered();

    void on_actionImportVideo_triggered();
    void on_actionExportKML_triggered();
    void on_actionExportPlot_triggered();
    void on_actionExportTrack_triggered();
    void on_actionExportToGoogleEarth_triggered();

    void on_actionUndoZoom_triggered();
    void on_actionRedoZoom_triggered();
    void on_actionZoomToExtent_triggered();

    void on_actionDeleteTrack_triggered();

    void on_actionChangeUnits_triggered();

private:
    typedef struct {
        flysight::DataPoint::Time rangeLower;
        flysight::DataPoint::Time rangeUpper;
    } ZoomLevel;

    Ui::MainWindow* m_ui;

    std::optional<flysight::Track> m_track;

    double mMarkStart;
    double mMarkEnd;
    bool mMarkActive;

    double mMediaCursor;
    QSet<QObject*> mMediaCursorRef;

    double m_viewDataRotation;

    PlotValue::Units m_units;

    DataPoints m_waypoints;

    Tool mTool;
    Tool mPrevTool;

    ZoomLevel mZoomLevel;
    ZoomLevel mZoomLevelPrev;
    QStack<ZoomLevel> mZoomLevelUndo;
    QStack<ZoomLevel> mZoomLevelRedo;

    // TODO(akenny): Can these be removed?
    // double mRangeLower;
    // double mRangeUpper;

    WindowMode mWindowMode;

    ScoringView* mScoringView;

    QVector<ScoringMethod*> mScoringMethods;
    ScoringMode mScoringMode;

    double m_mass;
    double m_planformArea;

    double m_minDrag;
    double m_minLift;
    double m_maxLift;
    double m_maxLD;

    int m_simulationTime;

    double mLineThickness;

    double mWindE, mWindN;
    bool mWindAdjustment;

    GroundReference mGroundReference;
    double mFixedReference;

    bool mUseDatabase;

    QString mDatabasePath;
    QSqlDatabase mDatabase;

    QString mTrackName;
    QVector<QString> mSelectedTracks;
    QMap<QString, DataPoints> mCheckedTracks;

    QTimer* zoomTimer;

    MapMode mMapMode;

    void writeSettings();
    void readSettings();

    void initDatabase();
    bool setDatabaseValue(QString trackName, QString column, QString value);
    bool getDatabaseValue(QString trackName, QString column, QString& value);
    void saveZoomToDatabase();

    void initPlot();
    void initViews();
    void initMapView();
    void initWindView();
    void initScoringView();
    void initLiftDragView();
    void initOrthoView();
    void initPlaybackView();
    void initLogbookView();
    void initSimulationView();

    void initSingleView(const QString& title, const QString& objectName, QAction* actionShow,
                        DataView::Direction direction);

    void import(QIODevice* device, DataPoints& data, QString trackName, bool initDatabase);

    void initRange(QString trackName);

    void updateBottomActions();
    void updateLeftActions();

    void updateGround(DataPoints& data, double ground);
    QString dateTimeToUTC(const QDateTime& dt);

    bool exportToKML(QIODevice* device, QString name);

signals:
    void dataLoaded();
    void dataChanged();
    void rangeChanged();
    void cursorChanged();
    void mediaCursorChanged();
    void mediaPaused();
    void aeroChanged();
    void rotationChanged(double rotation);
    void databaseChanged();
    void mapModeChanged();

public slots:
    void importFolder(QString folderName);
    void importFile(QString fileName);

private slots:
    void setScoringVisible(bool visible);
    void saveZoom();
    void onDockWidgetTopLevelChanged(bool floating);
};

#endif // MAINWINDOW_H
