#pragma once
#include "ui_StationSet.h"
#include "StationAttribute.h"
#include "ModelTool.h"
#include "ROITool.h"
#include <QDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDebug>
#define cnStr(str) QString::fromLocal8Bit(str)

class StationSet : public QDialog  {
    Q_OBJECT

public:
    explicit StationSet(QWidget* parent = nullptr);
    ~StationSet();

    void ShowStation();
    void UpdateTable(QString Abute, QString AbuteName, int NUM);
    void CreateModel();
    void ObtainStationAbute(int MaxScore_Place);

signals:
    void Send_ShowModelTool(QString Station_Num);
    void Send_ShowROITool(QString CircularOrLine, QString Station_Num, int ROI_StationNum);
    void Send_MScoreAbuteName(QString MScoreAbuteName, int MaxScore_Place, int WorkStep_Sum);

private:
    Ui::StationSet *ui;
    StationAttribute *mStationAbute;

    int Row_Station = 0;
    QString CircularOrLine;
};