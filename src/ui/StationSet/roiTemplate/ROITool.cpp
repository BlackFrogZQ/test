#include "ROITool.h"
#include "src/hal/camera/camera.h"
CRoiToolDialog::CRoiToolDialog(QWidget *parent)
    : QDialog(parent),
      ROI_WindowHandle(-1),
      ROIContour_WindowHandle(-1),
      ui(new Ui::ROITool)
{
    ui->setupUi(this);
    ui->rBtn_ROIRectangle1->setChecked(true);
    ui->comboBox_ROI->addItems(QStringList() << cnStr("无") << cnStr("交") << cnStr("差"));
    ui->comboBox_ROI->setCurrentIndex(0);

    connect(ui->btn_ROIGrab, &QPushButton::clicked, this, &CRoiToolDialog::slotGrabImage);
    connect(ui->btn_ROIRenewImage, &QPushButton::clicked, this, &CRoiToolDialog::slotGrabImage);

    connect(ui->btn_DrawROI, &QPushButton::clicked, this, &CRoiToolDialog::slotDrawRoi);

    connect(ui->btn_ROISave, &QPushButton::clicked, this, [this]()
            {
                m_isSave = true;
                close(); });
    connect(ui->btn_ROIQuit, &QPushButton::clicked, this, [this]()
            { close(); });
}

CRoiToolDialog::~CRoiToolDialog()
{
    delPtr(ui);
}

bool CRoiToolDialog::isSaveStationWorkingProcedure(CStationWorkingProcedure *const p_pWorkingProcedure)
{
    m_isSave = false;
    m_pStationWorkingProcedure = p_pWorkingProcedure;
    assert(m_pStationWorkingProcedure != nullptr);
    this->exec();
    return m_isSave;
}

void CRoiToolDialog::slotDrawRoi()
{
    CDrawRoiShapeType shape;
    if (ui->rBtn_ROIRectangle1->isChecked())
    {
        shape = cdrstRectangle;
    }
    else if (ui->rBtn_ROIRectangle2->isChecked())
    {
        shape = cdrstRectangle2;
    }
    else if (ui->rBtn_ROICircle->isChecked())
    {
        shape = cdrstCircular;
    }
    else
    {
        shape = cdrstArbitrarily;
    }
    try
    {
        generateTemplate(shape, m_pStationWorkingProcedure->mode);
        showHImage(ROIContour_WindowHandle, m_contoursImage);
    }
    catch (HException &except)
    {
        warningBox(this, "CRoiToolDialog error", except.ErrorText().Text());
    }
}

void CRoiToolDialog::slotGrabImage()
{
    if (!camera()->grabImage())
    {
        warningBox(this, "error", cnStr("ROITool grabImage error:%1").arg(camera()->getLastError()));
        return;
    }
    m_image = camera()->getLastImage();
    setWidgetHandle();
    DispObj(m_image,ROI_WindowHandle);
    // showHImage(ROI_WindowHandle, m_image);
}

void CRoiToolDialog::generateTemplate(const CDrawRoiShapeType &p_drawShape, const CWorkingProcedureMode &p_stationMode)
{
    drawTemplate(p_drawShape);
    drawContours(p_stationMode);
}

void CRoiToolDialog::showHImage(HTuple p_windowHandle, const HObject &p_image)
{
    try
    {
        HTuple w, h;
        GetImageSize(p_image, &w, &h);
        SetPart(p_windowHandle, 0, 0, w, h);
        DispObj(p_image, p_windowHandle);
        this->update();
    }
    catch (HException &except)
    {
        warningBox(this, "CRoiToolDialog error", except.ErrorText().Text());
    }
}

void CRoiToolDialog::setWidgetHandle()
{
    try
    {
        QWidget *p_widget = ui->widget_ROIImage;
        HDevWindowStack::Push(ROI_WindowHandle);
        if (HDevWindowStack::IsOpen())
        {
            CloseWindow(ROI_WindowHandle);
            ROI_WindowHandle = -1;
        }
        HDevWindowStack::Pop();
        OpenWindow(0, 0, p_widget->width(), p_widget->height(), HTuple(long(p_widget->winId())), "visible", "", &ROI_WindowHandle);

        p_widget = ui->widget_ROIContours;
        HDevWindowStack::Push(ROIContour_WindowHandle);
        if (HDevWindowStack::IsOpen())
        {
            CloseWindow(ROIContour_WindowHandle);
            ROIContour_WindowHandle = -1;
        }
        HDevWindowStack::Pop();
        OpenWindow(0, 0, p_widget->width(), p_widget->height(), HTuple(long(p_widget->winId())), "visible", "", &ROIContour_WindowHandle);
    }
    catch (HException &except)
    {
        QWidget *p_widget = ui->widget_ROIImage;
        if (ROI_WindowHandle == -1)
        {
            OpenWindow(0, 0, p_widget->width(), p_widget->height(), HTuple(long(p_widget->winId())), "visible", "", &ROI_WindowHandle);
        }
        p_widget = ui->widget_ROIContours;
        if (ROIContour_WindowHandle == -1)
        {
            OpenWindow(0, 0, p_widget->width(), p_widget->height(), HTuple(long(p_widget->winId())), "visible", "", &ROIContour_WindowHandle);
        }
    }
}

void CRoiToolDialog::saveTemplate(const QString &p_stationDir)
{
    try
    {
        //暂时支持csmLine、csmCircular，其他的需要检测
        assert(m_pStationWorkingProcedure->mode == csmLine ||
               m_pStationWorkingProcedure->mode == csmCircular);
        WriteRegion(m_template,
                    (p_stationDir + "/" +
                     m_pStationWorkingProcedure->getDirName() + "/" +
                     m_pStationWorkingProcedure->getRoiTemplateName())
                        .toStdString()
                        .c_str());
    }
    catch (HException &except)
    {
        m_isSave = false;
        warningBox(this, "CRoiToolDialog error", except.ErrorText().Text());
    }
}