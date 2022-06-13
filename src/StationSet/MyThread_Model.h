#pragma once

#include "HalconCpp.h"
#include "HDevThread.h"
#include "halconshare.h"
#define cnStr(str) QString::fromLocal8Bit(str)

#include <QWidget>
#include <QThread>
#include <QObject>
#include <QTimer>
#include <QString>
#include <QMetaType>



using namespace HalconCpp;

class MyThread_Model : public QObject
{
    Q_OBJECT
public:
    explicit MyThread_Model(QObject *parent = nullptr);

public slots:
    void GenerateTemplate(const HObject& M_obj, HTuple hv_WindowHandle, int rBtn_Shape, int Numberlever, int AngleStart, 
                    int AngleExtern, int AngleStep, int Contrast, int MinContrast, QString Optimization, QString Metric);
    void mT_SavesModel(QString Station_Num);

signals:
    void Send_ModelContours(const HObject& p_obj);

protected:

private:
    HObject ho_Image, ho_GrayImage, ho_Model_template, ho_ImageReduced_template, ho_ModelImages, ho_ModelRegions, 
            ho_ModelContours, ho_ContoursAffinTrans,ModelContours;
    HTuple  hv_AcqHandle, hv_Width, hv_Height, hv_WindowHandle, hv_Row_template, hv_Column_template, hv_Radius_template, 
            hv_Row1, hv_Column1, hv_Row2, hv_Column2, hv_Row, hv_Column, hv_Phi, hv_Length1, hv_Length2, hv_ModelID, 
            hv_Angle, hv_Score, hv_HomMat2D, hv_Area_center, hv_Row_center, hv_Column_center, hv_Concat, hv_Row_ROI, 
            hv_Column_ROI, hv_Radius_ROI;
};