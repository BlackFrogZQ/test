#include "Mainscene.h"
#include "SwitchUsers/SwitchUsers.h"

Mainscene::Mainscene(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_Mainscene)
{
    ui->setupUi(this);
    initAction();
    qRegisterMetaType<HObject>("HObject");
    qDebug() << cnStr("主线程号：") << QThread::currentThread();

    //状态栏
    ui->statusbar->setSizeGripEnabled(false);
    ui->statusbar->setStyleSheet(QString("QStatusBar::item{border: 0px}"));

    QLabel *label_statubar = new QLabel(QString::fromLocal8Bit("提示信息："), this);
    QLabel *label_statubar_camera = new QLabel(this);
    QLabel *label_time = new QLabel(this);

    label_statubar_camera->setText(QStringLiteral("请打开相机!"));

    ui->statusbar->addWidget(label_statubar);
    ui->statusbar->addWidget(label_statubar_camera);

    QPalette red;
    red.setColor(QPalette::WindowText, Qt::red);
    label_statubar_camera->setPalette(red);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]()
            { timerUpdate(label_time); });
    timer->start(1000);
    ui->statusbar->addPermanentWidget(label_time);

    //其余控件
    ui->btn_Measure->setEnabled(false);

    /*******************实例化类对象*********************/
    myT_camera = new MyThread;
    thread = new QThread(this);
    myT_camera->moveToThread(thread);

    myG_Discern = new MyThread_Discern;
    thread_GJ = new QThread(this);
    myG_Discern->moveToThread(thread_GJ);

    myG_DisOneCircle = new MyThread_DisOneCircle;
    thread_DisOneCircle = new QThread(this);
    myG_DisOneCircle->moveToThread(thread_DisOneCircle);

    mEngDocument = NULL;
    mEngDocument = new EngDocument;

    mStationSet = NULL;
    mStationSet = new CSetStationDialog;

    mModelTool = NULL;
    mModelTool = new ModelTool;

    mCameraParams = NULL;
    mCameraParams = new CameraParams;

    mLightSet = NULL;
    mLightSet = new LightSet;

    connect(mStationSet, &CSetStationDialog::Send_ShowModelTool, this, &Mainscene::Show_ModelTool); //给创建ModelTool窗口及向模板界面发送图像
    connect(mModelTool, &ModelTool::Send_ObtainImage, this, &Mainscene::TransferImage);      //给创建ModelTool窗口及向模板界面发送图像



    connect(ui->actionCameraParameters, &QAction::triggered, this, [=]()
            { slot_CameraParams(); });

    connect(mCameraParams, &CameraParams::Send_CameraParams, this, &Mainscene::Change_CameraParams);

    connect(ui->actionLight, &QAction::triggered, this, [=]()
            { slot_LightSource(); });

    //打开相机部分
    connect(ui->btn_OpenCamera, &QPushButton::clicked, this, [=]()
            {
        //打开相机
        if(Lock_Camera == false){
            if(Lock_AnewOpenCamera == true){
                thread->quit();
                thread->wait();
                CloseFramegrabber(AcqHandle);
                CloseWindow(WindowHandle);
                Lock_AnewOpenCamera = false;
            }
            Open_Camera(ui->widget_Image);
            Lock_Camera = true;
            Lock_CameraFirstQuit = true;

            ui->btn_Measure->setEnabled(true);
            ui->btn_OpenCamera->setText(cnStr("关闭相机"));
            QPalette black;
            black.setColor(QPalette::WindowText,Qt::black);
            label_statubar_camera->setPalette(black);
            label_statubar_camera->setText(QStringLiteral("相机打开成功!"));
        }
        //关闭相机
        else{
            myT_camera->Time_Stop();
            Lock_Camera = false;

            ui->btn_OpenCamera->setText(cnStr("打开相机"));
            QPalette black;
            black.setColor(QPalette::WindowText,Qt::black);
            label_statubar_camera->setPalette(black);
            label_statubar_camera->setText(QStringLiteral("相机关闭成功!"));
        } });
    //打开相机持续采图
    connect(this, &Mainscene::startThread, myT_camera, [=]()
            { myT_camera->Start_Time(AcqHandle, WindowHandle); });
    //关闭相机持续采图
    connect(myT_camera, &MyThread::stop_thread, this, [=]()
            { Lock_AnewOpenCamera = true; });
    //实时在窗口更新图像
    connect(myT_camera, &MyThread::stary_thread, this, &Mainscene::Show_Image, Qt::QueuedConnection);

    //工件识别
    connect(ui->btn_Measure, &QPushButton::clicked, this, [=]()
            {
        //开始识别
        if(Lock_GJCamera == false){
            if(Lock_AnewOpenGJCamera == true){
                thread_GJ->quit();
                thread_GJ->wait();
                Lock_AnewOpenGJCamera = false;
            }
            Open_DiscernGJ();
            Lock_GJCamera = true;
            Lock_CameraFirstQuitGJ = true;
            Lock_ShowMainThreadImage = true;
            ui->btn_Measure->setText(cnStr("停止识别"));
        }
        //停止识别
        else{
            myG_Discern->Time_GJStop();
            Lock_GJCamera = false;
            Lock_ShowMainThreadImage = false;
            ui->btn_Measure->setText(cnStr("开始识别"));
        } });
    //开始识别
    connect(this, &Mainscene::startGJThread, myG_Discern, [=]()
            { myG_Discern->Open_GJTime(); });
    //停止识别
    connect(myG_Discern, &MyThread_Discern::Stop_thread, myG_Discern, [=]()
            { Lock_AnewOpenGJCamera = true; });

    //工件*就位*的线程发送过来的信号*停止*在窗口显示主线程的图像
    connect(myG_Discern, &MyThread_Discern::Send_CameraStop, this, &Mainscene::TransportMaxScore, Qt::QueuedConnection);
    connect(myG_Discern, &MyThread_Discern::Send_CameraStop, this, [=]()
            {
        i += 1;
        if(i == 1){
            Lock_ShowMainThreadImage = true;
        } });
    //工件*没有就位*的线程发送过来的信号*重新*在窗口显示主线程的图像
    connect(myG_Discern, &MyThread_Discern::Send_CameraStart, this, [=]()
            {
        i = 0;
        Lock_ShowMainThreadImage = false; });
    //接收最高分数的工位属性，并开始匹配
    connect(mStationSet, &CSetStationDialog::Send_MScoreAbuteName, this, &Mainscene::OpenDiscernThread);

    //圆定位
    connect(this, &Mainscene::start_DisOneCircle, myG_DisOneCircle, &MyThread_DisOneCircle::Start_DisOneCircle, Qt::QueuedConnection);
    connect(myG_DisOneCircle, &MyThread_DisOneCircle::Send_FinishDisOneCircle, this, &Mainscene::Show_DisOneCircle, Qt::QueuedConnection);

    //关闭窗口及线程
    connect(this, &Mainscene::destroyed, this, &Mainscene::dealClose);
}

