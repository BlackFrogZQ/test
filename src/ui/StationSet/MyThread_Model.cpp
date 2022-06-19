#include "MyThread_Model.h"

MyThread_Model::MyThread_Model(QObject *parent)
: QObject(parent)
{
    qRegisterMetaType<HObject>("HObject");
    qRegisterMetaType<HTuple>("HTuple");
}

void MyThread_Model::GenerateTemplate(const HObject& ho_Image, HTuple hv_WindowHandle, int rBtn_Shape, int Numberlever, 
                                    int AngleStart, int AngleExtern, int AngleStep, int Contrast, int MinContrast, 
                                    QString Optimization, QString Metric)
{
    //获得模板参数
    QString AUTO = "auto";
    //图像金字塔
    HTuple hTuple_Numberlever,hTuple_Numberlever_Last;
    if(Numberlever == 0)
    {
        hTuple_Numberlever[3] = AUTO.toStdString().data();
        hTuple_Numberlever_Last = hTuple_Numberlever[3];
    }
    else{
        hTuple_Numberlever[0] = Numberlever;
        hTuple_Numberlever_Last = hTuple_Numberlever[0];
    }

    //起始角度和角度范围
    HTuple hTuple_AngleStart,hTuple_AngleExtern;
    TupleRad(AngleStart, &hTuple_AngleStart);
    TupleRad(AngleExtern, &hTuple_AngleExtern);

    //角度步长
    HTuple hTuple_AngleStep,hTuple_AngleStep_Last,hTuple_ultimately;
    if(AngleStep == 0)
    {
        hTuple_AngleStep[3] = AUTO.toStdString().data();
        hTuple_AngleStep_Last = hTuple_AngleStep[3];
    }
    else{
        hTuple_AngleStep[0] = AngleStep;
        hTuple_AngleStep_Last = hTuple_AngleStep[0];
        TupleRad(hTuple_AngleStep_Last, &hTuple_ultimately);
        hTuple_AngleStep_Last = hTuple_ultimately;
    }

    //优化算法和极性
    HTuple hTuple_Optimization,hTuple_Metric;
    hTuple_Optimization[3] = Optimization.toStdString().data();
    hTuple_Metric[3] = Metric.toStdString().data();

    //对比度
    HTuple hTuple_Contrast,hTuple_Contrast_Last;
    if(Contrast == 0)
    {
        hTuple_Contrast[3] = AUTO.toStdString().data();
        hTuple_Contrast_Last = hTuple_Contrast[3];
    }
    else{
        hTuple_Contrast[0] = Contrast;
        hTuple_Contrast_Last = hTuple_Contrast[0];
    }

    //最小对比度
    HTuple hTuple_MinContrast,hTuple_MinContrast_Last;
    if(MinContrast == 0)
    {
        hTuple_MinContrast[3] = AUTO.toStdString().data();
        hTuple_MinContrast_Last = hTuple_MinContrast[3];
    }
    else{
        hTuple_MinContrast[0] = MinContrast;
        hTuple_MinContrast_Last = hTuple_MinContrast[0];
    }

    Rgb1ToGray(ho_Image, &ho_GrayImage);
    DispObj(ho_GrayImage, hv_WindowHandle);
    if(rBtn_Shape == 1){
        DrawRectangle1(hv_WindowHandle, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
        GenRectangle1(&ho_Model_template, hv_Row1, hv_Column1, hv_Row2, hv_Column2);
    }
    else if(rBtn_Shape == 2){
        DrawRectangle2(hv_WindowHandle, &hv_Row, &hv_Column, &hv_Phi, &hv_Length1, &hv_Length2);
        GenRectangle2(&ho_Model_template, hv_Row, hv_Column, hv_Phi, hv_Length1, hv_Length2);
    }
    else if(rBtn_Shape == 3){
        DrawCircle(hv_WindowHandle, &hv_Row_template, &hv_Column_template, &hv_Radius_template);
        GenCircle(&ho_Model_template, hv_Row_template, hv_Column_template, hv_Radius_template);
    }
    else{
        DrawRegion(&ho_Model_template, hv_WindowHandle);
    }
    ReduceDomain(ho_GrayImage, ho_Model_template, &ho_ImageReduced_template);

    InspectShapeModel(ho_ImageReduced_template, &ho_ModelImages, &ho_ModelRegions, 4, 30);
    CreateShapeModel(ho_ImageReduced_template, hTuple_Numberlever_Last, hTuple_AngleStart, hTuple_AngleExtern, 
                    hTuple_AngleStep_Last, hTuple_Optimization[3], hTuple_Metric[3], hTuple_Contrast_Last, 
                    hTuple_MinContrast_Last, &hv_ModelID);
    GetShapeModelContours(&ho_ModelContours, hv_ModelID, 1);

    InspectShapeModel(ho_ImageReduced_template, &ho_ModelImages, &ModelContours, 1, 30);
    emit Send_ModelContours(ModelContours);

    AreaCenter(ho_ImageReduced_template, &hv_Area_center, &hv_Row_center, &hv_Column_center);
    TupleConcat(hv_Row_center, hv_Column_center, &hv_Concat);
}

void MyThread_Model::mT_SavesModel(QString Station_Num)
{
    QString path_current = QDir::currentPath();
    QString add_current = "saveModel/";
    path_current += add_current;

    HTuple hTuple_ShapeModel, hTuple_ModelPos, hv_Station_Num, hv_path_current;
    hv_Station_Num[3] = Station_Num.toStdString().data();
    hv_path_current[3] = path_current.toStdString().data();
    hTuple_ShapeModel = (hv_path_current[3]+"ShapeModel_" + hv_Station_Num[3]) + ".smd.shm";
    hTuple_ModelPos = hv_path_current[3]+"ModelPos_" + hv_Station_Num[3] + ".tup";

    WriteShapeModel(hv_ModelID, hTuple_ShapeModel);
    WriteTuple(hv_Concat, hTuple_ModelPos);
}