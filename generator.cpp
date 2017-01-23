#include "generator.h"

#include <QtCore>
#include <qmath.h>
#include <iostream>
#include <QFile>
#include <QQueue>
#include <QTextStream>
#include <QMessageBox>
#include "sensor.h"

#define SIZE 34

generator::generator()
{
    maxColumns = 66;
    bitsPerWord = 10;
    maxFrameSize = 0;
    totalSPS = 0;
    minSPS = 100;
    initialkI = -1;
    numCol = 0;
    numRow = -1;
    majorFrameRate = 0;
    FSCol = 2;
    mapNumber = 0;
    finalkIIndex = 0;
    numAsym = 0;
    columnModifier = 0;
    bitRateAdder = 0;

    possibleBitRates[0] = 1.0f;
    possibleBitRates[1] = 1.2f;
    possibleBitRates[2] = 1.25f;
    possibleBitRates[3] = 1.5f;
    possibleBitRates[4] = 1.6f;
    possibleBitRates[5] = 2.0f;
    possibleBitRates[6] = 2.4f;
    possibleBitRates[7] = 2.5f;
    possibleBitRates[8] = 3.0f;
    possibleBitRates[9] = 3.2f;
    possibleBitRates[10] = 4.0f;
    possibleBitRates[11] = 4.8f;
    possibleBitRates[12] = 5.0f;
    possibleBitRates[13] = 6.0f;
    possibleBitRates[14] = 6.4f;
    possibleBitRates[15] = 8.0f;
    possibleBitRates[16] = 10.0f;

    niceNumbers[0] = 4;
    niceNumbers[1] = 5;
    niceNumbers[2] = 6;
    niceNumbers[3] = 8;
    niceNumbers[4] = 10;
    niceNumbers[5] = 12;
    niceNumbers[6] = 15;
    niceNumbers[7] = 16;
    niceNumbers[8] = 18;
    niceNumbers[9] = 20;
    niceNumbers[10] = 24;
    niceNumbers[11] = 30;
    niceNumbers[12] = 32;
    niceNumbers[13] = 36;
    niceNumbers[14] = 40;
    niceNumbers[15] = 48;
    niceNumbers[16] = 50;
    niceNumbers[17] = 60;
    niceNumbers[18] = 64;
    niceNumbers[19] = 72;
    niceNumbers[20] = 80;
    niceNumbers[21] = 90;
    niceNumbers[22] = 96;
    niceNumbers[23] = 100;
    niceNumbers[24] = 120;
    niceNumbers[25] = 128;
    niceNumbers[26] = 144;
    niceNumbers[27] = 150;
    niceNumbers[28] = 160;
    niceNumbers[29] = 180;
    niceNumbers[30] = 192;
    niceNumbers[31] = 200;
    niceNumbers[32] = 240;
    niceNumbers[33] = 256;
}

