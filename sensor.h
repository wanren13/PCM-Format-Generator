#ifndef SENSOR_H
#define SENSOR_H

#include <QString>

class sensor
{
public:
    sensor();

    QString getTag() const;
    QString getMnemonic() const;
    QString getDescription() const;
    int getInitialWord() const;
    int getWordInterval() const;
    int getFrameInterval() const;
    int getStartBit() const;
    int getLength() const;
    int getInitialFrame() const;
    bool getAsymmetric() const;
    int getminSPS() const;

    void setTag(const QString &value);
    void setMnemonic(const QString &value);
    void setDescription(const QString &value);
    void setInitialWord(int value);
    void setWordInterval(int value);
    void setFrameInterval(int value);
    void setStartBit(int value);
    void setLength(int value);
    void setInitialFrame(int value);
    void setAsymmetric(bool value);
    void setMinSPS(int value);

private:
    QString tag;
    QString mnemonic;
    QString description;
    bool aSymmetric;
    int minSPS;
    int initialWord;
    int wordInterval;
    int initialFrame;
    int frameInterval;
    int startBit;
    int length;

};

#endif // SENSOR_H
