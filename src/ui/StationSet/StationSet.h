#pragma once
#include "ui_StationSet.h"
#include "ModelTool.h"
#include "ROITool.h"
#include "StationAttributeDef.h"
#include "StationSetDef.h"

class QTableWidgetItem;
class CSetStationDialog : public QDialog
{
    Q_OBJECT

public:
    CSetStationDialog(QWidget *parent = nullptr);
    ~CSetStationDialog();

    void CreateModel();
    void ObtainStationAbute(int MaxScore_Place);

signals:
    void Send_ShowModelTool(QString Station_Num);
    void Send_ShowROITool(QString CircularOrLine, QString Station_Num, int ROI_StationNum);
    void Send_MScoreAbuteName(QString MScoreAbuteName, int MaxScore_Place, int WorkStep_Sum);
protected slots:
    void slotAddStationPb();

protected:
    static QTableWidgetItem *getTableWidgetItem(const QString &p_text, int p_textAlignment = Qt::AlignCenter);
    void addCircularPositioning();
    void addMultiCirclePositioning(const int& p_multiCircleCount);
    void addOneLineCrossingTwoSidesPositioning();

    void addStationWorkingProcedure(const CStationWorkingProcedure& p_workingProcedure);
    void addStationTitle(const CStationType& p_stationType, const QString& p_stationName);
private:
    Ui::StationSet *ui;

    int m_stationCount;
    QString CircularOrLine;
};