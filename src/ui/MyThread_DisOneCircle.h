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
#include <QMetaType>
#include <QTextBrowser>

using namespace HalconCpp;

class MyThread_DisOneCircle : public QObject
{
    Q_OBJECT
public:
    explicit MyThread_DisOneCircle(QObject *parent = nullptr);

public slots:
    void Start_DisOneCircle(HObject ho_Image, HTuple WindowHandle, int MaxScore_Place);

signals:
    void Send_FinishDisOneCircle(const HObject& Image_obj, const HTuple& Row_obj, const HTuple& Columm_obj, const HObject& Cross_obj, const HObject& Contours_obj);
    void Send_DisOneCircleFail();

protected:

private:
    HObject ho_Image, ho_GrayImage, ho_ModelContours, ho_ContoursAffinTrans, ho_Region, ho_Image1, ho_GrayImage1, ho_RegionAffineTrans, 
            ho_ImageReduced2, ho_ImageMedian, ho_Edges, ho_SelectedContours, ho_SelectedXLD, ho_SelectedXLD1, ho_Cross, ho_Image_XS;

    HTuple  hv_AcqHandle, hv_Width, hv_Height, hv_WindowHandle, hv_ModelID, hv_Tuple, hv_RomM, hv_ColM, hv_HomMat2D, hv_Row1, hv_Column1, 
            hv_Angle, hv_Score, hv_HomMat2D1, hv_Circularity, hv_Number, hv_CIRCULARITY, hv_Index, hv_Number1, hv_Area1, hv_Row2, hv_Column2, 
            hv_PointOrder, hv_CRadius, hv_Number2;

    QString result, result_CK, result_MM;
};