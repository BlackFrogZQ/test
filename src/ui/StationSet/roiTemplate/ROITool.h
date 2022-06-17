#pragma once
#include "ui_ROITool.h"
#include "HalconCpp.h"
#include "HDevThread.h"
#include "../halconshare.h"
#include "roiDef.h"
#include "../StationSetDef.h"
using namespace HalconCpp;

class ROITool : public QDialog
{
    Q_OBJECT

public:
    explicit ROITool(QWidget *parent = nullptr);
    ~ROITool();

public slots:
    void Obtain_ROIImage();
    void Model_Show_ROIImage(const HObject &Image, CStationMode CircularOrLine_Tem, QString Station_Num_Tem, int ROI_StationNum_Tem);
    //无后缀
    void saveTemplate(const QString &p_name);

signals:
    void Send_ObtainROIImage();

protected slots:
    void drawRoi();

protected:
    void generateTemplate(const CDrawRoiShapeType &p_drawShape, const CStationMode &p_stationMode);

    void showHImage(QWidget *p_widget, const HObject &p_image);
    HTuple getWidgetHandle(QWidget *p_widget);
    void drawTemplate(const CDrawRoiShapeType &p_drawShape);
    void drawRectangle();
    void drawRectangle2();
    void drawCircular();
    void drawArbitrarily();

    void drawContours(const CStationMode &p_stationMode);
    void drawContoursCircular();
    void drawContoursLine();

private:
    Ui::ROITool *ui;
    HObject m_image;
    HObject m_template;
    HObject m_contoursImage;

    HTuple ROI_WindowHandle;
    CStationMode m_stationMode;
};