#include "analyzer.h"
#include "mymath.h"






using namespace std;

QList<Timestep> Analyzer::timesteps;
QList<Pedestrian> *Analyzer::pedestrians;
QString *fn;
float Analyzer::areaLat;
float Analyzer::areaLon;
float Analyzer::areaLat2;
float Analyzer::areaLon2;
float Analyzer::areaWidth;
float Analyzer::areaHeight;
float Analyzer::latStep;
float Analyzer::lonStep;
float Analyzer::range;

// Used when calculating dynamically

int Analyzer::stepCount = 0;


QList< QList<Cell> > Analyzer::grid;
clock_t timer;




Analyzer::Analyzer(QObject *parent):
    QObject(parent)
{

}

void Analyzer::addTimestep(Timestep step)
{
    timesteps.append(step);
}


Vehicle Analyzer::parseVehicle(QXmlStreamReader &xml)
{
    Vehicle veh;
    if (xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "vehicle")
    {
        return veh;
    }

    QXmlStreamAttributes attributes = xml.attributes();

    if (attributes.hasAttribute("id"))
    {
        veh.id = attributes.value("id").toString();
    }
    if (attributes.hasAttribute("x"))
    {
        veh.x = attributes.value("x").toFloat();
    }
    if (attributes.hasAttribute("y"))
    {
        veh.y = attributes.value("y").toFloat();
    }
    return veh;

}

Pedestrian Analyzer::parsePerson(QXmlStreamReader &xml)
{
    Pedestrian ped;
    if (xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "person")
    {
        return ped;
    }

    QXmlStreamAttributes attributes = xml.attributes();

    if (attributes.hasAttribute("id"))
    {
        ped.id = attributes.value("id").toString();
    }
    if (attributes.hasAttribute("x"))
    {
        ped.lon = attributes.value("x").toFloat();
    }
    if (attributes.hasAttribute("y"))
    {
        ped.lat = attributes.value("y").toFloat();
    }

    return ped;

}


// The main PARSING METHOD below

void Analyzer::loadFileStream(QString fileName)
{
    timer = clock();
    QFile *file = new QFile(fileName);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file";
        return;
    }

    QXmlStreamReader xml(file);

    qDebug() << "Creating grid..." << endl;

    int gwid = areaWidth/range;
    int ghei = areaHeight/range;
    qDebug() << "Grid width:" << gwid;
    qDebug() << "Grid height:" << ghei;
    Analyzer::createGrid(Analyzer::grid, gwid, ghei);

    Timestep currentStep;


    qDebug() << "Load file method called";

    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartDocument)
        {
            continue;
        }



        if (token == QXmlStreamReader::EndElement)
        {
            if (xml.name() == "timestep")
            {
                MyMath::uptimeOneStep(*pedestrians, currentStep, range);
            }
        }




        if (token == QXmlStreamReader::StartElement)
        {
            if (xml.name() == "timesteps")
            {
                continue;
            }

            if (xml.name() == "timestep")
            {
                stepCount++;
                //double timeTaken = (clock() - timer) / (double)CLOCKS_PER_SEC;
                //timer = clock();
                Timestep step;
                step.time = xml.attributes().value("id").toFloat();
                step.grid = Analyzer::grid;
                currentStep = step;
            }
            if (xml.name() == "vehicle")
            {

                Vehicle veh = Analyzer::parseVehicle(xml);
                QPair<int, int> loc = Analyzer::getCellFor(veh.x, veh.y);
                //double timeTaken = (clock() - timer) / (double)CLOCKS_PER_SEC;
                //qDebug() << "Done.\nTime taken:" << timeTaken << endl;
                if (loc.first != -1) currentStep.grid[loc.first][loc.second].vehicles.append(veh);


            }



        }

    }


    if (xml.hasError())
    {
        qDebug() << xml.errorString() << endl;
    }
    xml.clear();
    delete file;


}

void Analyzer::setFileName(QString &fileName, QList<Pedestrian> *peds, float &r)
{
    fn = &fileName;
    pedestrians = peds;
    range = r;
}




void Analyzer::DoWork()
{
    qDebug() << "Parsing";
    qDebug() << "File name:" << *this->fn;
    loadFileStream(*this->fn);
    qDebug() << "Complete.";
    emit parsingComplete();
}



void Analyzer::createGrid(QList< QList<Cell> > &grid, float w, float h)
{

    float dLat = areaLat2 - areaLat;
    float dLon = areaLon2 - areaLon;
    latStep = dLat / h;
    lonStep = dLon / w;

    for (int i = 0; i < w; i++)
    {
        grid.append(QList<Cell>());
        for (int j = 0; j < h; j++)
        {
            float lo = areaLon + i * lonStep;
            float la = areaLat + j * latStep;
            grid[i].append(Cell(lo, la));
        }
    }
}


void Analyzer::setAreaValues(float lat, float lon, float lat2, float lon2)
{
    areaLat = lat;
    areaLon = lon;
    areaLat2 = lat2;
    areaLon2 = lon2;
}


// NEEDS CHECKING
QPair<int, int> Analyzer::getCellFor(float x, float y)
{
    QPair<int, int> p;
    int lonx = -1;
    int laty = -1;
    int beg, en, half;

    bool xUnd, xOvr, yUnd, yOvr;
    xUnd = x < grid.first().first().x;
    xOvr = x >= grid.last().last().x + lonStep;
    yUnd = y < grid.first().first().y;
    yOvr = y >= grid.last().last().y + latStep;
    if (xUnd || xOvr || yUnd || yOvr) goto finish;



    // Binary search

    beg = 0;
    en = grid.length() - 1;

    // Find the appropriate longitude first:
    while (true)
    {
        half = en - (en - beg) / 2;

        float testPoint = grid[half][0].x;

        if (testPoint + lonStep - x >= -0.0001 && x - testPoint >= -0.0001)
        {
            lonx = half;
            break;
        }
        else if (x < testPoint) en = half - 1;
        if (x >= testPoint + lonStep) beg = half + 1;
    }

    // And then latitude:
    beg = 0;
    en = grid[0].length() - 1;

    while (true)
    {

        half = en - (en - beg) / 2;
        float testPoint = grid[0][half].y;

        if (testPoint + latStep - y >= -0.0001 && y - testPoint >= -0.0001)
        {
            laty = half;
            break;
        }
        else if (y < testPoint) en = half - 1;
        if (y >= testPoint + latStep) beg = half + 1;
    }


    finish:
    p = qMakePair(lonx, laty);
    return p;

}

QList< QPair<int, int> > Analyzer::getNeighbours(QPair<int, int> pedCell, int w, int h)
{
    QList< QPair<int, int> > resList;
    if (pedCell.first == -1) return resList;
    QList<int> hor, ver;
    hor << -1 << 0 << 1;
    ver << -1 << 0 << 1;

    if (pedCell.first < 1) hor.removeFirst();
    if (pedCell.second < 1) ver.removeFirst();
    if (pedCell.first > w-2) hor.removeLast();
    if (pedCell.second > h-2) ver.removeLast();

    foreach (int hval, hor)
        foreach (int vval, ver)
        {
            resList.append(qMakePair(pedCell.first+hval, pedCell.second+vval));
        }
    return resList;
}































