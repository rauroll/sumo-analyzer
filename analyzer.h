#ifndef ANALYZER_H
#define ANALYZER_H
#include <QtXml>
#include <QFile>
#include <QXmlStreamReader>
#include <QObject>
#include <QThread>
#include "mainwindow.h"

#include "timestep.h"
#include "cell.h"
#include <QPair>









class Analyzer  : public QObject
{

    Q_OBJECT

public:


    QString *fn;


    static float areaLat;
    static float areaLon;
    static float areaLat2;
    static float areaLon2;
    static float areaWidth;
    static float areaHeight;
    static float latStep;
    static float lonStep;
    static float range;

    static int stepCount;

    static QList< QList<Cell> > grid;




    static QList<Timestep> timesteps;
    static QList<Pedestrian> *pedestrians;

    static void addTimestep(Timestep step);
    static void loadFileStream(QString fileName);

    explicit Analyzer(QObject *parent = 0);

    static Vehicle parseVehicle(QXmlStreamReader &xml);
    static Pedestrian parsePerson(QXmlStreamReader &xml);



    void setFileName(QString &fileName, QList<Pedestrian> *peds, float &r);
    static void createGrid(QList< QList<Cell> > &grid, float w, float h);
    void setAreaValues(float lat, float lon, float lat2, float lon2);
    static QPair<int, int> getCellFor(float x, float y);
    static QList< QPair<int, int> > getNeighbours(QPair<int, int> pedCell, int w, int h);
    //void setAreaValues(float lat, float lon, float width, float height);



signals:
    void parsingComplete();



public slots:
    void DoWork();




};




#endif // ANALYZER_H
