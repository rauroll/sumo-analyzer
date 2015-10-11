#ifndef MYMODEL_H
#define MYMODEL_H

#include <QAbstractTableModel>
#include <results.h>

class MyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MyModel(QObject *parent = 0);

    QList< QList<QString> > data_;

    void my_append_data(QString &person_ID, float &lat, float &lon, float &Uptime_Percentage);

signals:

public slots:

};

#endif // MYMODEL_H
