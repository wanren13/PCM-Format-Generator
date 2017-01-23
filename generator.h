#ifndef GENERATOR_H
#define GENERATOR_H

#include <QVector>
#include <QList>
#include <QString>
#include "sensor.h"

class generator
{
public:
    generator();
    bool determineDataMappings(QVector<sensor> *sensors);
    QVector<QVector<QString> > generateMap(QVector<sensor> *sensors);
    void setBitsPerWord(int value);
    void clearData();
    float getBitRate();
    int getFSNumber();
    void clearBitRateAdder();

private:
    int maxColumns;
    int bitsPerWord;
    int totalSPS;
    int minSPS;
    int maxFrameSize;
    float bitRate;
    float possibleBitRates[17];
    int niceNumbers[34];
    int initialkI;
    QVector<int> factors, factors1;
    QVector<int> superCommInt;
    QVector<int> subCommInt;
    QVector<double> actualSPS;
    QVector<int> timesPerMajorFrame;
    int numCol;
    int numRow;
    double majorFrameRate;
    int FSCol;
    QVector<QString> sensorTags;
    int mapNumber;
    int finalkIIndex;
    int numAsym;
    int columnModifier;
    int bitRateAdder;

    //utility functions
    int getMaxColumns();
    bool findNextBitRate();
    int gcd(int value1, int value2);
    QVector<int> getFactors(int value);
    int getSuperInterval(int kI);
    int getSubInterval(int kI);
    int calculateColumn();
    int calculateColumn2();
    int calculateRow();
    void fillEmptySpots(QVector<sensor> *sensors);
    void writeMeasurementText(QVector<sensor> *sensors);
    void detectClash();
    void fixClash();

    // new members and functions
    QVector<sensor> subSensors;
    QVector<sensor> superSensors;
    QVector<sensor> counters;

    struct sensorStruct{
        QString tag;
        int spanRows;
        int timesPerMajor;
    };


    float sumRows(const QList<sensorStruct>& l);
    bool isFactor(const QList<sensorStruct>& l, const sensorStruct& n2);
    void sortSensorList(QList<sensorStruct>& sList);
};



#endif // GENERATOR_H
