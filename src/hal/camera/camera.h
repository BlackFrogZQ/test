#pragma once
#include "HalconCpp.h"
#include <QObject>
#include "src/system/sigleClass.h"
class CCamera : public QObject
{
    Q_OBJECT
    SINGLETON(CCamera)

public:
    bool setPara(const QString& p_paraName,const HalconCpp::HTuple& p_value);
    bool openCamera(const QString& p_name = "0030533560c6_Basler_acA130060gc");
    bool grabImage();
    HalconCpp::HObject getLastImage() const;
    QString getLastError() const;

protected:
    QString m_error;
    HalconCpp::HObject m_image;
    HalconCpp::HTuple m_cameraHandle;
};
CCamera *camera();
