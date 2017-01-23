#include "printer.h"
#include <QDebug>
#include <QColor>
#include <QMap>

printer::printer(QStandardItemModel *model, map *mPtr)
{
    this->model = model;
    m = mPtr;
}

void printer::print()
{
    printOverview();
    printParameterDefinition();
    printMatrix();

    xlsx.saveAs(m->getOutFile());
}

void printer::printOverview()
{
    xlsx.currentWorksheet()->setColumn("A", "A", 25);
    xlsx.currentWorksheet()->setColumn("B", "B", 20);
    xlsx.currentWorksheet()->setColumn("C", "C", 10);

    QXlsx::Format font;
    font.setFontBold(true);
    font.setFontUnderline(QXlsx::Format::FontUnderlineSingle);

    QXlsx::Format border;
    border.setBorderStyle(QXlsx::Format::BorderThin);
    border.setHorizontalAlignment(QXlsx::Format::AlignLeft);

    QXlsx::Format centerborder;
    centerborder.setBorderStyle(QXlsx::Format::BorderThin);
    centerborder.setHorizontalAlignment(QXlsx::Format::AlignHCenter);

    QXlsx::Format centercolorborder;
    centercolorborder.setBorderStyle(QXlsx::Format::BorderThin);
    centercolorborder.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    centercolorborder.setPatternBackgroundColor(Qt::yellow);

    // Header
    xlsx.write("A1", "TM Link Definition");

    // User Defined
    xlsx.write("A4", "User Defined", font);

    xlsx.write("A5", "Mission Name", border);
    xlsx.write("B5", m->getMissionName(), centerborder);
    xlsx.write("C5", QString(), border);

    xlsx.write("A6", "Bit Rate", border);
    xlsx.write("B6", QString::number(m->getBitRate()/1000000,'f',2), centerborder);
    xlsx.write("C6", "Mbps", border);

    xlsx.write("A7", "Bits per Frame", border);
    xlsx.write("B7", QString::number(m->getBitsPerWord()), centerborder);
    xlsx.write("C7", "bits", border);

    xlsx.write("A8", "Words per Frame", border);
    xlsx.write("B8", QString::number(m->getOutputCols()), centerborder);     // words per frame == words per major ????
    xlsx.write("C8", "words", border);

    xlsx.write("A9", "Frames per Major Frame", border);
    xlsx.write("B9", QString::number(m->getOutputRows()), centerborder);
    xlsx.write("C9", "frames", border);

    xlsx.write("A10", "Framessync Patern(Hex)", border);
    xlsx.write("B10", m->getFrameSyncPattern(), centerborder);
    xlsx.write("C10", QString(), border);

    xlsx.write("A11", "SFID (Y or N)", border);
    xlsx.write("B11", m->getSfid(), centerborder);
    xlsx.write("C11", QString(), border);

    xlsx.write("A12", "SFID Word Location", border);
    xlsx.write("B12", QString::number(m->getSfidColumn()), centerborder);
    xlsx.write("C12", QString(), border);

    xlsx.write("A13", "SFID Mask (binary)", border);
    xlsx.write("B13", QString::number(m->getSfidMask()), centerborder);
    xlsx.write("C13", QString(), border);

    xlsx.write("A14", "Decom Type", border);
    xlsx.write("B14", m->getDecomType(), centerborder);
    xlsx.write("C14", QString(), border);

    xlsx.write("A15", "Encoder Type", border);
    xlsx.write("B15", m->getEncoderType(), centerborder);
    xlsx.write("C15", QString(), border);

    // Calculated
    xlsx.write("A18", "Calculated", font);

    xlsx.write("A19", "Seconds per Word", border);
    xlsx.write("B19", QString::number(m->getSecondsPerWord()), centercolorborder);
    xlsx.write("C19", "s/wd", border);

    xlsx.write("A20", "Seconds per Frame", border);
    xlsx.write("B20", QString::number(m->getSecondsPerFrame()), centercolorborder);
    xlsx.write("C20", "s/frm", border);

    xlsx.write("A21", "Seconds per Major Frame", border);
    xlsx.write("B21", QString::number(m->getSecondsPerMajorFrame()), centercolorborder);
    xlsx.write("C21", "s/mf frm", border);

    xlsx.renameSheet("Sheet1", "Overview");
}

