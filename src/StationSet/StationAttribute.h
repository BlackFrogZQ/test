#pragma once
#include "ui_StationAttribute.h"
#include "MultipleNum.h"
#include <QWidget>
#define cnStr(str) QString::fromLocal8Bit(str)

class StationAttribute : public QWidget  {
    Q_OBJECT

public:
    explicit StationAttribute(QWidget* parent = nullptr);
    ~StationAttribute();

    void CurrentRow_Abute();
    void AddStation();
    void Receive_Num(int NUM);

signals:
    void Send_Abute(QString Abute, QString AbuteName, int NUM);

private:
    Ui::StationAttribute *ui;
    MultipleNum *mNum;
    StationAttribute *mStationAbute;

    int SelectAbute_Row;
    int NUM = 0;
    QString Abute = NULL;
    QString AbuteName = NULL;
};