bool generator::determineDataMappings(QVector<sensor> *sensors)
{
    initialkI = -1;
    totalSPS = 0;
    numAsym = 0;
    QMessageBox errorMessage;

    //get our maxium number of columns based
    //off of frame sync constrictions
    maxColumns = getMaxColumns();
    if(maxColumns == -1)
    {
        errorMessage.setText("Invalid bits/word value");
        errorMessage.exec();
        return false;
    }

    QVector<int> spsValues;
    int i = 0, k = 0;
    int kI = 0, kPrime = 0;
    bool hasID = false, hasFS = false;

    //grab each value in sps column and total
    //step 3
    for(i=0;i<sensors->size();i++)
    {
        if(sensors->at(i).getAsymmetric())
            numAsym++;

        int tempInt = sensors->at(i).getminSPS();
        if(sensors->at(i).getTag().toLower() == "id")
            hasID = true;
        if(sensors->at(i).getTag().toLower().startsWith("fs"))
            hasFS = true;

        spsValues.append(tempInt);
        if(tempInt < minSPS)
        {
            minSPS = tempInt;
        }
        totalSPS += tempInt;
    }

    //add 10%
    totalSPS *= 1.1;

    //calculate required bitrate step 6
    bitRate = bitsPerWord * totalSPS;
    if(bitRate >= 10000000)
    {
        //errorMessage.setText("We had to constrict the video");
        //errorMessage.exec();
        spsValues.replace(0,bitRate - 10000000);
        bitRate = 9999999;
    }
    bitRate /= 1000000;

    //find an actual bitrate step 7
    if(!findNextBitRate())
    {
        errorMessage.setText("No valid bit rate");
        errorMessage.exec();
        return false;
    }

    bitRate *= 1000000;

    //make sure we don't divide by 0
    if(bitsPerWord == 0)
    {
        errorMessage.setText("Some how bits/word got set to 0!");
        errorMessage.exec();
        return false;
    }

    //re-calculate our new SPS step 8
    totalSPS = bitRate/bitsPerWord;

    //make sure we don't divide by 0
    if(minSPS == 0)
    {
        errorMessage.setText("Some how our minimum SPS got set to 0!");
        errorMessage.exec();
        return false;
    }

    //step 9 calculate max number of words
    maxFrameSize = (int) totalSPS/minSPS;

    //if there are to many words
    if(maxFrameSize > 8192)
    {
        minSPS = totalSPS/8192;
        maxFrameSize = 8192;
    }

    //add id/fs columns
    if(numAsym == 0)
    {
        int tempInt = (qSqrt(maxFrameSize) * minSPS);
        int insertIndex = 0;
        for(i=0;i<sensors->size();i++)
        {
            if(sensors->at(i).getminSPS() < tempInt)
            {
                insertIndex = i;
                break;
            }
        }
        for(i=0;i<FSCol + 1;i++)
        {
            sensor tempS;
            if(i == 0 && !hasID)
            {
                tempS.setTag("id");
                tempS.setMinSPS(tempInt);
                spsValues.insert(insertIndex, tempInt);
                sensors->insert(insertIndex, tempS);
                totalSPS += tempInt;
            }
            if(i != 0 && !hasFS)
            {
                tempS.setTag("fs");
                tempS.setMinSPS(tempInt);
                spsValues.insert(insertIndex, tempInt);
                sensors->insert(insertIndex, tempS);
                totalSPS += tempInt;
            }
        }
    }

    //step 11 and 12
    i = 0;
    kI = totalSPS/spsValues.at(i);

    //calculate intervals for super commutated data
    while(kI < qSqrt(maxFrameSize) || spsValues.at(i-1) == spsValues.at(i))
    {
        //if the sensor is not asymmetrically sampled
        if(!sensors->at(i).getAsymmetric())
        {
            kI = totalSPS/spsValues.at(i);
            kPrime = getSuperInterval(kI);
            if(kPrime == -1)
            {
                errorMessage.setText("Calcutlating a super commutated interval returned an error!");
                errorMessage.exec();
                return false;
            }
            else
            {
                superCommInt.append(kPrime);
            }
        }
        i++;
    }

    //step 13
    int subtractor = 0;
    numCol = calculateColumn();
    while(numRow == -1)
    {
        if(numCol == -1)
        {
            superCommInt.replace(superCommInt.size()-1,
                                 niceNumbers[finalkIIndex-subtractor]);
            numCol = calculateColumn2();
        }

        //step 14
        numRow = calculateRow();
        subtractor++;
    }
    //detectClash();

    //step 15
    majorFrameRate = totalSPS/(numRow*numCol);

    //step 16
    for(int k=0;k<superCommInt.size();k++)
    {
        actualSPS.append((double) totalSPS/superCommInt.at(k));
        timesPerMajorFrame.append((numRow*numCol)/superCommInt.at(k));
    }

    factors1.clear();
    factors1 = getFactors(numRow);
    factors1.append(1);
    factors1.append(2);
    qSort(factors1);

    //subCommutate the rest step 17
    k=0;
    i = superCommInt.size()+numAsym;   // fixed this
    while(i < sensors->size())
    {
        //qDebug() << "i: " << i << "k: " << k;

        //if the sensor is not asymmetrically sampled
        if(!sensors->at(i).getAsymmetric())
        {
            //calculate k'
            kI = totalSPS/spsValues.at(i);  // zero is divider when i is 80

            int tempV = getSubInterval(kI);

            if(tempV == -1)
            {
                errorMessage.setText("Sub Commute Error");
                errorMessage.exec();
                return false;
            }
            else
            {
                subCommInt.append((numRow/tempV)*numCol);
                actualSPS.append((double) totalSPS/subCommInt.at(k));
                timesPerMajorFrame.append((numRow*numCol)/subCommInt.at(k));
            }
            k++;
        }
        i++;
    }

    //reset our min to our calculated minSPS
    for(i = 0;i<actualSPS.size();i++)
    {
        sensor s;
        s = sensors->at(i);
        s.setMinSPS(actualSPS.at(i));

        sensors->replace(i,s);
    }

    //fill empty spots
    if(numAsym == 0)
    {
        fillEmptySpots(sensors);
    }

    qDebug() << bitRate <<" ACTSPS";
    qDebug() << totalSPS << " totalSPS";
    if(bitRate < totalSPS*bitsPerWord)
    {
        bitRateAdder++;
        clearData();
        determineDataMappings(sensors);
    }

    writeMeasurementText(sensors);

    return true;
}