Mainscene::~Mainscene()
{
    dealClose();
    delPtr(mEngDocument);
    delPtr(mStationSet);
    delPtr(mModelTool);
    delPtr(mCameraParams);
    delPtr(mCameraParams);
    delPtr(mLightSet);
    delPtr(ui);
}

//状态栏时间每秒刷新一次
void Mainscene::timerUpdate(QLabel *label_time)
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    label_time->setText(str);
}

//打开登录界面窗口
void Mainscene::slotSwitchUsers()
{
    CSwitchUsersDialog switchUsersDialog;
    if (switchUsersDialog.login())
    {
        qDebug() << cnStr("登录成功类型:") << switchUsersDialog.getUserType();
        //根据返回切换权限；
    }
}

//打开工程文档窗口
void Mainscene::slot_EngDocument()
{
    mEngDocument->setWindowModality(Qt::ApplicationModal); //设置为模态对话框
    mEngDocument->show();
}

void Mainscene::obtain_EngDocument(int EngDoc_CurrRow, QString EngDoc_CurrName)
{
    this->setWindowTitle(EngDoc_CurrName);
    Current_EngDoc = EngDoc_CurrRow;
}

//打开工位设置窗口
void Mainscene::slot_StationSet()
{
    mStationSet->exec();
}

//显示ModelTool窗口
void Mainscene::Show_ModelTool(QString Station_Num)
{
    Station_Num_Tem = Station_Num;
    mModelTool->exec();
}

//发送图像给模板界面
void Mainscene::TransferImage()
{
    mModelTool->Model_Show_Image(hv_Image, Station_Num_Tem);
}


//打开设置相机参数窗口
void Mainscene::slot_CameraParams()
{
    mCameraParams->show();
}

void Mainscene::Change_CameraParams(int Gain, int Exposure)
{
    GainRaw = Gain;
    ExposureTime = Exposure;
}

//打开设置光源参数窗口
void Mainscene::slot_LightSource()
{
    mLightSet->show();
}

//打开相机
void Mainscene::Open_Camera(QWidget *widget)
{
    if (Lock_AnewOpenCamera == false)
    {
        OpenFramegrabber("GigEVision2", 0, 0, 0, 0, 0, 0, "progressive", -1, "default",
                         -1, "false", "default", "0030533560c6_Basler_acA130060gc", 0, -1, &AcqHandle); //打开相机

        SetFramegrabberParam(AcqHandle, "GainAuto", "Off");
        SetFramegrabberParam(AcqHandle, "GainSelector", "All");
        SetFramegrabberParam(AcqHandle, "GainRaw", GainRaw);
        SetFramegrabberParam(AcqHandle, "BlackLevelSelector", "All");
        SetFramegrabberParam(AcqHandle, "BlackLevelRaw", 19);
        SetFramegrabberParam(AcqHandle, "ExposureTimeRaw", ExposureTime);

        long windowID = widget->winId();
        OpenWindow(0, 0, widget->width(), widget->height(), windowID, "visible", "", &WindowHandle); //绑定窗口
    }

    if (thread->isRunning() == true)
    {
        thread->quit();
        thread->wait();
    }
    thread->start();
    emit startThread();
}

