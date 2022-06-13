#pragma once
#include "ui_CameraParams.h"
#include <QWidget>
#include <QMessageBox>
#include <QDebug>

#define cnStr(str) QString::fromLocal8Bit(str)

class CameraParams : public QWidget  {
    Q_OBJECT

public:
    explicit CameraParams(QWidget* parent = nullptr);
    ~CameraParams();

signals:
    void Send_CameraParams(int GainRaw, int ExposureTime);

private:
    Ui::CameraParams *ui;

    int GainRaw = 51;
    int ExposureTime = 35000;
};