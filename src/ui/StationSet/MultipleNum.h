#pragma once
#include "ui_MultipleNum.h"
#include <QWidget >
#define cnStr(str) QString::fromLocal8Bit(str)

class MultipleNum : public QWidget  {
    Q_OBJECT

public:
    explicit MultipleNum(QWidget* parent = nullptr);
    ~MultipleNum();

    void Obtain_Num();

signals:
    void Send_Num(int NUM);

private:
    Ui::MultipleNum *ui;

    int NUM_tem;
};