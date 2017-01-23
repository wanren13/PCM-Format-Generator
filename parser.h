#ifndef PARSER_H
#define PARSER_H

#include<Qstring>
#include<QVector>

class parser
{
public:
    parser();
    //loads and reads in file temp
    bool LoadParser(QString temp);
    void printFile();

    int getNumOfRows();
    int getNumOfCols();
    int getspsCol();
    int getspsTotal();
    QString getTitleAt(int index);
    QString getFileName();

    QString getEntry(int row, int col);
    QVector<QString> getRow(int index);

    void setRow(int index, QVector<QString> temp);
    void clearData();
private:
    //QVector of the title row
    QVector<QString> titles;
    //each column is stored in its own QString Vector
    QVector<QVector<QString> > inputFileData;
    int numOfRows;
    int numOfCols;
    int spsColumn;
    int spsMin;
    int spsTotal;
    QString fileName;

    bool parseTxt();
    bool parseXlsx();
    void cleanData();
    void findTotalSPS();
};

#endif // PARSER_H
