#include "ModelTool.h"

ModelTool::ModelTool(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ModelTool)
{
    ui->setupUi(this);
    ui->rBtn_Rectangle1->setChecked(true);

    mT_Model = new MyThread_Model;
    thread_Model = new QThread(this);
    mT_Model->moveToThread(thread_Model);

    //优化算法
    QStringList OptimizationList;
    OptimizationList << "auto"
                     << "none"
                     << "point_reduction_low"
                     << "point_reduction_medium"
                     << "point_reduction_high";
    ui->ComboBox_Optimization->addItems(OptimizationList);
    ui->ComboBox_Optimization->setCurrentIndex(0);
    //极性
    QStringList MetricList;
    MetricList << "use_polarity"
               << "ignore_global_polarity"
               << "ignore_local_polarity"
               << "ignore_color_polarity";
    ui->ComboBox_Metric->addItems(MetricList);
    ui->ComboBox_Metric->setCurrentIndex(0);

    connect(ui->btn_ModelGrab, &QPushButton::clicked, this, [this]()
            { Obtain_Image(); });
    connect(ui->btn_RenewImage, &QPushButton::clicked, this, [this]()
            { Obtain_Image(); });
    connect(ui->btn_DrawModel, &QPushButton::clicked, this, [this]()
            { DrawModel(); });
    connect(this, &ModelTool::Send_StartThread_Model, mT_Model, &MyThread_Model::GenerateTemplate, Qt::QueuedConnection);
    connect(mT_Model, &MyThread_Model::Send_ModelContours, this, &ModelTool::Show_ModelContours);
    connect(ui->btn_ModelSave, &QPushButton::clicked, this, [this]()
            { Saves_Model(); });
    connect(ui->btn_ModelQuit, &QPushButton::clicked, this, [this]()
            {
        dealClose();
        close(); });
    connect(this, &ModelTool::destroyed, this, &ModelTool::dealClose);
}

ModelTool::~ModelTool()
{
    delete ui;
}

void ModelTool::Obtain_Image()
{
    emit Send_ObtainImage();
}
void ModelTool::Model_Show_Image(const HObject &Image, QString Station_Num_Tem)
{
    Model_Image = Image;
    Station_Num = Station_Num_Tem;
    if (Lock_UpdateImage == true)
    {
        CloseWindow(WindowHandle);
    }
    update();
    long windowID = ui->widget_ModelImage->winId();
    OpenWindow(0, 0, ui->widget_ModelImage->width(), ui->widget_ModelImage->height(), windowID, "visible", "", &WindowHandle);
    GetImageSize(Image, &x, &y);
    SetPart(WindowHandle, 0, 0, y, x);
    DispObj(Image, WindowHandle);
    Lock_UpdateImage = true;
}

void ModelTool::DrawModel()
{
    int Numberlever = ui->SpinBox_Numberlever->value();
    int AngleStart = ui->SpinBox_AngleStart->value();
    int AngleExtern = ui->SpinBox_AngleExtern->value();
    int AngleStep = ui->SpinBox_AngleStep->value();
    int Contrast = ui->SpinBox_Contrast->value();
    int MinContrast = ui->SpinBox_MinContrast->value();
    QString Optimization = ui->ComboBox_Optimization->currentText();
    QString Metric = ui->ComboBox_Metric->currentText();

    if (ui->rBtn_Rectangle1->isChecked())
    {
        rBtn_Shape = 1;
    }
    else if (ui->rBtn_Rectangle2->isChecked())
    {
        rBtn_Shape = 2;
    }
    else if (ui->rBtn_Circle->isChecked())
    {
        rBtn_Shape = 3;
    }
    else
    {
        rBtn_Shape = 4;
    }

    if (thread_Model->isRunning() == true)
    {
        thread_Model->quit();
        thread_Model->wait();
    }
    thread_Model->start();
    emit Send_StartThread_Model(Model_Image, WindowHandle, rBtn_Shape, Numberlever, AngleStart, AngleExtern, AngleStep,
                                Contrast, MinContrast, Optimization, Metric);
}
void ModelTool::Show_ModelContours(const HObject &Contours_Image)
{
    if (Lock_ModelContours == true)
    {
        CloseWindow(ModelContours_WindowHandle);
    }
    update();
    long windowID_Contours = ui->widget_ModelContours->winId();
    OpenWindow(0, 0, ui->widget_ModelContours->width(), ui->widget_ModelContours->height(), windowID_Contours, "visible", "", &ModelContours_WindowHandle);
    GetImageSize(Model_Image, &x2, &y2);
    SetPart(ModelContours_WindowHandle, 0, 0, y2, x2);
    DispObj(Contours_Image, ModelContours_WindowHandle);
    Lock_ModelContours = true;
}

void ModelTool::Saves_Model()
{
    mT_Model->mT_SavesModel(Station_Num);
}

void ModelTool::dealClose()
{
    if (thread_Model->isRunning() == false)
    {
        return;
    }
    thread_Model->terminate();
    thread_Model->wait();
}