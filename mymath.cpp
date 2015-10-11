#include "mymath.h"





float MyMath::toRadians(float deg)
{
    return deg * M_PI / 180;
}


float MyMath::haversine(float lat1, float long1, float lat2, float long2)
{
    float earthRad = 6368.5; //as km, estimated average radius of Earth
    double dlat1=lat1*(M_PI/180);

    double dlong1=long1*(M_PI/180);
    double dlat2=lat2*(M_PI/180);
    double dlong2=long2*(M_PI/180);

    double dLong=dlong1-dlong2;
    double dLat=dlat1-dlat2;

    double aHarv= pow(sin(dLat/2.0),2.0)+cos(dlat1)*cos(dlat2)*pow(sin(dLong/2),2);
    double cHarv=2*atan2(sqrt(aHarv),sqrt(1.0-aHarv));
//    float dLat =  MyMath::toRadians(lat2 - lat1);
//    float dLon = MyMath::toRadians(lon2 - lon1);
//    float sindLat = sin(dLat/2);
//    float sindLon = sin(dLon/2);
//    float mid = pow(sindLat, 2) + pow(sindLon, 2) * cos(toRadians(lat1) * cos(toRadians(lat2)));
//    float c = 2 * atan2(sqrt(mid), sqrt(1-mid));
    return earthRad * cHarv;
}

float MyMath::equirec(float lat1, float lon1, float lat2, float lon2)
{
    float rad = 0.017453292519943295; // Pi / 180
    float a = (90 - lat1) * rad;
    float b = (90 - lat2) * rad;
    return sqrt( a*a + b*b - 2 * a * b * cos((lon2 - lon1)*rad)) * 6368500;

}











bool MyMath::isInRange(float lat1, float lon1, float lat2, float lon2, float range)
{
    return (haversine(lat1, lon1, lat2, lon2) <= range);
}

void MyMath::calculateDynamically(QList<Pedestrian> &pedestrians, Timestep &timestep, float range)
{
    for (Pedestrian &ped: pedestrians)
    {
        bool turnedOn;


    }

}

void MyMath::uptimeOneStep(QList<Pedestrian> &pedestrians, Timestep &timestep, float range)
{
    for(Pedestrian &ped:pedestrians)
    {

        if (ped.cell.first == -2) ped.cell = Analyzer::getCellFor(ped.lon, ped.lat);
        if (ped.cell.first == -1) continue;
        QList< QPair<int, int> > neighbours = Analyzer::getNeighbours(ped.cell, timestep.grid.length(), timestep.grid[0].length());

        bool turnedOn = false;
        for (QPair<int, int> &pair:neighbours)
        {
            const Cell &cell = timestep.grid[pair.first][pair.second];
            for (const Vehicle &veh:cell.vehicles)
            {
                turnedOn = MyMath::isInRange(ped.lat, ped.lon, veh.y, veh.x, range);
                if (turnedOn)
                {
                    break;
                }
            }
            if (turnedOn) break;
        }
        float len = Analyzer::stepCount;
        ped.activityValue = (ped.activityValue * (len - 1) + turnedOn) / len;
    }

}

void MyMath::uptimeWithGrid(QList<Pedestrian> &pedestrians, QList<Timestep> &timesteps, float range)
{
    qDebug() << pedestrians.length();
    for (int i = 0; i < pedestrians.length(); i++)
    {
        //if (i == 0) qDebug() << "Calculating for pedestrian number" << i << endl;
        const Pedestrian &ped = pedestrians.at(i);
        QList<bool> wasInRange;


        //qDebug() << "Getting cell for lon:" << pedestrians.at(i).lon << ", lat" << pedestrians.at(i).lat;


        QPair<int, int> pedCell = Analyzer::getCellFor(pedestrians.at(i).lon, pedestrians.at(i).lat);

        if (pedCell.first == -1) continue;

        //qDebug() << "Cell found: " << Analyzer::grid[pedCell.first][pedCell.second].x << "," << Analyzer::grid[pedCell.first][pedCell.second].y;
        if (i == 0) qDebug() << "Cell coordinates in grid:" << pedCell.first << "," << pedCell.second;

        QList< QPair<int, int> > neighbours = Analyzer::getNeighbours(pedCell, timesteps.first().grid.length(), timesteps.first().grid[0].length());

//        if (i == 0)
//        {
//            qDebug() << "Neighbours:";
//            for(QPair<int,int> pair: neighbours)
//                qDebug() << "Lon:" << pair.first << ", Lat:" << pair.second;
//        }
        for (int j = 0; j < timesteps.length(); j++)
        {
            //qDebug() << "Entered timesteps loop" << endl;


            bool turnedOn = false;
            for (int n = 0; n < neighbours.length(); n++)
            {
                const QPair<int, int> &pair = neighbours.at(n);
                //if (i == 402) qDebug() << "Problem check:" << pair.first << "," << pair.second;
                const Cell &cell = timesteps.at(j).grid[pair.first][pair.second];
                //if (i == 402) qDebug() << "Looping through vehicles, number of vehicles:" << cell.vehicles.length() << endl;
                for (int k = 0; k < cell.vehicles.length(); k++)
                {
                    //qDebug() << "Looping through vehicles, number of vehicles:" << cell.vehicles.length() << endl;
                    Vehicle veh = cell.vehicles.at(k);
                    turnedOn = MyMath::isInRange(ped.lat, ped.lon, veh.y, veh.x, range);
                    if (turnedOn)
                    {
                        if (i == 0 && j < 50)
                        {
                            qDebug() << "Found a car in range  timestep:" << j;
                            qDebug() << "Vehicle's coordinates were: lon" << veh.x << ", lat" << veh.y;
                            qDebug() << "The vehicle was" << MyMath::haversine(ped.lat, ped.lon, veh.y, veh.x) * 1000 << "meters away";
                        }
                        break;
                    }
                }
                if (turnedOn) break;
            }
            wasInRange.append(turnedOn);
        }

        QList<bool> flist = wasInRange;
        QMutableListIterator<bool> it(flist);
        while (it.hasNext())
        {
            if (!it.next())
            {
                it.remove();
            }
        }
        float actVal = flist.length() * 100.0 / wasInRange.length();
        pedestrians[i].activityValue = actVal;


    }

//    for (int i = 0; i < pedestrians.length(); i++)
//    {
//        qDebug() << "Activity value for pedestrian number" << i << ":" << pedestrians[i].activityValue;
//    }

}


