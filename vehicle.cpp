#include "vehicle.h"

void Vehicle::setValues(QString id_, float x_, float y_, float angle_, QString vtype_, float speed_, float pos_, QString lane_, float slope_)
{
    Vehicle::id = id_;
    Vehicle::x = x_;
    Vehicle::y = y_;
    Vehicle::angle = angle_;
    Vehicle::vtype = vtype_;
    Vehicle::speed = speed_;
    Vehicle::pos = pos_;
    Vehicle::lane = lane_;
    Vehicle::slope = slope_;
}

void Vehicle::printout()
{
    qDebug() << "Vehicle id " << Vehicle::id << ", is at " << Vehicle::x << ", " << Vehicle::y << "." << endl;
}
