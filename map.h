#ifndef MAP_H
#define MAP_H

#include <QString>
#include <QVector>
#include <QMap>
#include "sensor.h"
#include "checkerSensor.h"
#include "generator.h"

class map
{
public:
    map();
    bool createMap();
    void addSensors(QVector<sensor> tempS);
    void setAsymmetricSensor(QVector<bool> tempB);
    void clearData();

    QString getMissionName() const;
    QString getDecom() const;
    QString getEncoder() const;
    QString getFlightLength() const;
    QString getOutFile() const;

    int getBitsPerWord() const;
    float getBitRate();
    QString getFrameSyncPattern();
    QString getSfid() const;
    int getSfidColumn() const;
    int getSfidMask() const;
    QString getDecomType() const;
    QString getEncoderType() const;
    float getSecondsPerWord();
    float getSecondsPerFrame();
    float getSecondsPerMajorFrame();
    QVector<sensor> getSensors() const;
    QMap<QString, sensor> getSensorsMap() const;
    QVector<QVector<QString> > getOutputMatrix() const;
    int getOutputRows() const;
    int getOutputCols() const;

    void setMissionName(const QString &value);
    void setDecom(const QString &value);
    void setEncoder(const QString &value);
    void setFlightLength(const QString &value);
    void setOutFile(const QString &value);
    void setDecomType(const QString &value);
    void setEncoderType(const QString &value);

    void setSensors(const QVector<sensor> &value);
    void setBitsPerWord(int value);

    void checker();
    void clearBitRateAdder();

private:
    QString missionName;
    QString decom;
    QString encoder;
    QString flightLength;
    QString outFile;

    int bitsPerWord;
    QString sfid;
    int sfidColumn;
    int sfidMask;
    QString decomType;
    QString encoderType;
    QVector<sensor> sensors;
    QMap<QString, sensor> sensorsMap;
    QVector<QVector<QString> > outputMatrix;

    generator theGenerator;

};

#endif // MAP_H
