#include "map.h"
#include <QtCore>
#include <QMessageBox>

map::map()
{
    missionName = "";
    bitsPerWord = 0;
    sfid = "Y";
    sfidColumn = 1;
    sfidMask = 0;
    decomType = "";
    encoderType = "";
    sensors.clear();
}

bool map::createMap()
{
    theGenerator.setBitsPerWord(bitsPerWord);

    if(theGenerator.determineDataMappings(&sensors))
            outputMatrix = theGenerator.generateMap(&sensors);
    else
        return false;

    checker();
    for(int i=0; i<sensors.size(); i++)
        sensorsMap.insert(sensors[i].getTag(), sensors[i]);

    if(outputMatrix.isEmpty())
        return false;    

    QMessageBox errorMessage;
    errorMessage.setText("SUCCESS! Look for MeasurementList.txt in your directory");
    errorMessage.exec();

    return true;
}

void map::addSensors(QVector<sensor> tempS)
{
    sensors = tempS;
}

void map::setAsymmetricSensor(QVector<bool> tempB)
{
    for(int i=0;i<sensors.size();i++)
    {
        sensor s;
        s = sensors.at(i);
        s.setAsymmetric(tempB.at(i));

        sensors.replace(i, s);
    }
}

void map::clearData()
{
    sensors.clear();
    for(int i=0;i<outputMatrix.size();i++)
    {
        outputMatrix[i].clear();
    }
    outputMatrix.clear();

    theGenerator.clearData();
}

//start of getters and setters
QString map::getMissionName() const
{
    return missionName;
}

QString map::getDecom() const
{
    return decom;
}

QString map::getEncoder() const
{
    return encoder;
}

QString map::getFlightLength() const
{
    return flightLength;
}

QString map::getOutFile() const
{
    return outFile;
}

int map::getBitsPerWord() const
{
    return bitsPerWord;
}

float map::getBitRate()
{
    return theGenerator.getBitRate();
}

QString map::getFrameSyncPattern()
{
    int temp = theGenerator.getFSNumber();

    if(bitsPerWord == 8)
    {
        switch(temp)
        {
        case 2:
            return "EB90";
        case 3:
            return "FAF320";
        case 4:
            return "FE6B2840";
        default:
            break;
        }
    }
    else if(bitsPerWord == 10)
    {
        switch(temp)
        {
        case 2:
            return "EDE20";
        case 3:
            return "3EBCCD00";
        default:
            break;
        }
    }
    else if(bitsPerWord == 12)
    {
        switch(temp)
        {
        case 2:
            return "FAF320";
        }
    }
    else if(bitsPerWord == 16)
    {
        return "FE6B2840";
    }
    return "ERROR";
}

QString map::getSfid() const
{
    return sfid;
}

int map::getSfidColumn() const
{
    return sfidColumn;
}

int map::getSfidMask() const
{
    return sfidMask;
}

QString map::getDecomType() const
{
    return decomType;
}

QString map::getEncoderType() const
{
    return encoderType;
}

float map::getSecondsPerWord()
{
    return bitsPerWord/
            (getBitRate());
}

float map::getSecondsPerFrame()
{
    return (bitsPerWord/
            getBitRate())*
            getOutputCols();
}

float map::getSecondsPerMajorFrame()
{
    return ((bitsPerWord/
            (getBitRate()))*
             getOutputCols())*getOutputRows();
}

QVector<sensor> map::getSensors() const
{
    return sensors;
}

QMap<QString, sensor> map::getSensorsMap() const
{
    return sensorsMap;
}

QVector<QVector<QString> > map::getOutputMatrix() const
{
    return outputMatrix;
}

int map::getOutputRows() const
{
    return outputMatrix.size();
}

int map::getOutputCols() const
{
    if(outputMatrix.size() > 0)
        return outputMatrix[0].size();
    else
        return 0;
}

void map::setMissionName(const QString &value)
{
    missionName = value;
}

void map::setDecom(const QString &value)
{
    decom = value;
    setDecomType(decom);
}

void map::setEncoder(const QString &value)
{
    encoder = value;
    setEncoderType(encoder);
}

void map::setFlightLength(const QString &value)
{
    flightLength = value;
}