QVector<QVector<QString> > generator::generateMap(QVector<sensor> *sensors)
{
    //temporarily holds the number of FSCols
    int tempInt = FSCol;

    //set the size of our map
    QVector<QVector<QString> > outputMap;
    outputMap.resize(numRow);
    for(int i=0;i<outputMap.size();i++)
    {
        outputMap[i].resize(numCol);
    }


    //place sfid and fsync columns
    for(int i=0;i<numRow;i++)
    {
        outputMap[i].replace(0,"SFID");
        while(FSCol > 0)
        {
            outputMap[i].replace(numCol-FSCol,"FSYNC");
            FSCol--;
        }
        //reset FSCol
        FSCol = tempInt;
    }

    //place each super commutated label
    for(int i=0;i<superCommInt.size();i++)
    {
        //qDebug() << sensors->at(i+numAsym).getTag();
        if(sensors->at(i+numAsym).getTag() != "id" &&
                sensors->at(i+numAsym).getTag() != "fs")
        {
            for(int j=0;j<numRow;j++)
            {
                int tempCol = i+1;
                while(tempCol < numCol)
                {
                    if(outputMap[j].at(tempCol).isEmpty())
                    {
                        outputMap[j].replace(tempCol,sensors->at(i+numAsym).getTag());
                    }
                    else
                    {
                        //the map tried to place data in a column that already is placed
                    }
                    tempCol += superCommInt.at(i);
                }
            }
        }
    }

    // procedures for determining subSensors, superSensors, counters, i don't
    // think it is good idea to genereates these vectors here.
    ////////////////////////////////////////////////////////////////////////////

    int sensorIndex = 0;
    int numOfSuperSensors = 0;

    while(numOfSuperSensors<superCommInt.size()){
        if(!sensors->at(sensorIndex).getAsymmetric()){
            numOfSuperSensors++;
            superSensors.append(sensors->at(sensorIndex));
        }
        sensorIndex++;
    }

    while(sensorIndex<sensors->size()){
        if(!sensors->at(sensorIndex).getAsymmetric()){
            subSensors.append(sensors->at(sensorIndex));
        }

        sensorIndex++;
    }


    ////////////////////////////////////////////////////////////////////////////


    QVector<int> emptyCol;

    for(int i=0; i<outputMap[0].size(); i++)
        if(outputMap[0][i].isEmpty())
            emptyCol.append(i);

    QList<sensorStruct> subList;
    for(int i=superCommInt.size(); i<timesPerMajorFrame.size(); i++){
        sensorStruct s = {subSensors.at(i-superCommInt.size()).getTag(),
                          numRow/timesPerMajorFrame.at(i),
                          timesPerMajorFrame.at(i)};
        subList.append(s);
    }

    sortSensorList(subList);

    //************for debugging**********************************
    QList<sensorStruct> subList1 = subList;     // for debugging*
    //***********************************************************


    QList<sensorStruct> colToPlace;
    QList<QList<sensorStruct> > matrixToPlace;

    while(!subList.isEmpty()){

        int index = 0;
        sensorStruct tempSensor;
        do
        {
            if(subList.isEmpty())
                break;
            tempSensor = subList.at(index);
            if(colToPlace.isEmpty()){
                colToPlace.append(tempSensor);
                subList.removeAt(index--);
            }
            else{
                if(isFactor(colToPlace, tempSensor)){
                    colToPlace.append(tempSensor);
                    subList.removeAt(index--);
                }
            }

            index++;


        }while(sumRows(colToPlace)<numRow && index < subList.size());

        // restore
        if(sumRows(colToPlace)>numRow){
            colToPlace.pop_back();
            subList.insert(index, tempSensor);
        }

        matrixToPlace.append(colToPlace);
        colToPlace.clear();
    }


    /************for debugging**********************************
    QString sensorCol;
    for(int listIndex = 0; listIndex < matrixToPlace.size(); listIndex++){
        QList<sensorStruct> colList = matrixToPlace.at(listIndex);
        sensorCol.append("Col: \n");
        for(int sIndex = 0; sIndex < colList.size(); sIndex++){
            sensorStruct s = colList.at(sIndex);
            sensorCol.append("Tag: ");
            sensorCol.append(s.tag);
            sensorCol.append(" SpanRows: ");
            sensorCol.append(QString::number(s.spanRows));
            sensorCol.append(" TimesperMajor: ");
            sensorCol.append(QString::number(s.timesPerMajor));
            sensorCol.append(" Fraction: ");
            sensorCol.append(QString::number(s.fraction));
            sensorCol.append("\n");
        }
        qDebug() << sensorCol;
        sensorCol.clear();
    }
    //***********************************************************
    //*/

    int index = 0;

    qDebug() << matrixToPlace.size();
    qDebug() << emptyCol.size();

    if(matrixToPlace.size() > emptyCol.size())
    {
        columnModifier++;
        mapNumber--;
        clearData();
        determineDataMappings(sensors);
        outputMap = generateMap(sensors);
        return outputMap;
    }

    for(int listIndex = 0; listIndex < matrixToPlace.size(); listIndex++){
        QList<sensorStruct> colList = matrixToPlace.at(listIndex);
        int fillCol = emptyCol.at(index);
        int r = 0;
        for(int sIndex = 0; sIndex < colList.size(); sIndex++){
            sensorStruct s = colList.at(sIndex);
            for(int i=0; i<s.timesPerMajor; i++)
                outputMap[r+i*s.spanRows][fillCol] = s.tag;
            for(int i=0; i<numRow; i++){
                if(outputMap[i][fillCol].isEmpty()){
                    r = i;
                    break;
                }
            }
        }

        index++;

        /************for debugging**********************************
        QString rowStr;
        for(int j=0; j<outputMap.size(); j++){
            for(int k=0; k<outputMap[0].size(); k++){
                rowStr.append(outputMap[j][k]);
                rowStr.append(" ");
            }
            qDebug () << rowStr;
            rowStr.clear();
        }
        //***********************************************************
        //*/
    }

    // place counter(s)
    int counterRow = 0;
    for(int i = 0; i < counters.size(); i++){
        sensor s = counters.at(i);
        if(i == numRow-1){
            counterRow = 0;
            index++;
        }
        outputMap[counterRow][emptyCol.at(index)] = s.getTag();
        counterRow++;
    }


    // fill in video
    QString video;
    for(int i = 0; i < sensors->size(); i++){
        sensor s = sensors->at(i);
        if(s.getAsymmetric()){
            video = s.getTag();
            break;
        }
    }

    for(int r=0; r<outputMap.size(); r++){
        for(int c=0; c<outputMap[0].size(); c++){
            if(outputMap[r][c].isEmpty())
                outputMap[r][c] = video;
        }
    }

    /************for debugging**********************************
    for(int i = 0; i < subList1.size(); i++){
        sensorStruct s = subList1.at(i);
        int sum = 0;
        for(int j = 0; j < outputMap.size(); j++){
            QVector<QString> v = outputMap.at(j);
            sum += v.count(s.tag);
        }
        qDebug() << s.tag << ": " << sum;
    }
    //***********************************************************
    //*/

    return outputMap;
}

