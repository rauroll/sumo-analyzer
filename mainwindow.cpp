
#include <QtCore/QTextStream>
#include <QFileDialog>
#include <QDateTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "analyzer.h"
#include "results.h"
#include "mymath.h"
#include <QSlider>
#include <areawindow.h>
#include <pedestrianwindow.h>
#include <pedestriangenerator.h>
#include <QTableView>
#include "resultswindow.h"
#include <QTableWidget>
#include <QtSql>
#include "mymodel.h"



using namespace std;

Analyzer analyzer;
Results results;
clock_t start;
PedestrianGenerator gen;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->timeLineEdit->setReadOnly(true);
    ui->pedLineEdit->setReadOnly(true);
    ui->areaBox->setEnabled(false);
    ui->vehicleBox->setEnabled(false);
    ui->pedestrianBox->setEnabled(false);
    ui->calcButton->setDisabled(true);


    ui->rangeSlider->setRange(10, 1000);
    ui->rangeSpinBox->setRange(10, 1000);

    QObject::connect(ui->areaBox, SIGNAL(stateChanged(int)), this, SLOT(checkIfReadyToCalculate()));

    QObject::connect(ui->rangeSlider, SIGNAL(valueChanged(int)), ui->rangeSpinBox, SLOT(setValue(int)));
    QObject::connect(ui->rangeSpinBox, SIGNAL(valueChanged(int)), ui->rangeSlider, SLOT(setValue(int)));
    QObject::connect(&analyzer, SIGNAL(parsingComplete()), this, SLOT(handleParsingResults()));
    QObject::connect(&results, SIGNAL(writingComplete()), this, SLOT(finishWriting()));
    QObject::connect(&gen, SIGNAL(generated()), this, SLOT(finishGenerating()));
    analyzer.connect(&thread3, SIGNAL(started()), &gen, SLOT(generate()));
    analyzer.connect(&thread, SIGNAL(started()), &analyzer, SLOT(DoWork()));
    analyzer.connect(&thread2, SIGNAL(started()), &results, SLOT(fixedWriteToCSV()));




    analyzer.moveToThread(&thread);
    results.moveToThread(&thread2);



    ui->rangeSlider->setValue(200);






}

void MainWindow::checkIfReadyToCalculate()
{
    if (ui->areaBox->isChecked() && ui->pedestrianBox->isChecked() && ui->vehicleBox->isChecked())
    {
        ui->calcButton->setEnabled(true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::finishWriting()
{
    thread2.quit();

    qDebug() << "Writing to file complete!";
}

void MainWindow::handleParsingResults()
{

    thread.quit();

    qDebug() << "Adding parsed data to GUI";
    int vehs = 0;
    int peds = 0;
    //MyMath::numberOfVehsAndPeds(&analyzer.timesteps, vehs, peds);

    MainWindow::updateSimInfo(analyzer.stepCount, results.peds.length());
    ui->calcButton->setEnabled(true);
    //MainWindow::on_calcButton_clicked();
    double timeTaken = (clock() - start) / (double)CLOCKS_PER_SEC;
    qDebug() << "Done.\nTime taken:" << timeTaken << endl;

}



void MainWindow::updateSimInfo(int timesteps, int pedestrians)
{
    QString t = QString::number(timesteps);
    QString p = QString::number(pedestrians);
    ui->timeLineEdit->setText(t);
    ui->pedLineEdit->setText(p);

}



void MainWindow::on_vehButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.*)"));
    ui->fileNameLineEdit->setText(fileName);


}






void MainWindow::on_parseOutputButton_clicked()
{


    if (!fileName.isEmpty())
    {


        clearAll();

        start = clock();

        analyzer.setFileName(fileName, &results.peds, range);

        thread.start();
        ui->calcButton->setEnabled(false);


    }
    else
    {
        qDebug() << "No file selected yet." << endl;
    }

}





void MainWindow::clearAll()
{
    analyzer.timesteps.clear();
    results.persons.clear();
    ui->timeLineEdit->setText("");
    ui->pedLineEdit->setText("");

}




void MainWindow::on_rangeSlider_valueChanged(int value)
{
    range = value / 1000.0;

}

void MainWindow::on_clearButton_clicked()
{
    MainWindow::clearAll();

}


void MainWindow::on_outputButton_clicked()
{
    outputFileName = QFileDialog::getSaveFileName(this, "Export results as CSV", "analyzer_out_" + QDateTime::currentDateTime().toString("dd.MM.yy-hh.mm.ss") + ".csv", "CSV files (*.csv);;Zip files (*.zip, *.7z)", 0, 0);
    if (outputFileName == "")
    {
        return;
    }
    if (results.peds.isEmpty())
    {
        qDebug() << "No data to export found.";
        return;
    }
    results.setOutputFileName(outputFileName, analyzer);
    thread2.start();

}



void MainWindow::on_areaSettingsButton_clicked()
{
    AreaWindow areaWindow;
    areaWindow.setModal(true);

    if (areaWindow.exec() == QDialog::Accepted)
    {
        QString slat = areaWindow.lat;
        QString slon = areaWindow.lon;
        QString slat2 = areaWindow.lat2;
        QString slon2 = areaWindow.lon2;

        if (slat.isEmpty() || slon.isEmpty() || slat2.isEmpty() || slon2.isEmpty())
        {
            qDebug() << "You must enter all values." << endl;
            gen.setAreaValues(50.818525, 6.769511, 51.115404, 7.216403);
            analyzer.setAreaValues(50.818525, 6.769511, 51.115404, 7.216403);
        }
        else
        {


        gen.setAreaValues(slat.toFloat(), slon.toFloat(), slat2.toFloat(), slon2.toFloat());
        analyzer.setAreaValues(slat.toFloat(), slon.toFloat(), slat2.toFloat(), slon2.toFloat());
        }

    }
    else
    {
        qDebug() << "Canceled" << endl;
    }


}

void MainWindow::finishGenerating()
{
    thread3.quit();

    qDebug() << "Finished generating pedestrians" << endl;
}

void MainWindow::on_pedGenButton_clicked()
{
    PedestrianWindow pedWindow;
    pedWindow.setModal(true);

    if (pedWindow.exec() == QDialog::Accepted)
    {
        gen.setup(pedWindow.distance, results.peds);
        qDebug() << "Starting thread 3";
        thread3.start();
    }
}

void MainWindow::on_resultsButton_clicked()
{
    MainWindow::updateSimInfo(analyzer.stepCount, results.peds.length());

}