void MyMath::uptime(QList<Pedestrian> &pedestrians, QList<Timestep> *timesteps, float range)
{

    for (int i = 0; i < pedestrians.length(); i++)
    {
        QList<bool> wasInRange;


        for (int j = 0; j < timesteps->length(); j++)
        {
            bool turnedOn = false;
            for (int k = 0; k < timesteps->at(j).vehicles.length(); k++)
            {
                const Pedestrian *ped = &pedestrians.at(i);
                const Vehicle *veh = &timesteps->at(j).vehicles.at(k);
                turnedOn = MyMath::isInRange(ped->lat, ped->lon, veh->y, veh->x, range);
                if (turnedOn)
                {
                    break;
                }
            }
            wasInRange.append(turnedOn);
        }



        QList<bool> flist = wasInRange;
        QMutableListIterator<bool> it(flist);
        while (it.hasNext())
        {
            if (!it.next())
            {
                it.remove();
            }
        }
        float actVal = flist.length() * 100.0 / wasInRange.length();
        pedestrians[i].activityValue = actVal;

        qDebug() << "Pedestrian:" << i;



    }
    for (int n = 0; n < pedestrians.length(); n++)
    {
        qDebug() << pedestrians.at(n).activityValue;
    }
}

// |||||                      |||||
// VVVVV CURRENTLY NOT IN USE VVVVV
void MyMath::calcUptime(QList< QList<QString> > *persons, QList<Timestep> *timesteps, float range)
{

    QList<float> activityValues;
    QMultiMap<QString, bool> activity;
    QMultiMap<QString, bool> filteredActivity;

    for (int i = 0; i < timesteps->length(); i++)
    {
        const QList<Pedestrian> *peds = &timesteps->at(i).pedestrians;
        const QList<Vehicle> *vehs = &timesteps->at(i).vehicles;

        for (int j = 0; j < peds->length(); j++)
        {


            bool turnedOn;
            const Pedestrian *ped = &peds->at(j);


            for (int k = 0; k < vehs->length(); k++)
            {
                const Vehicle *veh = &vehs->at(k);
                turnedOn = MyMath::isInRange(ped->lat, ped->lon, veh->y, veh->x, range);
                if (turnedOn)
                {
                    filteredActivity.insert(ped->id, turnedOn);
                    break;
                }
            }
            activity.insert(ped->id, turnedOn);
        }
    }

    // qDebug() << "Timesteps multiplied by each active pedestrian: " << activity.keys().size();

    QList<QString> checked;

    persons->clear();

    foreach (QString curKey, activity.keys())
    {

        if (!checked.contains(curKey))
        {
            float value = filteredActivity.values(curKey).length() * 100.0 / activity.values(curKey).length();
            activityValues.append(value);
            checked.append(curKey);
            QList<QString> person;
            person << curKey << QString::number(value);
            persons->append(person);
        }
    }

    if (activityValues.isEmpty())
    {
        qDebug() << "The simulation contained no pedestrians.";
    }
    else
    {
        qDebug() << "The activity values of the simulation are:" << endl;
    }

    for (int i = 0; i < activityValues.length(); i++)
    {
        qDebug() << activityValues.at(i) << endl;
    }
}

void MyMath::numberOfVehsAndPeds(QList<Timestep> *timesteps, int &vehs, int &peds)
{
    QSet<QString> vehl;
    QSet<QString> pedl;

    for (int i = 0; i < timesteps->length(); i++)
    {
        qDebug() << i;
        const Timestep *step = &timesteps->at(i);
        for (int j = 0; j < step->vehicles.length(); j++)
        {
            const Vehicle *veh = &step->vehicles.at(j);
            vehl.insert(veh->id);
        }
        for (int j = 0; j < step->pedestrians.length(); j++)
        {
            const Pedestrian *ped = &step->pedestrians.at(j);
            pedl.insert(ped->id);
        }
    }
    vehs = vehl.size();
    peds = pedl.size();
}


float MyMath::mean(QList<float> *list)
{
    float sum = 0;
    foreach (float val, *list)
        sum += val;
    return (sum / list->length());
}

float MyMath::median(QList<float> *list)
{
    if (!list && !list->isEmpty()) {
        qDebug() << "Nullpointer or empty list!";
        return 0;
    }
    qSort(list->begin(), list->end());
    int l = list->length();
    float val = list->at(l/2);
    if (l % 2 == 0)
    {
        return ((val + list->at(l/2 - 1)) / 2.0);
    }
    else
    {
        return val;
    }
}




void MyMath::test()
{

    bool a = 0;
    qDebug() << a;
    float b = 0.65;
    int timestepsLength = 55;

    b = (b * (timestepsLength-1) + a) / timestepsLength;
    qDebug() << b;




}























