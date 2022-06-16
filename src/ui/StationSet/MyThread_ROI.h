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

class MyThread_ROI : public QObject
{
    Q_OBJECT
public:
    explicit MyThread_ROI(QObject *parent = nullptr);

public slots:
    void GenerateROITemplate(const HObject& M_obj, HTuple hv_WindowHandle, int rBtn_Shape, QString CircularOrLine);
    void mT_SavesROI(QString Station_Num, int ROI_StationNum);

signals:
    void Send_ROIContours(const HObject& p_obj);

protected:

private:
    HObject ho_Image, ho_GrayImage, ho_ROI_template, ho_ImageReduced_template, ho_ImageEmphasize, ho_ImageMedian, ho_Edges, 
            ho_UnionContours, ho_SelectedContours, ho_SelectedXLD, ho_SelectedXLD1, ho_ContCircle, ho_ImageEmphasize_Line, 
            ho_ImageMedian_Line, ho_Lines, ho_UnionContours_Line, ho_SelectedContours1_Line, ho_SelectedContours2_Line, 
            ho_RegionLines, ho_Contour_Line;
    HTuple  hv_AcqHandle, hv_Width, hv_Height, hv_WindowHandle, hv_Row_Rectangle11, hv_Column_Rectangle11, hv_Row_Rectangle12, 
            hv_Column_Rectangle12, hv_Row_Rectangle21, hv_Column_Rectangle21, hv_Phi_Rectangle2, hv_Length_Rectangle21, hv_Length_Rectangle22, 
            hv_Row_Circle, hv_Column_Circle, hv_Radius_Circle, hv_Circularity, hv_Number, hv_CIRCULARITY, hv_Index, hv_Number1, hv_Row, hv_Column, 
            hv_Radius, hv_StartPhi, hv_EndPhi, hv_PointOrder, hv_Length_Line, hv_Number1_Line, hv_Length_max, hv_Index_Line, hv_Number2_Line, 
            hv_RowBegin, hv_ColBegin, hv_RowEnd, hv_ColEnd, hv_Nr, hv_Nc, hv_Dist;
};