int generator::getMaxColumns()
{
    FSCol = 3;
    if(bitsPerWord == 16)
    {
        FSCol = 2;
        return 66;
    }
    else if(bitsPerWord == 10 || bitsPerWord == 12)
        return 100;
    else if(bitsPerWord == 8)
        return 133;
    else
        return -1;
}

bool generator::findNextBitRate()
{
    switch((int) bitRate)
    {
    //bit rate is less than 1Mbps
    case 0:
        bitRate = possibleBitRates[0+bitRateAdder];
        break;
    //bit rate is in the 1Mbps range
    case 1:
        if(bitRate < possibleBitRates[1])
        {
            bitRate = possibleBitRates[1+bitRateAdder];
        }
        else if(bitRate < possibleBitRates[2])
        {
            bitRate = possibleBitRates[2+bitRateAdder];
        }
        else if(bitRate < possibleBitRates[3])
        {
            bitRate = possibleBitRates[3+bitRateAdder];
        }
        else if(bitRate < possibleBitRates[4])
        {
            bitRate = possibleBitRates[4+bitRateAdder];
        }
        //return 2Mbps
        else
        {
            bitRate = possibleBitRates[5+bitRateAdder];
        }
        break;
    //bit rate is in the 2Mbps range
    case 2:
        if(bitRate < possibleBitRates[6])
        {
            bitRate = possibleBitRates[6+bitRateAdder];
        }
        else if(bitRate < possibleBitRates[7])
        {
            bitRate = possibleBitRates[7+bitRateAdder];
        }
        //return 3Mbps
        else
        {
            bitRate = possibleBitRates[8+bitRateAdder];
        }
        break;
    //bit rate is in the 3Mbps range
    case 3:
        if(bitRate < possibleBitRates[9])
        {
            bitRate = possibleBitRates[9+bitRateAdder];
        }
        //return 4Mbps
        else
        {
            bitRate = possibleBitRates[10+bitRateAdder];
        }
        break;
    //bit rate is in the 4Mbps range
    case 4:
        if(bitRate < possibleBitRates[11])
        {
            bitRate = possibleBitRates[11+bitRateAdder];
        }
        //return 5Mbps
        else
        {
            bitRate = possibleBitRates[12+bitRateAdder];
        }
        break;
    //bit rate is in the Mbps range
    case 5:
        //return 6Mbps
        bitRate = possibleBitRates[13+bitRateAdder];
        break;
    //bit rate is in the 6Mbps range
    case 6:
        if(bitRate < possibleBitRates[14])
        {
            bitRate = possibleBitRates[14+bitRateAdder];
        }
        else
        {
            bitRate = possibleBitRates[15+bitRateAdder];
        }
        break;
    //bit rate is in the 7Mbps range
    case 7:
        bitRate = possibleBitRates[15+bitRateAdder];
        break;
    //bit rate is in the 8Mbps range
    case 8:
    case 9:
        bitRate = possibleBitRates[16];
        break;
    default:
        //bit rate is not in range
        bitRate = -1;
        break;
    }

    if(bitRate == -1)
        return false;

    return true;
}

