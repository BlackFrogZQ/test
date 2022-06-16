#include "ROITool.h"

ROITool::ROITool(QWidget *parent)
    :   QWidget(parent),
        ui(new Ui::ROITool)
{
    ui->setupUi(this);
    ui->rBtn_ROIRectangle1->setChecked(true);

    QStringList MetricList;
    MetricList << cnStr("无")<< cnStr("交")<< cnStr("差");
    ui->comboBox_ROI->addItems(MetricList);
    ui->comboBox_ROI->setCurrentIndex(0);

    mT_ROI = new MyThread_ROI;
    thread_ROI = new QThread(this);
    mT_ROI->moveToThread(thread_ROI);

    connect(ui->btn_ROIGrab, &QPushButton::clicked, this, [this](){Obtain_ROIImage();});
    connect(ui->btn_ROIRenewImage, &QPushButton::clicked, this, [this](){Obtain_ROIImage();});
    connect(ui->btn_DrawROI, &QPushButton::clicked, this, [this](){DrawROI();});
    connect(this, &ROITool::Send_StartThread_ROI, mT_ROI, &MyThread_ROI::GenerateROITemplate, Qt::QueuedConnection);
    connect(mT_ROI, &MyThread_ROI::Send_ROIContours, this, &ROITool::Show_ROIContours);
    connect(ui->btn_ROISave, &QPushButton::clicked, this, [this]() {Saves_ROI();});
    connect(ui->btn_ROIQuit, &QPushButton::clicked, this, [this](){
        dealClose();
        close();});
    connect(this, &ROITool::destroyed, this, &ROITool::dealClose);
}

ROITool::~ROITool()
{
    delete ui;
}

void ROITool::Obtain_ROIImage()
{
    emit Send_ObtainROIImage();
}
void ROITool::Model_Show_ROIImage(const HObject &Image, QString CircularOrLine_Tem, QString Station_Num_Tem, int ROI_StationNum_Tem)
{
    CircularOrLine = CircularOrLine_Tem;
    Station_Num = Station_Num_Tem;
    ROI_StationNum = ROI_StationNum_Tem;

    ROI_Image = Image;
    if(Lock_UpdateROIImage == true){
        CloseWindow(ROI_WindowHandle);
    }
    update();
    long windowID = ui->widget_ROIImage->winId();
    OpenWindow(0, 0, ui->widget_ROIImage->width(), ui->widget_ROIImage->height(), windowID, "visible", "", &ROI_WindowHandle);
    GetImageSize(Image, &x, &y);
    SetPart(ROI_WindowHandle, 0, 0, y, x);
    DispObj(Image, ROI_WindowHandle);
    Lock_UpdateROIImage = true;
}

void ROITool::DrawROI()
{
    if(ui->rBtn_ROIRectangle1->isChecked()){
        rBtn_Shape = 1;
    }
    else if(ui->rBtn_ROIRectangle2->isChecked()){
        rBtn_Shape = 2;
    }
    else if(ui->rBtn_ROICircle->isChecked()){
        rBtn_Shape = 3;
    }
    else{
        rBtn_Shape = 4;
    }

    if(thread_ROI->isRunning() == true)
    {
        thread_ROI->quit();
        thread_ROI->wait();
    }
    thread_ROI->start();
    emit Send_StartThread_ROI(ROI_Image, ROI_WindowHandle, rBtn_Shape, CircularOrLine);
}
void ROITool::Show_ROIContours(const HObject &Contours_Image)
{
    if(Lock_ROIContours == true){
        CloseWindow(ROIContours_WindowHandle);
    }
    update();
    long windowID_Contours = ui->widget_ROIContours->winId();
    OpenWindow(0, 0, ui->widget_ROIContours->width(), ui->widget_ROIContours->height(), windowID_Contours, "visible", "", &ROIContours_WindowHandle);
    GetImageSize(ROI_Image, &x2, &y2);
    SetPart(ROIContours_WindowHandle, 0, 0, y2, x2);
    DispObj(Contours_Image, ROIContours_WindowHandle);
    Lock_ROIContours = true;
}

void ROITool::Saves_ROI()
{
    mT_ROI->mT_SavesROI(Station_Num, ROI_StationNum);
}

void ROITool::dealClose()
{
    if(thread_ROI->isRunning() == false)
    {
        return;
    }
    thread_ROI->terminate();
    thread_ROI->wait();
}