#include "MyThread_DisOneCircle.h"

MyThread_DisOneCircle::MyThread_DisOneCircle(QObject *parent)
: QObject(parent)
{
    qRegisterMetaType<HObject>("HObject");
    qRegisterMetaType<HTuple>("HTuple");
}

//识别单个圆
void MyThread_DisOneCircle::Start_DisOneCircle(HObject ho_Image, HTuple hv_WindowHandle, int MaxScore_Place)
{
    Rgb1ToGray(ho_Image, &ho_GrayImage);

    QString path_current = QDir::currentPath();
    QString add_current = "/src/StationSet/SaveModel/";
    path_current += add_current;

    HTuple hTuple_ShapeModel, hTuple_ModelPos, hTuple_region, hv_Station_Num, hv_path_current, hv_Tail;
    hv_Station_Num[3] = MaxScore_Place;
    hv_path_current[3] = path_current.toStdString().data();
    hv_Tail[3] = 1;
    hTuple_ShapeModel = (hv_path_current[3]+"ShapeModel_" + hv_Station_Num[3]) + ".smd.shm";
    hTuple_ModelPos = hv_path_current[3]+"ModelPos_" + hv_Station_Num[3] + ".tup";
    hTuple_region = (hv_path_current[3]+"region_" + hv_Station_Num[3] + hv_Tail[3]) +".hobj";

    ReadShapeModel(hTuple_ShapeModel, &hv_ModelID);
    ReadTuple(hTuple_ModelPos, &hv_Tuple);
    ReadRegion(&ho_Region, hTuple_region);

    hv_RomM = ((const HTuple &)hv_Tuple)[0];
    hv_ColM = ((const HTuple &)hv_Tuple)[1];

    GetShapeModelContours(&ho_ModelContours, hv_ModelID, 1);

    FindShapeModel(ho_GrayImage, hv_ModelID, 0, 360, 0.5, 1, 0.5, "least_squares",0, 0.9, &hv_Row1, &hv_Column1, &hv_Angle, &hv_Score);

    if (0 != ((hv_Score.TupleLength()) > 0))
    {
        VectorAngleToRigid(0, 0, 0, hv_Row1, hv_Column1, hv_Angle, &hv_HomMat2D);
        AffineTransContourXld(ho_ModelContours, &ho_ContoursAffinTrans, hv_HomMat2D);

        VectorAngleToRigid(hv_RomM, hv_ColM, 0, hv_Row1, hv_Column1, hv_Angle, &hv_HomMat2D1);
        AffineTransRegion(ho_Region, &ho_RegionAffineTrans, hv_HomMat2D1, "nearest_neighbor");

        //获得圆
        ReduceDomain(ho_GrayImage, ho_RegionAffineTrans, &ho_ImageReduced2);
        MedianImage(ho_ImageReduced2, &ho_ImageMedian, "circle", 5, "mirrored");
        EdgesSubPix(ho_ImageMedian, &ho_Edges, "canny", 3, 20, 40);

        SelectContoursXld(ho_Edges, &ho_SelectedContours, "contour_length", 300, 600, -0.5, 0.5);
        SelectShapeXld(ho_SelectedContours, &ho_SelectedXLD, "circularity", "and", 0.85, 1);

        CircularityXld(ho_SelectedXLD, &hv_Circularity);
        CountObj(ho_SelectedXLD, &hv_Number);
        hv_CIRCULARITY = 0;
        {
            HTuple end_val65 = hv_Number - 1;
            HTuple step_val65 = 1;
            for (hv_Index = 0; hv_Index.Continue(end_val65, step_val65); hv_Index += step_val65)
            {
                if (0 != (HTuple(hv_Circularity[hv_Index]) > hv_CIRCULARITY))
                {
                    hv_CIRCULARITY = ((const HTuple &)hv_Circularity)[hv_Index];
                }
            }
        }
        SelectShapeXld(ho_SelectedXLD, &ho_SelectedXLD1, "circularity", "and", hv_CIRCULARITY, hv_CIRCULARITY);

        CountObj(ho_SelectedXLD1, &hv_Number1);
        if (0 != (hv_Number1 != 1))
        {
            return;
        }

        //获得圆心
        AreaCenterXld(ho_SelectedXLD1, &hv_Area1, &hv_Row2, &hv_Column2, &hv_PointOrder);
        GenCrossContourXld(&ho_Cross, hv_Row2, hv_Column2, 60, 0.785398);
    }

    emit Send_FinishDisOneCircle(ho_GrayImage, hv_Row2, hv_Column2, ho_Cross, ho_SelectedContours);
    qDebug()<<cnStr("单个圆识别线程号：")<<QThread::currentThread();
}
