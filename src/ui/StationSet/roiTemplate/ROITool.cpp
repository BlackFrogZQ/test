#include "ROITool.h"
#include "src/hal/camera/camera.h"
CRoiToolDialog::CRoiToolDialog(QWidget *parent)
    : QDialog(parent),
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
                //todo 工程管理类
                //saveTemplate(); 
            });
    connect(ui->btn_ROIQuit, &QPushButton::clicked, this, [this]()
            { close(); });
}

CRoiToolDialog::~CRoiToolDialog()
{
    delPtr(ui);
}

void CRoiToolDialog::setStationMode(CWorkingProcedureMode CircularOrLine_Tem)
{
    m_stationMode = CircularOrLine_Tem;
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
    generateTemplate(shape, m_stationMode);
    showHImage(ui->widget_ROIContours, m_contoursImage);
}

void CRoiToolDialog::slotGrabImage()
{
    if (!camera()->grabImage())
    {
        warningBox(this, "error", cnStr("ROITool grabImage error:%1").arg(camera()->getLastError()));
        return;
    }
    m_image = camera()->getLastImage();
    ROI_WindowHandle = getWidgetHandle(ui->widget_ROIImage);
}

void CRoiToolDialog::generateTemplate(const CDrawRoiShapeType &p_drawShape, const CWorkingProcedureMode &p_stationMode)
{
    drawTemplate(p_drawShape);
    drawContours(p_stationMode);
}

void CRoiToolDialog::showHImage(QWidget *p_widget, const HObject &p_image)
{
    HTuple windowHandle = getWidgetHandle(p_widget), w, h;
    GetImageSize(p_image, &w, &h);
    SetPart(windowHandle, 0, 0, w, h);
    DispObj(p_image, windowHandle);
}

HTuple CRoiToolDialog::getWidgetHandle(QWidget *p_widget)
{
    HTuple windowHandle;
    OpenWindow(0, 0, p_widget->width(), p_widget->height(), long(p_widget->winId()), "visible", "", &windowHandle);
    return windowHandle;
}

void CRoiToolDialog::saveTemplate(const QString &p_name)
{
    WriteRegion(m_template, (p_name + ".hobj").toStdString().c_str());
}