int generator::gcd(int value1, int value2)
{
    int number = 0;
    for(int i=1; i<= value1 && i <=value2;i++){
        if(value1 % i == 0 && value2 % i == 0)
        {
            number = i;
        }
    }
    return number;
}

QVector<int> generator::getFactors(int value)
{
    QVector<int> tempV;

    for(int i=2;i<qSqrt(value)+1;i++)
    {
        if(value % i == 0)
        {
            if(i != 2)
            {
                tempV.append(i);
            }
            tempV.append(value/i);
        }
    }
    tempV.append(value);
    return tempV;
}

int generator::getSuperInterval(int kI)
{
    for(int j= SIZE-1;j>=0;j--)
    {
        if(niceNumbers[j] <= kI && niceNumbers[j] <= maxColumns)
        {
            if(initialkI == -1)
            {
                factors = getFactors(niceNumbers[j]);
                initialkI = niceNumbers[j];
                qSort(factors);
                return niceNumbers[j];
            }
            else
            {
                factors1 = getFactors(niceNumbers[j+columnModifier]);
                qSort(factors1);
                for(int r=0;r<factors.size();r++)
                {
                    for(int s=0;s<factors1.size();s++)
                    {
                        if(factors.at(r) == factors1.at(s))
                        {
                            finalkIIndex = j+columnModifier;
                            return niceNumbers[j+columnModifier];//CHANGED
                        }
                    }
                }
            }
        }
    }
    return -1;
}

int generator::getSubInterval(int kI)
{
    for(int k=0;k<factors1.size();k++)
    {
        //grab next largest factor of numRow
        if(factors1.at(k) > ((numRow*numCol)/kI))
        {
            return factors1.at(k);
        }
    }
    return -1;
}

