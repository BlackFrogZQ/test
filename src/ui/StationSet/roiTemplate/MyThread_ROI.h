#pragma once

#include "HalconCpp.h"
#include "HDevThread.h"
#include "../halconshare.h"

#include <QThread>
#include <QMetaType>

using namespace HalconCpp;

class MyThread_ROI : public QThread
{
    Q_OBJECT
public:
    MyThread_ROI();
    void startGenerateTemplate(const HObject& p_image,const CDrawRoiShapeType& p_drawShape, const CStationMode& p_stationMode);

    void savesROI(QString Station_Num, int ROI_StationNum);
    bool isSuccessful() const;
    QString getLastError() const;
    HObject getROIContours() const;

protected:
	void run() override;

protected:
    bool m_init;
    QString m_error;
    HObject m_template;
    HObject m_contoursImage;
};