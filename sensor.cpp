#include "sensor.h"

sensor::sensor()
{
    tag = "";
    mnemonic = "";
    description = "";
    aSymmetric = false;
    initialWord = 0;
    wordInterval = 0;
    frameInterval = 0;
    startBit = 0;
    length = 0;
}

//start of getters and setters
QString sensor::getTag() const
{
    return tag;
}

QString sensor::getMnemonic() const
{
    return mnemonic;
}

QString sensor::getDescription() const
{
    return description;
}

int sensor::getInitialWord() const
{
    return initialWord;
}

int sensor::getWordInterval() const
{
    return wordInterval;
}

int sensor::getFrameInterval() const
{
    return frameInterval;
}

int sensor::getStartBit() const
{
    return startBit;
}

int sensor::getLength() const
{
    return length;
}

int sensor::getInitialFrame() const
{
    return initialFrame;
}

bool sensor::getAsymmetric() const
{
    return aSymmetric;
}

int sensor::getminSPS() const
{
    return minSPS;
}

void sensor::setTag(const QString &value)
{
    tag = value;
}

void sensor::setMnemonic(const QString &value)
{
    mnemonic = value;
}

void sensor::setDescription(const QString &value)
{
    description = value;
}

void sensor::setInitialWord(int value)
{
    initialWord = value;
}

void sensor::setWordInterval(int value)
{
    wordInterval = value;
}

void sensor::setFrameInterval(int value)
{
    frameInterval = value;
}

void sensor::setStartBit(int value)
{
    startBit = value;
}

void sensor::setLength(int value)
{
    length = value;
}

void sensor::setInitialFrame(int value)
{
    initialFrame = value;
}

void sensor::setAsymmetric(bool value)
{
    aSymmetric = value;
}

void sensor::setMinSPS(int value)
{
    minSPS = value;
}
