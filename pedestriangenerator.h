#ifndef PEDESTRIANGENERATOR_H
#define PEDESTRIANGENERATOR_H

#include <QObject>
#include "pedestrian.h"



class PedestrianGenerator : public QObject
{
    Q_OBJECT


private:

    float areaLat;
    float areaLon;
    float areaLat2;
    float areaLon2;
    float areaWidth;
    float areaHeight;



    int pedestrianDistance;


    void generatePedestrians();



public:

    explicit PedestrianGenerator(QObject *parent = 0);

    QList<Pedestrian> *fixedPeds;


    void setAreaValues(float lat, float lon, float lat2, float lon2);
    void setup(int &dist, QList<Pedestrian> &pedestrians);

signals:
    void generated();

public slots:
    void generate();



};

#endif // PEDESTRIANGENERATOR_H