void printer::printParameterDefinition()
{
    xlsx.addSheet("Parameter Definition");

    xlsx.currentWorksheet()->setColumn("A", "A", 10);
    xlsx.currentWorksheet()->setColumn("B", "B", 40);
    xlsx.currentWorksheet()->setColumn("C", "C", 40);
    xlsx.currentWorksheet()->setColumn("D", "D", 15);
    xlsx.currentWorksheet()->setColumn("E", "E", 15);
    xlsx.currentWorksheet()->setColumn("F", "F", 15);
    xlsx.currentWorksheet()->setColumn("G", "G", 15);

    QXlsx::Format aquatitlecell;
    aquatitlecell.setFontBold(true);
    aquatitlecell.setBorderStyle(QXlsx::Format::BorderMedium);
    aquatitlecell.setFontUnderline(QXlsx::Format::FontUnderlineSingle);
    aquatitlecell.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    aquatitlecell.setPatternBackgroundColor(QColor(218,238,243));

    QXlsx::Format yellowtitlecell;
    yellowtitlecell.setFontBold(true);
    yellowtitlecell.setBorderStyle(QXlsx::Format::BorderMedium);
    yellowtitlecell.setFontUnderline(QXlsx::Format::FontUnderlineSingle);
    yellowtitlecell.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    yellowtitlecell.setPatternBackgroundColor(Qt::yellow);

    QXlsx::Format aquacentercell;
    aquacentercell.setBorderStyle(QXlsx::Format::BorderThin);
    aquacentercell.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    aquacentercell.setPatternBackgroundColor(QColor(218,238,243));

    QXlsx::Format aqualeftcell;
    aqualeftcell.setBorderStyle(QXlsx::Format::BorderThin);
    aqualeftcell.setHorizontalAlignment(QXlsx::Format::AlignLeft);
    aqualeftcell.setPatternBackgroundColor(QColor(218,238,243));

    QXlsx::Format yellowcentercell;
    yellowcentercell.setBorderStyle(QXlsx::Format::BorderThin);
    yellowcentercell.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    yellowcentercell.setPatternBackgroundColor(Qt::yellow);

    xlsx.write("A1", "Parameter");

    // write title
    xlsx.write("A4", "Index", aquatitlecell);
    xlsx.write("B4", "Parameter Name(Mnemonic)", aquatitlecell);
    xlsx.write("C4", "Data Description", aquatitlecell);
    xlsx.write("D4", "Word", yellowtitlecell);
    xlsx.write("E4", "Word INT", yellowtitlecell);
    xlsx.write("F4", "Frame", yellowtitlecell);
    xlsx.write("G4", "Frame INT", yellowtitlecell);

    int row = 5;

    QMap<QString, sensor> sMap = m->getSensorsMap();

    for(int r=0; r < model->rowCount(); r++){

        QString index = model->item(r,0)->text();

        sensor s = sMap[index];
        if(s.getAsymmetric()){
            continue;
        }

        QString pName = model->item(r,2)->text();
        QString dDesc = model->item(r,1)->text();

        xlsx.write(row, 1, index, aquacentercell);
        xlsx.write(row, 2, pName, aqualeftcell);
        xlsx.write(row, 3, dDesc, aqualeftcell);
        xlsx.write(row, 4, s.getInitialWord(), yellowcentercell);
        xlsx.write(row, 5, s.getWordInterval(), yellowcentercell);
        xlsx.write(row, 6, s.getInitialFrame(), yellowcentercell);
        xlsx.write(row, 7, s.getFrameInterval(), yellowcentercell);

        row++;
    }
}

void printer::printMatrix()
{
    xlsx.addSheet("Matrix");

    QXlsx::Format titlecell;
    titlecell.setFontBold(true);
    titlecell.setBorderStyle(QXlsx::Format::BorderThin);
    titlecell.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    titlecell.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    titlecell.setPatternBackgroundColor(QColor(191,191,191));

    QXlsx::Format normalell;
    normalell.setBorderStyle(QXlsx::Format::BorderThin);
    normalell.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    normalell.setVerticalAlignment(QXlsx::Format::AlignVCenter);

    // waiting for map to be finished
    QVector<QVector<QString> > matrix = m->getOutputMatrix();
    int rows = matrix.size();
    int cols = matrix[0].size();

    xlsx.currentWorksheet()->setColumn(1, cols, 10);

    xlsx.write("A1", QString("%1/%2").arg(cols).arg(rows), normalell);

    for(int c = 1; c <= cols; c++)
        xlsx.write(1, c+1, QString::number(c), titlecell);

    for(int r = 0; r < rows; r++)
    {
        xlsx.write(r+2, 1, QString::number(r+1), titlecell);
        for(int c = 0; c < cols; c++)
            xlsx.write(r+2, c+2, matrix[r][c], normalell);

    }

}
