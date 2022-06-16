#pragma once
#include "ui_StationAttribute.h"
#include "StationAttributeDef.h"
class StationAttribute : public QDialog
{
    Q_OBJECT

public:
    explicit StationAttribute(QWidget *parent = nullptr);
    ~StationAttribute();
    bool addStation();

    CStationType getStationType() const;
    QString getStationName() const;
    int getMultiCircleCount() const;

protected:
    void addStationTypeItem(const QString& p_icon,const QString& p_cnName);

private:
    Ui::StationAttribute *ui;
    bool m_isAdd;
};