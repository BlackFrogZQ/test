#pragma once
#include "ui_StationAttribute.h"
#include "src/system/projectManager/StationSetDef.h"
#include "../basic.h"
class StationAttribute : public QDialog
{
    Q_OBJECT

public:
    explicit StationAttribute(QWidget *parent = nullptr);
    ~StationAttribute();
    bool addStation();
    CStation getAddStation(const int& p_id);
protected:
    void addStationTypeItem(const QString& p_icon,const QString& p_cnName);
    void addCircularPositioning();
    void addMultiCirclePositioning();
    void addOneLineCrossingTwoSidesPositioning();
private:
    Ui::StationAttribute *ui;
    CStation m_station;
    bool m_isAdd;
};