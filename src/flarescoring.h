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

#ifndef FLARESCORING_H
#define FLARESCORING_H

#include "scoringmethod.h"

class MainWindow;

class FlareScoring : public ScoringMethod {
public:
    FlareScoring(MainWindow* mainWindow);

    double windowBottom(void) const { return mWindowBottom; }
    void setWindowBottom(double windowBottom);

    double score(const MainWindow::DataPoints& result);
    QString scoreAsText(double score);

    void prepareDataPlot(DataPlot* plot);

    bool getWindowBounds(const MainWindow::DataPoints& result, DataPoint& dpBottom,
                         DataPoint& dpTop);

    void optimize() { ScoringMethod::optimize(mMainWindow, mWindowBottom); }

private:
    MainWindow* mMainWindow;

    double mWindowBottom;

signals:

public slots:
};

#endif // FLARESCORING_H
