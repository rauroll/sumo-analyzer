#include "pedestriangenerator.h"
#include <mymath.h>

PedestrianGenerator::PedestrianGenerator(QObject *parent) :
    QObject(parent)
{
}


void PedestrianGenerator::setAreaValues(float lat, float lon, float lat2, float lon2)
{
    areaLat = lat;
    areaLon = lon;
    areaLat2 = lat2;
    areaLon2 = lon2;
    MyMath::haversine(lat, lon, lat, lon2);
    MyMath::haversine(lat, lon, lat2, lon);
    Analyzer::areaWidth = MyMath::haversine(lat, lon, lat, lon2);
    Analyzer::areaHeight = MyMath::haversine(lat, lon, lat2, lon);
    qDebug() << "Area width:" << Analyzer::areaWidth;
    qDebug() << "Area height:" << Analyzer::areaHeight;
}

void PedestrianGenerator::setup(int &dist, QList<Pedestrian> &pedestrians)
{
    pedestrianDistance = dist;
    this->fixedPeds = &pedestrians;

}

void PedestrianGenerator::generate()
{
    int x = Analyzer::areaWidth * 1000 / pedestrianDistance;
    int y = Analyzer::areaHeight * 1000 / pedestrianDistance;
    float latstep = (areaLat2 - areaLat) / y;
    float lonstep = (areaLon2 - areaLon) / x;


    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            Pedestrian ped;
            QString id = "Pedx" + QString::number(i) + "y" + QString::number(j);

            ped.setValues(id, areaLat + j * latstep, areaLon + i * lonstep);
            fixedPeds->append(ped);
        }
    }

    qDebug() << fixedPeds->length();





    emit generated();
}
