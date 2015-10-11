#include "mymodel.h"

MyModel::MyModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

void MyModel::my_append_data(QString &person_ID, float &lat, float &lon, float &Uptime_Percentage) {
    int newRow = data_.count();
    QList<QString> row;
    row << person_ID << QString::number(lat) << QString::number(lon) << QString::number(Uptime_Percentage);
    beginInsertRows(QModelIndex(), newRow, newRow);
    data_.append(row);
    endInsertRows();
}
