#include "LogbookView.h"
#include "ui_LogbookView.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>

#include "mainwindow.h"

LogbookView::LogbookView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogbookView),
    mMainWindow(0)
{
    ui->setupUi(this);
    updateView();
}

LogbookView::~LogbookView()
{
    delete ui;
}

void LogbookView::setMainWindow(
        MainWindow *mainWindow)
{
    mMainWindow = mainWindow;
}

void LogbookView::updateView()
{
    QSqlDatabase db = QSqlDatabase::database("flysight");
    QSqlQuery query(db);

    if (!query.exec("select * from files"))
    {
        QSqlError err = query.lastError();
        QMessageBox::critical(0, tr("Query failed"), err.text());
    }

    ui->tableWidget->setColumnCount(query.record().count());
    ui->tableWidget->setRowCount(0);

    for (int j = 0; j < query.record().count(); ++j)
    {
        ui->tableWidget->setHorizontalHeaderItem(j, new QTableWidgetItem(query.record().field(j).name()));
    }

    int index = 0;
    while (query.next())
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        for (int j = 0; j < query.record().count(); ++j)
        {
            ui->tableWidget->setItem(index, j, new QTableWidgetItem(query.value(j).toString()));
        }
        ++index;
    }
}
