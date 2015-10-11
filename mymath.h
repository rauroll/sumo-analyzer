#ifndef MYMATH_H
#define MYMATH_H
#include <cmath>
#include "timestep.h"
#include "results.h"
#include <QMultiMap>
#include <algorithm>
#include <QList>

class MyMath
{
public:
    MyMath();
    static float haversine(float lat1, float lon1, float lat2, float lon2);
    static float equirec(float lat1, float lon1, float lat2, float lon2);
    static float toRadians(float deg);
    static bool isInRange(float lat1, float lon1, float lat2, float lon2, float range);
    static void uptimeOneStep(QList<Pedestrian> &pedestrians, Timestep &timestep, float range);
    static void uptime(QList<Pedestrian> &pedestrians, QList<Timestep> *timesteps, float range);
    static void calcUptime(QList< QList<QString> > *persons, QList<Timestep> *timesteps, float range);
    static void calculateDynamically(QList<Pedestrian> &pedestrians, Timestep &timestep, float range);
    static void numberOfVehsAndPeds(QList<Timestep> *timesteps, int &vehs, int &peds);
    static float mean(QList<float> *list);
    static float median(QList<float> *list);
    static void test();
    static void uptimeWithGrid(QList<Pedestrian> &pedestrians, QList<Timestep> &timesteps, float range);
};

#endif // MYMATH_H
