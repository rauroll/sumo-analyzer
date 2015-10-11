#ifndef RESULTS_H
#define RESULTS_H

#include <QObject>
//#include <QFile>
//#include "mainwindow.h"
//#include <QDebug>
#include <QTextStream>
#include <analyzer.h>
#include <pedestriangenerator.h>

class Results : public QObject
{
    Q_OBJECT
public:

    QString *outputFileName;
    QList<Pedestrian> peds;

    QList < QList<QString> > persons;



    explicit Results(QObject *parent = 0);
    void setOutputFileName(QString &fileName, Analyzer &analyzer);




signals:
    void writingComplete();

public slots:
    void writeToCSV();
    void fixedWriteToCSV();


};

#endif // RESULTS_H
