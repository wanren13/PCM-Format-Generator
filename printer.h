#ifndef PRINTER_H
#define PRINTER_H

#include <QStandardItemModel>
#include "parser.h"
#include "sensor.h"
#include "map.h"
#include "xlsxdocument.h"


class printer
{
public:
    printer(QStandardItemModel *model, map *mPtr);

    void print();

private:
    QStandardItemModel *model;
    map *m;

    QXlsx::Document xlsx;

    void printOverview();
    void printParameterDefinition();
    void printMatrix();
};

#endif // PRINTER_H
