#ifndef PEDESTRIAN_H
#define PEDESTRIAN_H
#include <QString>
#include <QList>
#include <QPair>

class Pedestrian
{


public:
    QString id;
    float lat;
    float lon;
    float activityValue;
    QList<bool> wasInRange;
    QPair<int,int> cell;

    void setValues(QString id_, float lat_, float lon_);
    Pedestrian();
};

#endif // PEDESTRIAN_H
