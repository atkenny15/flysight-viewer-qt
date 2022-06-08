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

#include "speedform.h"
#include "ui_speedform.h"

#include "common.h"
#include "datapoint.h"
#include "mainwindow.h"
#include "plotvalue.h"
#include "speedscoring.h"
#include "ppcupload.h"

#define METERS_TO_FEET 3.280839895

SpeedForm::SpeedForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpeedForm),
    mMainWindow(0)
{
    ui->setupUi(this);

    connect(ui->faiButton, SIGNAL(clicked()), this, SLOT(onFAIButtonClicked()));

    connect(ui->fromExitEdit, SIGNAL(editingFinished()), this, SLOT(onApplyButtonClicked()));
    connect(ui->bottomEdit, SIGNAL(editingFinished()), this, SLOT(onApplyButtonClicked()));
    connect(ui->validationEdit, SIGNAL(editingFinished()), this, SLOT(onApplyButtonClicked()));

    // Connect optimization buttons
    connect(ui->actualButton, SIGNAL(clicked()), this, SLOT(onActualButtonClicked()));
    connect(ui->optimalButton, SIGNAL(clicked()), this, SLOT(onOptimalButtonClicked()));
    connect(ui->optimizeButton, SIGNAL(clicked()), this, SLOT(onOptimizeButtonClicked()));

    // Connect PPC button
    connect(ui->ppcButton, SIGNAL(clicked()), this, SLOT(onPpcButtonClicked()));
}

SpeedForm::~SpeedForm()
{
    delete ui;
}

QSize SpeedForm::sizeHint() const
{
    // Keeps windows from being initialized as very short
    return QSize(175, 175);
}

void SpeedForm::setMainWindow(
        MainWindow *mainWindow)
{
    mMainWindow = mainWindow;
}

void SpeedForm::updateView()
{
    if (mMainWindow->dataSize() == 0) return;

    // Update mode selection
    ui->actualButton->setChecked(mMainWindow->windowMode() == MainWindow::Actual);
    ui->optimalButton->setChecked(mMainWindow->windowMode() == MainWindow::Optimal);

    SpeedScoring *method = (SpeedScoring *) mMainWindow->scoringMethod(MainWindow::Speed);

    const double fromExit = method->fromExit();
    const double bottom = method->windowBottom();
    const double validationWindow = method->validationWindow();

    // Update window bounds
    ui->fromExitEdit->setText(QString("%1").arg(fromExit * METERS_TO_FEET));
    ui->bottomEdit->setText(QString("%1").arg(bottom * METERS_TO_FEET));
    ui->validationEdit->setText(QString("%1").arg(validationWindow * METERS_TO_FEET));

    // Get exit
    DataPoint dpExit = mMainWindow->interpolateDataT(0);

    DataPoint dpBottom, dpTop;
    bool success;

    double scoreAccuracy;
    bool accuracyOkay = false;

    switch (mMainWindow->windowMode())
    {
    case MainWindow::Actual:
        success = method->getWindowBounds(mMainWindow->data(), dpBottom, dpTop, dpExit);
        accuracyOkay = method->getAccuracy(mMainWindow->data(), scoreAccuracy, dpExit);
        break;
    case MainWindow::Optimal:
        success = method->getWindowBounds(mMainWindow->optimal(), dpBottom, dpTop, dpExit);
        break;
    }

    if (success)
    {
        // Calculate results
        const double time = dpBottom.t - dpTop.t;
        const double verticalSpeed = (dpTop.z - dpBottom.z) / time;

        // Update display
        if (mMainWindow->units() == PlotValue::Metric)
        {
            ui->verticalSpeedEdit->setText(QString("%1").arg(verticalSpeed * MPS_TO_KMH, 0, 'f', 2));
            ui->verticalSpeedUnits->setText(tr("km/h"));
        }
        else
        {
            ui->verticalSpeedEdit->setText(QString("%1").arg(verticalSpeed * MPS_TO_MPH, 0, 'f', 2));
            ui->verticalSpeedUnits->setText(tr("mph"));
        }

        const double performanceBottom = qMax(dpExit.z - fromExit, bottom);
        const double validationTop = performanceBottom + validationWindow;

        ui->exitAltitudeEdit->setText(QString("%1").arg(dpExit.z, 0, 'f', 3));
        ui->validationTopEdit->setText(QString("%1").arg(validationTop, 0, 'f', 3));
        ui->performanceBottomEdit->setText(QString("%1").arg(performanceBottom, 0, 'f', 3));
        ui->scoringTopEdit->setText(QString("%1").arg(dpTop.z, 0, 'f', 3));
        ui->scoringBottomEdit->setText(QString("%1").arg(dpBottom.z, 0, 'f', 3));

        ui->actualButton->setEnabled(true);
        ui->optimizeButton->setEnabled(true);
        ui->ppcButton->setEnabled(true);
    }
    else
    {
        // Update display
        if (mMainWindow->units() == PlotValue::Metric)
        {
            ui->verticalSpeedUnits->setText(tr("km/h"));
        }
        else
        {
            ui->verticalSpeedUnits->setText(tr("mph"));
        }

        ui->verticalSpeedEdit->setText(tr("n/a"));

        ui->exitAltitudeEdit->setText(tr("n/a"));
        ui->validationTopEdit->setText(tr("n/a"));
        ui->performanceBottomEdit->setText(tr("n/a"));
        ui->scoringTopEdit->setText(tr("n/a"));
        ui->scoringBottomEdit->setText(tr("n/a"));

        ui->actualButton->setEnabled(false);
        ui->optimalButton->setEnabled(false);
        ui->optimizeButton->setEnabled(false);
        ui->ppcButton->setEnabled(false);
    }

    if (accuracyOkay)
    {
        ui->scoreAccuracyEdit->setText(QString("%1").arg(scoreAccuracy, 0, 'f', 3));
    }
    else
    {
        ui->scoreAccuracyEdit->setText(tr("n/a"));
    }
}

