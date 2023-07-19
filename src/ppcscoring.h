/***************************************************************************
**                                                                        **
**  FlySight Viewer                                                       **
**  Copyright 2018 Michael Cooper                                         **
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

#ifndef PPCSCORING_H
#define PPCSCORING_H

#include "scoringmethod.h"

class MainWindow;

class PPCScoring : public ScoringMethod
{
public:
    typedef enum {
        Time, Distance, Speed
    } Mode;

    PPCScoring(MainWindow *mainWindow);

    Mode mode() const { return mMode; }
    void setMode(Mode mode);

    double windowTop(void) const { return mWindowTop; }
    double windowBottom(void) const { return mWindowBottom; }
    void setWindow(double windowBottom, double windowTop);

    bool drawLane(void) const { return mDrawLane; }
    void setDrawLane(bool drawLane);

    double endLatitude(void) const { return mEndLatitude; }
    double endLongitude(void) const { return mEndLongitude; }
    void setEnd(double endLatitude, double endLongitude);

    double score(const MainWindow::DataPoints &result);
    QString scoreAsText(double score);

    void prepareDataPlot(DataPlot *plot);
    void prepareMapView(MapView *view);

    bool updateReference(double lat, double lon);

    bool getWindowBounds(const MainWindow::DataPoints &result,
                         DataPoint &dpBottom, DataPoint &dpTop);
    bool getSEP(const MainWindow::DataPoints &result,
                double &sep);

    void readSettings();
    void writeSettings();

    void optimize() { ScoringMethod::optimize(mMainWindow, mWindowBottom); }

private:
    MainWindow *mMainWindow;

    Mode        mMode;
    double      mWindowTop;
    double      mWindowBottom;

    bool        mDrawLane;
    double      mEndLatitude;
    double      mEndLongitude;
    double      mLaneWidth;

    void splitLine(QList<QVariant> &data,
                   double startLat, double startLon,
                   double endLat, double endLon,
                   double threshold, int depth);

signals:

public slots:
};

#endif // PPCSCORING_H
