#ifndef TIMESTEP_H
#define TIMESTEP_H
#include <QList>
#include "pedestrian.h"
#include "cell.h"

class Timestep
{


public:
    float time;



    QList< QList<Cell> > grid;
    QList<Vehicle> vehicles;
    QList<Pedestrian> pedestrians;
    void addVehicle(Vehicle veh);
    void addPedestrian(Pedestrian ped);
};

#endif // TIMESTEP_H
