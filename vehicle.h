#ifndef VEHICLE_H
#define VEHICLE_H
#include <QString>
#include <QDebug>

class Vehicle
{


public:


    QString id;
    float x;
    float y;
    float angle;
    QString vtype;
    float speed;
    float pos;
    QString lane;
    float slope;


    void setValues(QString id_, float x_, float y_, float angle_, QString vtype_, float speed_, float pos_, QString lane_, float slope_);
    void printout();

};



#endif // VEHICLE_H
