#pragma once

#include "HalconCpp.h"
#include "HDevThread.h"
#include "StationSet/halconshare.h"
#define cnStr(str) QString::fromLocal8Bit(str)

#include <QWidget>
#include <QThread>
#include <QObject>
#include <QTimer>
#include <QString>

using namespace HalconCpp;

class MyThread : public QObject
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = nullptr);

    void Start_Time(HTuple hv_AcqHandle, HTuple hv_WindowHandle);
    void Start_Circulation();

public slots:
    void Time_Stop();

signals:
    void Send_RunStart();
    void stop_thread();
    void stary_thread(const HObject& p_obj);
 
protected:

private:
    HObject Image;
    HTuple  AcqHandle, x, y, WindowHandle;

    QTimer *timer;//定义时钟
};