#include "MyThread.h"

MyThread::MyThread(QObject *parent)
: QObject(parent)
{
}

//接收主程序发送过来的相机和窗口句柄并打开计时器
void MyThread::Start_Time(HTuple hv_AcqHandle, HTuple hv_WindowHandle)
{
    AcqHandle = hv_AcqHandle;
    WindowHandle = hv_WindowHandle;
    timer = new QTimer();
    int k=20;
    timer->start(k);
    connect(timer, &QTimer::timeout, this, [=](){Start_Circulation();});
}

//根据时间跳动循环采图
void MyThread::Start_Circulation()
{
    GrabImage(&Image, AcqHandle);
    emit stary_thread(Image);
    qDebug()<<cnStr("采图线程号：")<<QThread::currentThread();
}

//停止采图
void MyThread::Time_Stop()
{
    timer->stop();
    emit stop_thread();
}