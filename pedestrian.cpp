#include "pedestrian.h"

Pedestrian::Pedestrian()
{
    Pedestrian::cell = QPair<int, int>(-2, -2);
}

void Pedestrian::setValues(QString id_, float lat_, float lon_)
{
    Pedestrian::id = id_;
    Pedestrian::lat = lat_;
    Pedestrian::lon = lon_;
}

