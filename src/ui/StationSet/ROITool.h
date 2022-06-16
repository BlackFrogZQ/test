#pragma once
#include "ui_ROITool.h"
#include "HalconCpp.h"
#include "HDevThread.h"
#include "halconshare.h"
#include "MyThread_ROI.h"
#include <QWidget>
#include <QDebug>
#include <QStringList>
#define cnStr(str) QString::fromLocal8Bit(str)
using namespace HalconCpp;

class ROITool : public QWidget  {
    Q_OBJECT

public:
    explicit ROITool(QWidget* parent = nullptr);
    ~ROITool();

public slots:
    void Obtain_ROIImage();
    void Model_Show_ROIImage(const HObject& Image, QString CircularOrLine_Tem, QString Station_Num_Tem, int ROI_StationNum_Tem);
    void DrawROI();
    void Show_ROIContours(const HObject& Contours_Image);
    void Saves_ROI();
    void dealClose();

signals:
    void Send_ObtainROIImage();
    void Send_StartThread_ROI(const HObject& M_obj, HTuple hv_WindowHandle, int rBtn_Shape, QString CircularOrLine);

private:
    Ui::ROITool *ui;
    HObject ROI_Image;
    HTuple  x, y, ROI_WindowHandle, ROIContours_WindowHandle, x2, y2;

    MyThread_ROI  *mT_ROI;
    QThread *thread_ROI;

    bool Lock_UpdateROIImage = false;
    bool Lock_ROIContours = false;
    int rBtn_Shape = 1;
    QString CircularOrLine;
    QString Station_Num;
    int ROI_StationNum;
};