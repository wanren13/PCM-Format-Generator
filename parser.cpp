#include <QString>
#include <QFile>
#include <QStringList>
#include <iostream>
#include <QTextStream>
#include "parser.h"
#include <QMessageBox>
#include <qmath.h>
#include "xlsxdocument.h"

#include <QDebug>

parser::parser()
{
    fileName = "";
    numOfRows = 0;
    numOfCols = 0;
    spsColumn = 0;
    spsTotal = 0;
    spsMin = 50000;
}

bool parser::LoadParser(QString temp)
{
    fileName = temp;

    if(fileName.endsWith(".xlsx"))
        return parseXlsx();

    if(fileName.endsWith(".txt"))
        return parseTxt();

    return false;
}


bool parser::parseTxt()
{
    int counter=0;    // avoid reading title into minimum value
    //    int minimum=0;    // smallest value in column 5

    //    fileName = temp;
    QFile inputFile(fileName);

    //file failed to open
    if(!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    //open up a stream to read
    QTextStream in(&inputFile);

    //read in the title row first
    if(!in.atEnd())
    {
        QString line = in.readLine();
        //split the line by commas
        QStringList split = line.split(",");
        numOfCols = split.size();

        for(int i=0; i<split.size();i++)
        {
            line = split.at(i);
            line.remove((" "));

            titles.append(line);
            //check if this column is sps column
            if(line.toLower().contains("sps") ||
                    line.toLower().contains("second") ||
                    line.toLower().contains("sample"))
            {
                spsColumn = i;
            }
        }
    }
    while(!in.atEnd())
    {
        //read one line
        QString line = in.readLine();
        //split the line by commas
        QStringList split = line.split(",");

        QVector<QString> rowData;

        //insert each value to its corresponding column
        for(int i=0; i<split.size(); i++)
        {
            line = split.at(i);
            line.remove(" ");

            if(i == spsColumn)
            {
                //row is part of a parallel stream
                if(line == "")
                {
                    //the value is the same as the inital stream
                    line = "0";
                }
                //check if line is text
                else
                {
                    QRegExp re("\\d*");
                    if(!re.exactMatch(line))
                    {
                        line = "-1";
                    }
                }
            }
            rowData.append(line);
        }

        inputFileData.append(rowData);
        counter++;
    }
    inputFile.close();

    numOfRows = inputFileData.size();
    numOfCols = inputFileData[0].size();

    cleanData();
    findTotalSPS();

    return true;
}


bool parser::parseXlsx()
{
    int counter=0;    // avoid reading title into minimum value
    //    int minimum=0;    // smallest value in column 5

    QXlsx::Document xlsx(fileName);

    QXlsx::CellRange r = xlsx.dimension();

    QString line;

    for(int col=r.firstColumn(); col<=r.lastColumn(); col++)
    {
        line = xlsx.read(r.firstRow(),col).toString();
        line.remove((" "));
        titles.append(line);

        if(line.toLower().contains("sps") ||
                line.toLower().contains("second") ||
                line.toLower().contains("sample"))
            spsColumn = col;
    }

    for(int row=r.firstRow()+1; row<=r.lastRow(); row++)
    {
        QVector<QString> rowData;
        //insert each value to its corresponding column
        for(int col=r.firstColumn(); col<=r.lastColumn(); col++)
        {
            line = xlsx.read(row ,col).toString();            
            if(col == spsColumn)
            {
                line.remove((" "));
                //row is part of a parallel stream
                if(line.isEmpty())
                {
                    //the value is the same as the inital stream
                    line = "0";
                }
                //check if line is text
                else
                {
                    QRegExp re("\\d*");
                    if(!re.exactMatch(line))
                    {
                        line = "-1";
                    }
                }
            }
            rowData.append(line);
        }

        inputFileData.append(rowData);
    }    

    numOfRows = inputFileData.size();
    numOfCols = inputFileData[0].size();

    spsColumn = spsColumn - 1;

    if(spsColumn < 0)
        return false;

    cleanData();
    findTotalSPS();

    return true;
}


void parser::setRow(int index, QVector<QString> temp)
{
    inputFileData[index] = temp;
}

void parser::cleanData()
{/*
    //int to place into frame sync and id row
    int tempInt = 2500;
    //flags used to check if ID or frame sync rows already exist
    bool IDflag = false;
    bool FSflag = false;

    for(int i=0;i<numOfRows;i++)
    {
        if(inputFileData[i].at(spsColumn) == "-1" |
           inputFileData[i].at(spsColumn) == "0")   // Requested by Josh
        {
            inputFileData[i].insert(spsColumn, QString::number(spsMin));
        }
        //check to see if there is already an id column
        if(inputFileData[i].at(0) == "ID")
        {
            IDflag = true;
        }
        if(inputFileData[i].at(0).contains("FS"))
        {
            FSflag = true;
        }
    }
    if(IDflag == false)
    {
        QVector<QString> temp;
        for(int i=0;i<numOfCols;i++)
        {
            temp.append("ID");
        }
        temp.insert(spsColumn, QString::number(tempInt));
        inputFileData.append(temp);
        numOfRows++;
    }
    if(FSflag == false)
    {
        QVector<QString> temp, temp1, temp2;
        if(qSqrt(spsTotal) > 67)
        {
            FSflag = true;
        }
        for(int i=0;i<numOfCols;i++)
        {
            temp.append("FS1");
            temp1.append("FS2");
            temp2.append("FS3");
        }
        temp.insert(spsColumn, QString::number(tempInt));
        temp1.insert(spsColumn, QString::number(tempInt));
        temp2.insert(spsColumn, QString::number(tempInt));
        inputFileData.append(temp);
        numOfRows++;
        inputFileData.append(temp1);
        numOfRows++;
        if(FSflag == true)
        {
            inputFileData.append(temp2);
            numOfRows++;
        }
    }*/
}

void parser::findTotalSPS()
{
    spsTotal = 0;
    int temp = 0;
    for(int i=0; i<numOfRows;i++)
    {
        temp = inputFileData[i].at(spsColumn).toInt();
        spsTotal += temp;
    }
    spsTotal*=1.10;
}

/*
void parser::sortBySPSColumn()
{
    QVector<QString> temp;
    int j = 0;

    for(int i=1;i<numOfRows;i++)
    {
        temp = getRow(i);
        j = i -1;
        while(j >= 0 && compareLines(getEntry(j, spsColumn), temp.at(spsColumn)))
        {
            setRow(j+1, getRow(j));
            j--;
        }
        setRow(j+1, temp);
    }
}


// return true if line1 < line2
bool parser::compareLines(QString line1, QString line2)
{
    QRegExp re("^[0-9]+$");

    if(re.exactMatch(line1) && re.exactMatch(line2))
        return line1.toInt() < line2.toInt();
    else if(re.exactMatch(line1) && !re.exactMatch(line2))
        return false;
    else if(!re.exactMatch(line1) && re.exactMatch(line2))
        return true;
    else
        return line1 < line2;
}
*/
void parser::clearData()
{
    fileName = "";
    numOfRows = 0;
    numOfCols = 0;
    spsColumn = 0;
    spsTotal = 0;
    spsMin = 50000;

    for(int i=0; i<inputFileData.size(); i++)
    {
        inputFileData[i].clear();
    }
    inputFileData.clear();
}

int parser::getNumOfRows()
{
    return numOfRows;
}

int parser::getNumOfCols()
{
    return numOfCols;
}

int parser::getspsCol()
{
    return spsColumn;
}

int parser::getspsTotal()
{
    return spsTotal;
}

QString parser::getFileName()
{
    return fileName;
}

QString parser::getEntry(int row, int col)
{
    return inputFileData[row][col];
}

QVector<QString> parser::getRow(int index)
{
    return inputFileData[index];
}

QString parser::getTitleAt(int index)
{
    return titles.at(index);
}
