#pragma once
#include "ui_Mainscene.h"
#include <QMainWindow>
#include "basic.h"

#include <QTimer>
#include "HalconCpp.h"
#include "HDevThread.h"
#include "MyThread.h"
#include "MyThread_Discern.h"
#include "MyThread_DisOneCircle.h"
#include "EngDocument/EngDocument.h"
#include "StationSet/halconshare.h"
#include "StationSet/StationSet.h"
#include "StationSet/ModelTool.h"
#include "StationSet/roiTemplate/ROITool.h"
#include "Camera/CameraParams.h"
#include "LightSource/LightSet.h"

#include <QMetaType>
#include <QTimer>
#include <QLabel>
#include <QDateTime>
#include <QPalette>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QSerialPortInfo>
#include <QTextBrowser>
#include <QDebug>

using namespace HalconCpp;

class Mainscene : public QMainWindow
{
    Q_OBJECT

public:
    Mainscene(QWidget *parent = nullptr);
    ~Mainscene();

public slots:
    void timerUpdate(QLabel *label_time);

    void slot_EngDocument();
    void obtain_EngDocument(int EngDoc_CurrRow, QString EngDoc_CurrName);

    void slot_StationSet();
    void Show_ModelTool(QString Station_Num);
    void TransferImage();
    void slot_CameraParams();
    void Change_CameraParams(int Gain, int Exposure);

    void slot_LightSource();

    void Open_Camera(QWidget *widget);
    void Show_Image(const HObject &Image);

    void Open_DiscernGJ();
    void TransportMaxScore(int MaxScore_Place);
    void OpenDiscernThread(QString MScoreAbuteName, int MaxScore_Place, int WorkStep_Sum);

    void Show_DisOneCircle(const HObject &Image_obj, const HTuple &Row_obj, const HTuple &Columm_obj, const HObject &Cross_obj, const HObject &Contours_obj);

    void dealClose();

signals:
    void startThread();
    void startGJThread();
    void start_DisOneCircle(const HObject &Image, HTuple hv_WindowHandle, int MaxScore_Place);

private:
    Ui_Mainscene *ui;

    MyThread *myT_camera;
    QThread *thread;
    MyThread_Discern *myG_Discern;
    QThread *thread_GJ;
    MyThread_DisOneCircle *myG_DisOneCircle;
    QThread *thread_DisOneCircle;

    CSetStationDialog *mStationSet;
    EngDocument *mEngDocument;
    ModelTool *mModelTool;
    ROITool *mROITool;
    CameraParams *mCameraParams;
    LightSet *mLightSet;

    HObject Image, hv_Image;
    HTuple AcqHandle, WindowHandle, x, y, hv_SerialHandle, hv_output_SerialPort,
        hv_string_Raw, hv_output, hv_Ords, hv_Raw;

    bool Lock_Camera = false;
    bool Lock_AnewOpenCamera = false;
    bool Lock_CameraFirstQuit = false;
    bool lock_SerialPort = false;

    bool Lock_GJCamera = false;
    bool Lock_AnewOpenGJCamera = false;
    bool Lock_CameraFirstQuitGJ = false;
    bool Lock_ShowMainThreadImage = false;
    int i = 0;

    int GainRaw = 51;
    int ExposureTime = 35000;
    int Current_EngDoc;
    QString CircularOrLine_Tem;
    QString Station_Num_Tem;
    int ROI_StationNum_Tem;

protected slots:
    void slotSwitchUsers();

protected:
    void initAction();
};