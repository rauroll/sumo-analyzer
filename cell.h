#ifndef CELL_H
#define CELL_H

#include "vehicle.h"
#include <QList>


class Cell
{
public:
    Cell(float x_, float y_);
    float x;
    float y;
    QList<Vehicle> vehicles;
};

#endif // CELL_H
