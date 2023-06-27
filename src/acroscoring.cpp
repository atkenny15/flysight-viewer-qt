/***************************************************************************
**                                                                        **
**  FlySight Viewer                                                       **
**  Copyright 2018 Michael Cooper, Matt Coffin                            **
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

#include "acroscoring.h"

#include "mainwindow.h"

using namespace units::isq::si::references;

AcroScoring::AcroScoring(MainWindow* mainWindow) :
    ScoringMethod(mainWindow), mMainWindow(mainWindow), mSpeed(8 * (m / s)), mAltitude(2286 * m) {
}

void AcroScoring::setSpeed(flysight::DataPoint::Speed speed) {
    mSpeed = speed;
    emit scoringChanged();
}

void AcroScoring::setAltitude(flysight::DataPoint::Length altitude) {
    mAltitude = altitude;
    emit scoringChanged();
}

void AcroScoring::prepareDataPlot(DataPlot* plot) {
    if (mMainWindow->track_is_empty()) {
        return;
    }
    const auto& track = *mMainWindow->get_track();

    flysight::DataPoint dpBottom, dpTop;
    bool success = false;

    switch (mMainWindow->windowMode()) {
        case MainWindow::Actual:
        case MainWindow::Optimal:
            success = getWindowBounds(track, dpBottom, dpTop);
            break;
    }

    // Add shading for scoring window
    if (success && plot->yValue(DataPlot::Elevation)->visible()) {
        const auto dpLower = track.interpolate_t(mMainWindow->rangeLower());
        const auto dpUpper = track.interpolate_t(mMainWindow->rangeUpper());

        const double xMin = plot->xValue()->value(dpLower, mMainWindow->units());
        const double xMax = plot->xValue()->value(dpUpper, mMainWindow->units());

        QVector<double> xElev, yElev;

        xElev << xMin << xMax;
        yElev << plot->yValue(DataPlot::Elevation)->value(dpBottom, mMainWindow->units())
              << plot->yValue(DataPlot::Elevation)->value(dpBottom, mMainWindow->units());

        QCPGraph* graph = plot->addGraph(plot->axisRect()->axis(QCPAxis::atBottom),
                                         plot->yValue(DataPlot::Elevation)->axis());
        graph->setData(xElev, yElev);
        graph->setPen(QPen(QBrush(Qt::lightGray), mMainWindow->lineThickness(), Qt::DashLine));

        yElev.clear();
        yElev << plot->yValue(DataPlot::Elevation)->value(dpTop, mMainWindow->units())
              << plot->yValue(DataPlot::Elevation)->value(dpTop, mMainWindow->units());

        graph = plot->addGraph(plot->axisRect()->axis(QCPAxis::atBottom),
                               plot->yValue(DataPlot::Elevation)->axis());
        graph->setData(xElev, yElev);
        graph->setPen(QPen(QBrush(Qt::lightGray), mMainWindow->lineThickness(), Qt::DashLine));

        QCPItemRect* rect = new QCPItemRect(plot);

        rect->setPen(QPen(QBrush(Qt::lightGray), mMainWindow->lineThickness(), Qt::DashLine));
        rect->setBrush(QColor(0, 0, 0, 8));

        rect->topLeft->setType(QCPItemPosition::ptAxisRectRatio);
        rect->topLeft->setAxes(plot->xAxis, plot->yValue(DataPlot::Elevation)->axis());
        rect->topLeft->setCoords(-0.1, -0.1);

        rect->bottomRight->setType(QCPItemPosition::ptAxisRectRatio);
        rect->bottomRight->setAxes(plot->xAxis, plot->yValue(DataPlot::Elevation)->axis());
        rect->bottomRight->setCoords(
            (plot->xValue()->value(dpTop, mMainWindow->units()) - xMin) / (xMax - xMin), 1.1);

        rect = new QCPItemRect(plot);

        rect->setPen(QPen(QBrush(Qt::lightGray), mMainWindow->lineThickness(), Qt::DashLine));
        rect->setBrush(QColor(0, 0, 0, 8));

        rect->topLeft->setType(QCPItemPosition::ptAxisRectRatio);
        rect->topLeft->setAxes(plot->xAxis, plot->yValue(DataPlot::Elevation)->axis());
        rect->topLeft->setCoords(
            (plot->xValue()->value(dpBottom, mMainWindow->units()) - xMin) / (xMax - xMin), -0.1);

        rect->bottomRight->setType(QCPItemPosition::ptAxisRectRatio);
        rect->bottomRight->setAxes(plot->xAxis, plot->yValue(DataPlot::Elevation)->axis());
        rect->bottomRight->setCoords(1.1, 1.1);
    }
}

bool AcroScoring::getWindowBounds(const flysight::Track& track, flysight::DataPoint& dpBottom,
                                  flysight::DataPoint& dpTop) {
    const size_t iExit = track.find_index_below_t(0 * s);
    const auto& data = track.data();

    size_t iStart;
    for (iStart = iExit; iStart < data.size(); ++iStart) {
        const auto& dp = data[iStart];
        if (dp.velocity_down > mSpeed)
            break;
    }

    if (iStart == data.size())
        return false;

    const auto& dp1 = data[iStart - 1];
    const auto& dp2 = data[iStart];
    dpTop = dp1.interpolate(dp2,
                            (mSpeed - dp1.velocity_down) / (dp2.velocity_down - dp1.velocity_down));

    if ((dpTop.t < dp1.t) || (dpTop.t > dp2.t))
        return false;

    size_t iEnd;
    for (iEnd = iStart; iEnd < data.size(); ++iEnd) {
        const auto& dp = data[iEnd];
        if (dp.height_msl < dpTop.height_msl - mAltitude)
            break;
    }

    if (iEnd == data.size())
        return false;

    const auto& dp3 = data[iEnd - 1];
    const auto& dp4 = data[iEnd];
    dpBottom = dp3.interpolate(dp4, (dpTop.height_msl - mAltitude - dp3.height_msl) /
                                        (dp4.height_msl - dp3.height_msl));

    if ((dpBottom.t < dp3.t) || (dpBottom.t > dp4.t))
        return false;

    return true;
}

int AcroScoring::findIndexBelowT(const MainWindow::DataPoints& result, double t) {
    int below = -1;
    int above = result.size();

    while (below + 1 != above) {
        int mid = (below + above) / 2;
        const DataPoint& dp = result[mid];

        if (dp.t < t)
            below = mid;
        else
            above = mid;
    }

    return below;
}