void SpeedForm::onFAIButtonClicked()
{
    SpeedScoring *method = (SpeedScoring *) mMainWindow->scoringMethod(MainWindow::Speed);

    method->setFromExit(2255.52);
    method->setWindowBottom(1706.88);
    method->setValidationWindow(1005.84);
}

void SpeedForm::onApplyButtonClicked()
{
    double fromExit = ui->fromExitEdit->text().toDouble() / METERS_TO_FEET;
    double bottom = ui->bottomEdit->text().toDouble() / METERS_TO_FEET;
    double validationWindow = ui->validationEdit->text().toDouble() / METERS_TO_FEET;

    SpeedScoring *method = (SpeedScoring *) mMainWindow->scoringMethod(MainWindow::Speed);

    method->setFromExit(fromExit);
    method->setWindowBottom(bottom);
    method->setValidationWindow(validationWindow);

    mMainWindow->setFocus();
}

void SpeedForm::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        // Reset window bounds
        updateView();

        // Release focus
        mMainWindow->setFocus();
    }

    QWidget::keyPressEvent(event);
}

void SpeedForm::onActualButtonClicked()
{
    mMainWindow->setWindowMode(MainWindow::Actual);
}

void SpeedForm::onOptimalButtonClicked()
{
    mMainWindow->setWindowMode(MainWindow::Optimal);
}

void SpeedForm::onOptimizeButtonClicked()
{
    SpeedScoring *method = (SpeedScoring *) mMainWindow->scoringMethod(MainWindow::Speed);

    // Perform optimization
    method->optimize();

    ui->optimalButton->setEnabled(true);

    // Switch to optimal view
    mMainWindow->setWindowMode(MainWindow::Optimal);
}

void SpeedForm::onPpcButtonClicked() {

    // Return if plot empty
    if (mMainWindow->dataSize() == 0) return;

    PPCUpload *uploader = new PPCUpload(mMainWindow);
    SpeedScoring *method = (SpeedScoring *) mMainWindow->scoringMethod(MainWindow::Speed);
    DataPoint dpBottom, dpTop;

    ui->faiButton->click();
    ui->actualButton->click();

    // Get exit
    DataPoint dpExit = mMainWindow->interpolateDataT(0);

    if (method->getWindowBounds(mMainWindow->data(), dpBottom, dpTop, dpExit)) {

        const double time = dpBottom.t - dpTop.t;
        const double distance = mMainWindow->getDistance(dpTop, dpBottom);
        const double windowTop = dpTop.z;
        const double windowBottom = dpBottom.z;

        uploader->upload("SS", windowTop, windowBottom, time, distance);
    }
}
