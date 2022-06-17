#include "ROITool.h"

ROITool::ROITool(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ROITool)
{
    ui->setupUi(this);
    ui->rBtn_ROIRectangle1->setChecked(true);
    ui->comboBox_ROI->addItems(QStringList() << cnStr("无") << cnStr("交") << cnStr("差"));
    ui->comboBox_ROI->setCurrentIndex(0);

    connect(ui->btn_ROIGrab, &QPushButton::clicked, this, [this]()
            { 
                //todo 采图相机 
                });
    connect(ui->btn_ROIRenewImage, &QPushButton::clicked, this, [this]() 
    {

    });

    connect(ui->btn_DrawROI, &QPushButton::clicked, this, [this]()
            { DrawROI(); });

    connect(ui->btn_ROISave, &QPushButton::clicked, this, [this]()
            { 
                //todo 工程管理类
                saveTemplate(); });
    connect(ui->btn_ROIQuit, &QPushButton::clicked, this, [this]()
            { close(); });
}

ROITool::~ROITool()
{
    delete ui;
}

void ROITool::Obtain_ROIImage()
{
    emit Send_ObtainROIImage();
}

void ROITool::Model_Show_ROIImage(const HObject &Image, CStationMode CircularOrLine_Tem, QString Station_Num_Tem, int ROI_StationNum_Tem)
{
    m_stationMode = CircularOrLine_Tem;
    m_image = Image;
    ROI_WindowHandle = getWidgetHandle(ui->widget_ROIImage);
}

void ROITool::DrawROI()
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

void ROITool::generateTemplate(const CDrawRoiShapeType &p_drawShape, const CStationMode &p_stationMode)
{
    drawTemplate(p_drawShape);
    drawContours(p_stationMode);
}

void ROITool::showHImage(QWidget *p_widget, const HObject &p_image)
{
    HTuple windowHandle = getWidgetHandle(p_widget), w, h;
    GetImageSize(p_image, &w, &h);
    SetPart(windowHandle, 0, 0, w, h);
    DispObj(p_image, windowHandle);
}

HTuple ROITool::getWidgetHandle(QWidget *p_widget)
{
    HTuple windowHandle;
    OpenWindow(0, 0, p_widget->width(), p_widget->height(), long(p_widget->winId()), "visible", "", &windowHandle);
    return windowHandle;
}

void ROITool::saveTemplate(const QString &p_name)
{
    WriteRegion(m_template, (p_name + ".hobj").toStdString().c_str());
}