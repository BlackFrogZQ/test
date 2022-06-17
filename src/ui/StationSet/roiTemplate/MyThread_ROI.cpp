#include "MyThread_ROI.h"
#include <QTimer>

MyThread_ROI::MyThread_ROI()
: m_init(false)
{
    qRegisterMetaType<HObject>("HObject");
    qRegisterMetaType<HTuple>("HTuple");
}



void MyThread_ROI::mT_SavesROI(QString Station_Num, int ROI_StationNum)
{
    QString path_current = QDir::currentPath();
    QString add_current = "/src/StationSet/SaveModel/";
    path_current += add_current;

    HTuple hTuple_region, hv_Station_Num, hv_path_current, hv_ROI_StationNum;
    hv_Station_Num[3] = Station_Num.toStdString().c_str();
    hv_path_current[3] = path_current.toStdString().c_str();

    hTuple_region = (hv_path_current[3]+"region_" + hv_Station_Num[3] + ROI_StationNum) + ".hobj";
    WriteRegion(ho_ROI_template, hTuple_region);
}