void map::setOutFile(const QString &value)
{
    outFile = value;
}

void map::setDecomType(const QString &value)
{
    decomType = value;
}

void map::setEncoderType(const QString &value)
{
    encoderType = value;
}

void map::setSensors(const QVector<sensor> &value)
{
    sensors = value;
}

void map::setBitsPerWord(int value)
{
    bitsPerWord = value;
}

/****************************************************
 *
 *     Everything Below is for the WordCheck
 *
 * *************************************************/
void map::checker()
{
    QVector<checkerSensor> allsensors;
    bool exists = 0;
    bool dupcoords = false;
    coords tempC;
    checkerSensor tempS;
    QString tempSKey;

    int x = 0, y = 0, z = 0;

    checkerSensor s;
    coords c;

    c.x=0;
    c.y=0;

    s.key = outputMatrix[c.x].at(c.y);// c.x = row     c.y = column
    s.coordinates.append(c);
    allsensors.append(s);

    int rowAmount = getOutputRows();
    int colAmount = getOutputCols();

    for(x = 0; x < rowAmount; x++)// Check each row x
    {
        for(y = 0; y < colAmount; y++)// Check each column y in row x
        {
            tempC.x = x;
            tempC.y = y;
            tempSKey = outputMatrix[x].at(y);// x = row   y = column

            for(z = 0; z < allsensors.size(); z++)
            {

                if(allsensors.at(z).key == tempSKey)//Check to see if a key exists in allsensors
                {
                    exists = 1;
                    break;
                }
            }

            if(!exists)
            {
                tempS.key = tempSKey;
                tempS.coordinates.append(tempC);// If the key has not been found, add it, along with its location,
                allsensors.append(tempS);// to our "sensor library" vector
            }
            else
            {
                allsensors[z].coordinates.append(tempC);
            }
            exists = 0;
            dupcoords = false;
            tempS.coordinates.clear();                                                  //Clear pre-existing coords in the temp variable
        }
    }

    int a,b;
    for(a = 0; a < allsensors.size(); a++)//Frame Interval Calculation
    {
        if(allsensors[a].coordinates.size() > 1)
        {
            int xdiff = allsensors[a].coordinates.at(1).x-allsensors[a].coordinates.at(0).x;
            if(xdiff == 0)
                allsensors[a].frameint = 1;
            else
                allsensors[a].frameint = xdiff;
        }
        else
        {
            allsensors[a].frameint = rowAmount;
        }
    }

    for(a = 0; a < allsensors.size(); a++)                               //Word Interval Calculation
    {
        if(allsensors[a].coordinates.size() > 1)
        {
            int ydiff = allsensors[a].coordinates.at(1).y-allsensors[a].coordinates.at(0).y;
            int ymatches = 0;

            for(b = 0; b < allsensors[a].coordinates.size()-1; b++)
            {

                if(allsensors[a].coordinates.at(b+1).y - allsensors[a].coordinates.at(b).y == ydiff &&
                    allsensors[a].coordinates.at(b+1).x == allsensors[a].coordinates.at(b).x)
                {
                    ymatches++;
                }
            }

            if(ymatches == allsensors[a].coordinates.size()- rowAmount )
            {

                if(allsensors[a].frameint == 1)
                    allsensors[a].wordint = ydiff;

            }
            if(ydiff == 0)
            {
                allsensors[a].wordint = (allsensors[a].coordinates.at(1).x - allsensors[a].coordinates.at(0).x)
                                            * colAmount;
            }
        }
        else
        {
            allsensors[a].wordint = colAmount * rowAmount;
        }
    }
    for(int i=0;i<allsensors.size();i++)
    {
        for(int j=0;j<sensors.size();j++)
        {
            if(allsensors[i].key == sensors.at(j).getTag())
            {
                sensors[j].setInitialWord(allsensors[i].coordinates.at(0).y+1 + (colAmount * allsensors[i].coordinates.at(0).x));
                sensors[j].setInitialFrame(allsensors[i].coordinates.at(0).x+1);
                sensors[j].setFrameInterval(allsensors[i].frameint);
                sensors[j].setWordInterval(allsensors[i].wordint);
            }
        }
    }

}

void map::clearBitRateAdder()
{
    theGenerator.clearBitRateAdder();
}
