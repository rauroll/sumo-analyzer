#include "timestep.h"

void Timestep::addVehicle(Vehicle veh)
{
    this->vehicles.append(veh);
}

void Timestep::addPedestrian(Pedestrian ped)
{
    this->pedestrians.append(ped);
}