//在窗口上更新并显示图像
void Mainscene::Show_Image(const HObject &Image)
{
    if (Lock_ShowMainThreadImage == false)
    {
        SetFramegrabberParam(AcqHandle, "GainAuto", "Off");
        SetFramegrabberParam(AcqHandle, "GainSelector", "All");
        SetFramegrabberParam(AcqHandle, "GainRaw", GainRaw);
        SetFramegrabberParam(AcqHandle, "BlackLevelSelector", "All");
        SetFramegrabberParam(AcqHandle, "BlackLevelRaw", 19);
        SetFramegrabberParam(AcqHandle, "ExposureTimeRaw", ExposureTime);
        GetImageSize(Image, &x, &y);
        SetPart(WindowHandle, 0, 0, y, x);
        DispObj(Image, WindowHandle);
    }

    hv_Image = Image;
    myG_Discern->Update_Image(Image);
    // qDebug()<<cnStr("主线程号：")<<QThread::currentThread();
}

//识别工件
void Mainscene::Open_DiscernGJ()
{
    if (thread_GJ->isRunning() == true)
    {
        thread_GJ->quit();
        thread_GJ->wait();
    }
    thread_GJ->start();
    emit startGJThread();
}

//把最大匹配分数的位置发送给工位编辑
void Mainscene::TransportMaxScore(int MaxScore_Place)
{
    mStationSet->ObtainStationAbute(MaxScore_Place);
}

//打开工件识别线程
void Mainscene::OpenDiscernThread(QString MScoreAbuteName, int MaxScore_Place, int WorkStep_Sum)
{
    if (MScoreAbuteName == cnStr("圆定位"))
    {
        if (thread_DisOneCircle->isRunning() == true)
        {
            thread_DisOneCircle->quit();
            thread_DisOneCircle->wait();
        }
        thread_DisOneCircle->start();
        emit start_DisOneCircle(hv_Image, WindowHandle, MaxScore_Place);
    }
    else if (MScoreAbuteName == cnStr("多组圆定位"))
    {
    }
    else if (MScoreAbuteName == cnStr("一线交两边定位"))
    {
    }
}

//显示圆点定位的图像
void Mainscene::Show_DisOneCircle(const HObject &ho_GrayImage, const HTuple &hv_Row2, const HTuple &hv_Column2, const HObject &ho_Cross, const HObject &ho_SelectedContours)
{
    if (Lock_ShowMainThreadImage == true)
    {
        //显示黄色轮廓
        SetColor(WindowHandle, "yellow");
        DispObj(hv_Image, WindowHandle);
        DispObj(ho_SelectedContours, WindowHandle);
        DispObj(ho_Cross, WindowHandle);

        //显示红色圆心
        SetColor(WindowHandle, "red");
        DispCircle(WindowHandle, hv_Row2, hv_Column2, 5);

        //显示蓝色坐标
        SetColor(WindowHandle, "blue");
        SetTposition(WindowHandle, hv_Row2 - 200, hv_Column2 - 200);
        WriteString(WindowHandle, QString::fromLocal8Bit("圆心为:(").toStdString().data() + hv_Column2 + "," + hv_Row2 + ")");
    }
}

//关闭窗口的同时关闭所有的线程
void Mainscene::dealClose()
{
    //关闭相机采图线程
    if (Lock_AnewOpenCamera == false && Lock_CameraFirstQuit == true)
    {
        myT_camera->Time_Stop();
    }
    if (thread->isRunning() == false)
    {
        return;
    }
    thread->terminate();
    thread->wait();

    //关闭判断工件是否到位线程
    if (Lock_AnewOpenGJCamera == false && Lock_CameraFirstQuitGJ == true)
    {
        myG_Discern->Time_GJStop();
    }
    if (thread_GJ->isRunning() == false)
    {
        return;
    }
    thread_GJ->terminate();
    thread_GJ->wait();

    //关闭圆点定位线程
    if (thread_DisOneCircle->isRunning() == false)
    {
        return;
    }
    thread_DisOneCircle->terminate();
    thread_DisOneCircle->wait();
}

void Mainscene::initAction()
{
    connect(ui->actionQuit, &QAction::triggered, this, [this]()
            { close(); });
    connect(ui->actionSwitchUsers, &QAction::triggered, this, &Mainscene::slotSwitchUsers);

    connect(ui->actionEngDoc, &QAction::triggered, this, [=]()
            { slot_EngDocument(); });

    connect(ui->actionStationSet, &QAction::triggered, this, [=]()
            { slot_StationSet(); });
}
