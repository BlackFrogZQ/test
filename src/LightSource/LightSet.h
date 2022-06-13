#pragma once
#include "ui_LightSet.h"
#include "HalconCpp.h"
#include "HDevThread.h"
#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QRegExp>
#include <QDialog>
#include <QStatusBar>

using namespace HalconCpp;

#define cnStr(str) QString::fromLocal8Bit(str)

class LightSet : public QWidget  {
    Q_OBJECT

public:
    explicit LightSet(QWidget* parent = nullptr);
    ~LightSet();

    QStringList getPortNameList();//获取所有可用的串口列表

signals:

public slots:
    void openPort(QLabel * label_statubar_light);//打开串口

private:
    Ui::LightSet *ui;
    HTuple hv_SerialHandle, hv_output_SerialPort, hv_string_Raw, hv_output, hv_Ords, hv_Raw;
    bool lock_SerialPort  = false;
};