#include "camera.h"
SINGLETON_GARBO(CCamera);
using namespace HalconCpp;
CCamera::CCamera()
{
}
CCamera::~CCamera()
{
}

bool CCamera::setPara(const QString &p_paraName, const HalconCpp::HTuple &p_value)
{
    try
    {
        SetFramegrabberParam(m_cameraHandle, p_paraName.toStdString().c_str(), p_value);
        return true;
    }
    catch (HException &except)
    {
        m_error = except.ErrorText().Text();
    }
    return false;
}

bool CCamera::openCamera(const QString &p_name)
{
    try
    {
        OpenFramegrabber("GigEVision2", 0, 0, 0, 0, 0, 0, "progressive", -1, "default",
                         -1, "false", "default", p_name.toStdString().c_str(), 0, -1, &m_cameraHandle);
        return true;
    }
    catch (HException &except)
    {
        m_error = except.ErrorText().Text();
    }
    return false;
}

bool CCamera::grabImage()
{
    try
    {
        GrabImage(&m_image, m_cameraHandle);
        return true;
    }
    catch (HException &except)
    {
        m_error = except.ErrorText().Text();
    }
    return false;
}

HObject CCamera::getLastImage() const
{
    return m_image;
}

QString CCamera::getLastError() const
{
    return m_error;
}

CCamera *camera()
{
    return CCamera::instance();
}