int generator::calculateColumn()
{
    int lcm = -1;

    //calculate the least common multiple of our super commutated intervals
    if(superCommInt.size() >= 2)
    {
        lcm = (superCommInt.at(0) * superCommInt.at(1))/
                gcd(superCommInt.at(0),superCommInt.at(1));

    }
    for(int k = 2; k<superCommInt.size();k++)
    {
        lcm = lcm*superCommInt.at(k)/gcd(lcm,superCommInt.at(k));
    }

    //if the column size we calculated is valid due to frame sync
    if(lcm > maxColumns)
        lcm = -1;

    return lcm;
}

int generator::calculateRow()
{
    int tempRow = maxFrameSize/numCol;

    for(int i = SIZE-1;i>=0;i--)
    {
        if(niceNumbers[i] < tempRow)
        {
            return niceNumbers[i];
        }
    }
    return -1;
}

int generator::calculateColumn2()
{
    for(int i=0;i<superCommInt.size();i++)
    {
        for(int j=SIZE-1;j>=0;j--)
        {
            if(niceNumbers[j] <= superCommInt.at(i) &&
                    superCommInt.at(superCommInt.size()-1) % niceNumbers[j] == 0)
            {
                superCommInt.replace(i,niceNumbers[j]);
                j=0;
            }
        }
    }

    return calculateColumn();
}

void generator::fillEmptySpots(QVector<sensor> *sensors)
{
    int sum =0;
    for(int i=0;i<timesPerMajorFrame.size();i++)
    {
        sum += timesPerMajorFrame.at(i);
    }
    int numEmpty = (numRow*numCol)-sum+1;
    int tempInt = 0;
    int tempSize = superCommInt.size();

    qDebug() << sum << " OLD SUM";
    qDebug() << numEmpty << " OLD EMPTY";

    for(int i=0;i<subCommInt.size();i++)
    {
        if(sensors->at(i+tempSize).getTag().contains("M1") ||
            sensors->at(i+tempSize).getTag().contains("M2") ||
            sensors->at(i+tempSize).getTag().contains("M2"))
        {
            if(timesPerMajorFrame.at(i+tempSize) != numRow)
            {
                tempInt = timesPerMajorFrame.at(i+tempSize);
                subCommInt.replace(i,superCommInt.at(superCommInt.size()-1));
                timesPerMajorFrame.replace(tempSize+i,timesPerMajorFrame.at(superCommInt.size()-1));
                actualSPS.replace(tempSize+i,actualSPS.at(superCommInt.size()-1));
                numEmpty -= numRow-tempInt;
            }
        }
    }

    qDebug() << sum << " NEW SUM";
    qDebug() << numEmpty << " NEW EMPTY";
    for(int i=0;i<subCommInt.size();i++)
    {
        if(timesPerMajorFrame.at(tempSize+i) < numEmpty
                && timesPerMajorFrame.at(i+tempSize) != timesPerMajorFrame.at(tempSize-1))
        {
            tempInt = timesPerMajorFrame.at(i+tempSize);
            subCommInt.replace(i,subCommInt.at(i)/2);
            timesPerMajorFrame.replace(tempSize+i,timesPerMajorFrame.at(i+tempSize)*2);
            actualSPS.replace(tempSize+i,actualSPS.at(i+tempSize)*2);
            numEmpty -= timesPerMajorFrame.at(tempSize+i)-tempInt;
        }
    }

    sum = 0;
    for(int i=0;i<timesPerMajorFrame.size();i++)
    {
        sum += timesPerMajorFrame.at(i);
    }
    qDebug() << sum << " NEW SUM";
    qDebug() << numRow*numCol << " MATRXI";
    if(sum < numRow*numCol)
        fillEmptySpots(sensors);
}

void generator::writeMeasurementText(QVector<sensor> *sensors)
{
    QString temp = "MeasurementList";
    temp.append(QString::number(mapNumber));
    temp.append(".txt");
    mapNumber++;

    QFile file(temp);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    out << numCol << "/" << numRow << endl;
    out << bitRate/1000000 << " bitRate" << endl;

    for(int k=numAsym;k<actualSPS.size();k++)
    {
        if(!sensors->at(k).getAsymmetric())
        {
            out << sensors->at(k).getTag() << " ";
            if(k-numAsym < superCommInt.size())
            {
                out << "Super int: " << superCommInt.at(k-numAsym) << ",";
                out << "Actual sps: " << actualSPS.at(k-numAsym) << ",";
                out << "PerMajor: " << timesPerMajorFrame.at(k-numAsym) << endl;
            }
            else
            {
                out << "Sub int: " << subCommInt.at(k-superCommInt.size()) << ",";
                out << "Actual sps: " << actualSPS.at(k) << ",";
                out << "PerMajor: " << timesPerMajorFrame.at(k) << endl;
            }
        }
    }
}

