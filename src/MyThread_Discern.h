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

using namespace HalconCpp;

class MyThread_Discern : public QObject
{
    Q_OBJECT
public:
    explicit MyThread_Discern(QObject *parent = nullptr);

public slots:
    void Open_GJTime();
    void Start_Discern();
    void Time_GJStop();
    void Update_Image(HObject Image);

signals:
    void Stop_thread();
    void Send_CameraStop(int MaxScore_Place);
    void Send_CameraStart();

protected:

private:
    HObject ho_Image, ho_GrayImage, ho_Regions_White, ho_ConnectedRegions_White, ho_SelectedRegions_White, 
            ho_Contours_White, ho_Contours_WhiteLeft, ho_Contours_WhiteRight, ho_Regions_GJ, ho_ConnectedRegions_GJ, 
            ho_SelectedRegions_GJ, ho_SortedRegions_GJ, ho_ObjectSelected, ho_Contours_GJ, ho_Cross_CJLeft, ho_Cross_CJRight,
            ho_Rectangle_WhiteReduce, ho_RegionErosion_WhiteReduce, ho_ImageReduced_WhiteReduce, ho_Regions_WhiteReduce, 
            ho_ConnectedRegions_WhiteReduce, ho_SelectedRegions_WhiteReduce;
    HTuple  hv_AcqHandle, hv_WindowHandle, hv_Area_White, hv_Row_White, hv_Column_White, hv_Row_WhiteLeft, hv_Column_WhiteLeft,
            hv_Row_WhiteRight, hv_Column_WhiteRight, hv_Number, hv_Stop, hv_i, hv_Row_GJLeft, hv_Column_GJLeft, hv_Row_GJRight, 
            hv_Column_GJRight, hv_Row_WhiteReduce, hv_Column_WhiteReduce, hv_Phi_WhiteReduce, hv_Length1_WhiteReduce, 
            hv_Length2_WhiteReduce, hv_Number_WhiteReduce, hv_ModelID, hv_Tuple, hv_RomM, hv_ColM, hv_Row, hv_Column, hv_Angle, hv_Score;

    QTimer *timer;
    int MaxScore_Place;
};