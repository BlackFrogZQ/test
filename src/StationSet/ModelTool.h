#pragma once
#include "ui_ModelTool.h"
#include "HalconCpp.h"
#include "HDevThread.h"
#include "halconshare.h"
#include "MyThread_Model.h"
#include <QWidget>
#include <QDebug>
#include <QStringList>
#define cnStr(str) QString::fromLocal8Bit(str)
using namespace HalconCpp;

class ModelTool : public QWidget  {
    Q_OBJECT

public:
    explicit ModelTool(QWidget* parent = nullptr);
    ~ModelTool();

public slots:
    void Obtain_Image();
    void Model_Show_Image(const HObject& Image, QString Station_Num_Tem);
    void DrawModel();
    void Show_ModelContours(const HObject& Contours_Image);
    void Saves_Model();
    void dealClose();

signals:
    void Send_ObtainImage();
    void Send_StartThread_Model(const HObject& M_obj, HTuple hv_WindowHandle, int rBtn_Shape, int Numberlever, int AngleStart, 
                    int AngleExtern, int AngleStep, int Contrast, int MinContrast, QString Optimization, QString Metric);

private:
    Ui::ModelTool *ui;
    HObject Model_Image;
    HTuple  x, y, WindowHandle, ModelContours_WindowHandle, x2, y2;

    MyThread_Model  *mT_Model;
    QThread *thread_Model;

    bool Lock_UpdateImage = false;
    bool Lock_ModelContours = false;
    int rBtn_Shape = 1;
    QString Station_Num;
};