void generator::detectClash()
{
    QVector<int> initialCol;
    bool clash = false;
    for(int i=0;i<superCommInt.size()-(1+FSCol);i++)
    {
        clash = false;
        for(int j=0;j<initialCol.size();j++)
        {
            if(i+1 == initialCol.at(j))
                clash = true;
        }
        if(!clash)
            initialCol.append(i+1);
    }
    clash = false;
    for(int i=0;i<initialCol.size();i++)
    {
        for(int j=0;j<initialCol.size();j++)
        {
            if(i != j)
            {
                //there will be a clash
                if((initialCol.at(j) - initialCol.at(i) %
                    gcd(superCommInt.at(i), superCommInt.at(j))) == 0)
                {
                    clash = true;
                }
            }
        }
    }

    if(clash)
    {
        fixClash();
        //detectClash();
    }
    return;
}

void generator::fixClash()
{
    QVector<int> gValues;
    int minG = superCommInt.size()-1;

    for(int i=0;i<superCommInt.size()-(1+FSCol);i++)
    {
        int tempInt = 0;
        int x = superCommInt.at(i);
        for(int j=0;j<superCommInt.size()-(1+FSCol);j++)
        {
            int y = superCommInt.at(j);
            if(i != j)
            {
                if(x < y)
                    tempInt += gcd(x, y)/x;
                else
                    tempInt += gcd(x,y)/y;
            }
        }
        if(tempInt < superCommInt.at(minG))
            minG = i;
        gValues.append(tempInt);
    }
    while(superCommInt.at(minG) == superCommInt.at(minG+1))
        minG++;

    qDebug() << superCommInt.at(minG) << "MIN";

    for(int i=SIZE;i>=0;i--)
    {
        if(niceNumbers[i] < superCommInt.at(minG))
        {
            superCommInt.replace(minG,niceNumbers[i]);
            break;
        }
    }/*
    for(int i=0;i<SIZE;i++)
    {
        if(niceNumbers[i] > superCommInt.at(minG))
        {
            superCommInt.replace(minG,niceNumbers[i]);
            break;
        }
    }*/
}

void generator::clearBitRateAdder()
{
    bitRateAdder = 0;
}

void generator::setBitsPerWord(int value)
{
    bitsPerWord = value;
}

void generator::clearData()
{
    maxColumns = 66;
    bitsPerWord = 10;
    maxFrameSize = 0;
    totalSPS = 0;
    minSPS = 100;
    initialkI = -1;
    numCol = 0;
    numRow = -1;
    majorFrameRate = 0;
    FSCol = 2;
    finalkIIndex = 0;
    numAsym = 0;
    columnModifier = 0;

    factors.clear();
    factors1.clear();
    superCommInt.clear();
    subCommInt.clear();
    actualSPS.clear();
    timesPerMajorFrame.clear();
    sensorTags.clear();
    subSensors.clear();
    superSensors.clear();
    counters.clear();
}

float generator::getBitRate()
{
    return bitRate;
}

int generator::getFSNumber()
{
    return FSCol;
}

float generator::sumRows(const QList<sensorStruct>& l)
{
    float sum = 0;
    for(int i = 0; i < l.size(); i++){
        sensorStruct s = l.at(i);
        sum += s.timesPerMajor;
    }

    return sum;
}

bool generator::isFactor(const QList<sensorStruct>& l, const sensorStruct& n2)
{
    for(int i = 0; i < l.size(); i++){
        sensorStruct s = l.at(i);
        if(n2.spanRows%s.spanRows!=0)
            return false;
    }

    return true;
}

void generator::sortSensorList(QList<sensorStruct>& sList)
{
    sensorStruct s;
    int j = 0;

    for(int i=1;i<sList.size();i++)
    {
        s = sList.at(i);
        j = i -1;
        while(j >= 0 && sList.at(j).spanRows > s.spanRows)
        {
            sList.replace(j+1, sList.at(j));
            j--;
        }
        sList.replace(j+1, s);
    }

}
