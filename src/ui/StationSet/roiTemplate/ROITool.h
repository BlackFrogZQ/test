#pragma once
#include "ui_ROITool.h"
#include "HalconCpp.h"
#include "HDevThread.h"
#include "../halconshare.h"
#include "roiDef.h"
#include "src/system/projectManager/StationSetDef.h"
using namespace HalconCpp;

class CRoiToolDialog : public QDialog
{
    Q_OBJECT

public:
    CRoiToolDialog(QWidget *parent = nullptr);
    ~CRoiToolDialog();
    bool isSaveStationWorkingProcedure(CStationWorkingProcedure* const p_pWorkingProcedure);
    void saveTemplate(const QString &p_stationDir);

protected slots:
    void slotDrawRoi();
    void slotGrabImage();

protected:
    void generateTemplate(const CDrawRoiShapeType &p_drawShape, const CWorkingProcedureMode &p_stationMode);

    void showHImage(HTuple p_windowHandle, const HObject &p_image);
    void setWidgetHandle();
    void drawTemplate(const CDrawRoiShapeType &p_drawShape);
    void drawRectangle();
    void drawRectangle2();
    void drawCircular();
    void drawArbitrarily();

    void drawContours(const CWorkingProcedureMode &p_stationMode);
    void drawContoursCircular();
    void drawContoursLine();
private:
    Ui::ROITool *ui;
    HObject m_image;
    HObject m_template;
    HObject m_contoursImage;

    HTuple ROI_WindowHandle;
    HTuple ROIContour_WindowHandle;
    CStationWorkingProcedure* m_pStationWorkingProcedure;
    bool m_isSave;
};