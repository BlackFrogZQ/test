#include "MyThread_Discern.h"

MyThread_Discern::MyThread_Discern(QObject *parent)
: QObject(parent)
{
    qRegisterMetaType<HObject>("HObject");
    qRegisterMetaType<HTuple>("HTuple");
}

void MyThread_Discern::Open_GJTime()
{
    timer = new QTimer();
    int k = 20;
    timer->start(k);
    connect(timer, &QTimer::timeout, this, &MyThread_Discern::Start_Discern);
}

//根据时钟跳动识别工件
void MyThread_Discern::Start_Discern()
{
    Rgb1ToGray(ho_Image, &ho_GrayImage);
    
    double MaxScore = 0;
    for(int i = 0; i < 20 ; i++){
        QString path_current = QDir::currentPath();
        QString add_current = "/src/StationSet/SaveModel/";
        path_current += add_current;

        HTuple hTuple_ShapeModel, hv_Station_Num, hv_path_current;
        hv_Station_Num[3] = i;
        hv_path_current[3] = path_current.toStdString().c_str();
        hTuple_ShapeModel = (hv_path_current[3]+"ShapeModel_" + hv_Station_Num[3]) + ".smd.shm";
        try{
            ReadShapeModel(hTuple_ShapeModel, &hv_ModelID);
        }
        catch (...){
            break;
        }

        FindShapeModel(ho_GrayImage, hv_ModelID, 0, 360, 0.5, 1, 0.5, "least_squares", 0, 0.9, &hv_Row, &hv_Column, &hv_Angle, &hv_Score);
        try{
            double str = hv_Score[0].D();//没有模板时就没有hv_Score，这句话会报错
        }
        catch (...){
            continue;
        }
        if(hv_Score > MaxScore){
            MaxScore = hv_Score;
            MaxScore_Place = i;
        }
    }

    if(MaxScore >= 0.5){
        emit Send_CameraStop(MaxScore_Place);
    }
    else{
        emit Send_CameraStart();
    }

    // qDebug()<<cnStr("工件识别线程号：")<<QThread::currentThread();
}

//停止线程
void MyThread_Discern::Time_GJStop()
{
    timer->stop();
    emit Stop_thread();
}

//获取窗口显示的图片
void MyThread_Discern::Update_Image(HObject Image)
{
    ho_Image = Image;
}