#include "results.h"

using namespace std;


QString *outputFileName;


Results::Results(QObject *parent) :
    QObject(parent)
{
}

void Results::setOutputFileName(QString &fileName, Analyzer &analyzer)
{
    outputFileName = &fileName;
}

void Results::writeToCSV()
{
    if (!outputFileName)
        return;
    else
    {

        qDebug() << *this->outputFileName;
        QFile output(*outputFileName);
        if (output.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream toFile(&output);
            toFile << "Person_ID;Uptime_Percentage" << endl;
            for (int i = 0; i < this->persons.length(); i++)
            {
                toFile << persons.at(i).at(0) << ";" << persons.at(i).at(1) << endl;
            }
        }
        emit writingComplete();
    }

}

void Results::fixedWriteToCSV()
{
    if (!outputFileName)
        return;
    else
    {

        qDebug() << *this->outputFileName;
        QFile output(*outputFileName);
        if (output.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream toFile(&output);
            toFile << "Person_ID;lat;lon;Uptime_Value;Uptime_Percentage" << endl;
            for (int i = 0; i < peds.length(); i++)
            {
                const Pedestrian *ped = &peds.at(i);
                toFile << ped->id << ";" << ped->lat << ";" << ped->lon << ";" << ped->activityValue << ";" << QString::number(ped->activityValue * 100, 'g', 4) << endl;
            }
        }
        emit writingComplete();
    }

}
