#include "MyThread_ROI.h"

MyThread_ROI::MyThread_ROI(QObject *parent)
: QObject(parent)
{
    qRegisterMetaType<HObject>("HObject");
    qRegisterMetaType<HTuple>("HTuple");
}

void MyThread_ROI::GenerateROITemplate(const HObject& ho_Image, HTuple hv_WindowHandle, int rBtn_Shape, QString CircularOrLine)
{
    Rgb1ToGray(ho_Image, &ho_GrayImage);
    DispObj(ho_GrayImage, hv_WindowHandle);
    GetImageSize(ho_GrayImage, &hv_Width, &hv_Height);
    if(rBtn_Shape == 1){
        DrawRectangle1(hv_WindowHandle, &hv_Row_Rectangle11, &hv_Column_Rectangle11, &hv_Row_Rectangle12, &hv_Column_Rectangle12);
        GenRectangle1(&ho_ROI_template, hv_Row_Rectangle11, hv_Column_Rectangle11, hv_Row_Rectangle12, hv_Column_Rectangle12);
    }
    else if(rBtn_Shape == 2){
        DrawRectangle2(hv_WindowHandle, &hv_Row_Rectangle21, &hv_Column_Rectangle21, &hv_Phi_Rectangle2, &hv_Length_Rectangle21, &hv_Length_Rectangle22);
        GenRectangle2(&ho_ROI_template, hv_Row_Rectangle21, hv_Column_Rectangle21, hv_Phi_Rectangle2, hv_Length_Rectangle21, hv_Length_Rectangle22);
    }
    else if(rBtn_Shape == 3){
        DrawCircle(hv_WindowHandle, &hv_Row_Circle, &hv_Column_Circle, &hv_Radius_Circle);
        GenCircle(&ho_ROI_template, hv_Row_Circle, hv_Column_Circle, hv_Radius_Circle);
    }
    else{
        DrawRegion(&ho_ROI_template, hv_WindowHandle);
    }

    ReduceDomain(ho_GrayImage, ho_ROI_template, &ho_ImageReduced_template);

    if(CircularOrLine == cnStr("圆")){
        //图像增强
        Emphasize(ho_ImageReduced_template, &ho_ImageEmphasize, hv_Width, hv_Height, 1);
        //中值滤波(保留了边缘)
        MedianImage(ho_ImageEmphasize, &ho_ImageMedian, "circle", 5, "mirrored");

        //基于亚像素的边缘提取
        EdgesSubPix(ho_ImageMedian, &ho_Edges, "canny", 1, 20, 40);

        //合并属于同一个圆上的轮廓
        UnionCocircularContoursXld(ho_Edges, &ho_UnionContours, 0.5, 0.1, 0.2, 30, 10, 10, "true", 1);
        SelectContoursXld(ho_UnionContours, &ho_SelectedContours, "contour_length", 50, 20000, -0.5, 0.5);
        SelectShapeXld(ho_SelectedContours, &ho_SelectedXLD, "circularity", "and", 0.2, 1);

        //获取圆度最大的曲线
        CircularityXld(ho_SelectedXLD, &hv_Circularity);
        CountObj(ho_SelectedXLD, &hv_Number);
        hv_CIRCULARITY = 0;
        {
            HTuple end_val42 = hv_Number-1;
            HTuple step_val42 = 1;
            for (hv_Index=0; hv_Index.Continue(end_val42, step_val42); hv_Index += step_val42)
            {
                if (0 != (int(HTuple(hv_Circularity[hv_Index])>hv_CIRCULARITY)))
                {
                    hv_CIRCULARITY = HTuple(hv_Circularity[hv_Index]);
                }
            }
        }
        SelectShapeXld(ho_SelectedXLD, &ho_SelectedXLD1, "circularity", "and", hv_CIRCULARITY, hv_CIRCULARITY);
        CountObj(ho_SelectedXLD1, &hv_Number1);
        if (0 != (int(hv_Number1!=1)))
        {
            return;
        }

        //拟合成圆
        FitCircleContourXld(ho_SelectedXLD1, "algebraic", -1, 0, 0, 3, 2, &hv_Row, &hv_Column, &hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
        GenCircleContourXld(&ho_ContCircle, hv_Row, hv_Column, hv_Radius, 0, HTuple(360).TupleRad(), "positive", 1);

        SetColor(hv_WindowHandle,"red");
        DispObj(ho_GrayImage, hv_WindowHandle);
        DispObj(ho_ContCircle, hv_WindowHandle);
        emit Send_ROIContours(ho_ContCircle);
    }
    else{
        //图像增强
        Emphasize(ho_ImageReduced_template, &ho_ImageEmphasize_Line, 11, 11, 30);
        //中值滤波(保留了边缘)
        MedianImage(ho_ImageEmphasize_Line, &ho_ImageMedian_Line, "circle", 5, "mirrored");

        //基于亚像素的边缘提取
        //edges_sub_pix (ImageMedian_Line, Edges, 'canny', 1, 20, 40)
        LinesGauss(ho_ImageEmphasize_Line, &ho_Lines, 1.5, 3, 8, "dark", "true", "bar-shaped", "true");

        //合并位于同一条直线上的轮廓
        UnionCollinearContoursXld(ho_Lines, &ho_UnionContours_Line, 15, 5, 2, 0.1, "attr_keep");
        SelectContoursXld(ho_UnionContours_Line, &ho_SelectedContours1_Line, "contour_length", 10, 20000, -0.5, 0.5);

        //获取最长的直线
        LengthXld(ho_SelectedContours1_Line, &hv_Length_Line);
        CountObj(ho_SelectedContours1_Line, &hv_Number1_Line);
        hv_Length_max = 0;
        {
            HTuple end_val42 = hv_Number1_Line-1;
            HTuple step_val42 = 1;
            for (hv_Index_Line=0; hv_Index_Line.Continue(end_val42, step_val42); hv_Index_Line += step_val42)
            {
                if (0 != (int(HTuple(hv_Length_Line[hv_Index_Line])>hv_Length_max)))
                {
                    hv_Length_max = HTuple(hv_Length_Line[hv_Index_Line]);
                }
            }
        }
        SelectContoursXld(ho_SelectedContours1_Line, &ho_SelectedContours2_Line, "contour_length", hv_Length_max-1, hv_Length_max, -0.5, 0.5);
        CountObj(ho_SelectedContours2_Line, &hv_Number2_Line);
        if (0 != (int(hv_Number2_Line!=1)))
        {
            return;
        }

        //拟合成直线
        FitLineContourXld(ho_SelectedContours2_Line, "tukey", -1, 0, 5, 2, &hv_RowBegin, &hv_ColBegin, &hv_RowEnd, &hv_ColEnd, &hv_Nr, &hv_Nc, &hv_Dist);
        //生成区域的直线
        GenRegionLine(&ho_RegionLines, hv_RowBegin, hv_ColBegin, hv_RowEnd, hv_ColEnd);
        //生成轮廓的直线
        GenContourPolygonXld(&ho_Contour_Line, hv_RowBegin.TupleConcat(hv_RowEnd), hv_ColBegin.TupleConcat(hv_ColEnd));

        SetColor(hv_WindowHandle,"red");
        DispObj(ho_GrayImage, hv_WindowHandle);
        DispObj(ho_Contour_Line, hv_WindowHandle);
        emit Send_ROIContours(ho_Contour_Line);
    }
}

void MyThread_ROI::mT_SavesROI(QString Station_Num, int ROI_StationNum)
{
    QString path_current = QDir::currentPath();
    QString add_current = "/src/StationSet/SaveModel/";
    path_current += add_current;

    HTuple hTuple_region, hv_Station_Num, hv_path_current, hv_ROI_StationNum;
    hv_Station_Num[3] = Station_Num.toStdString().c_str();
    hv_path_current[3] = path_current.toStdString().c_str();

    hTuple_region = (hv_path_current[3]+"region_" + hv_Station_Num[3] + ROI_StationNum) + ".hobj";
    WriteRegion(ho_ROI_template, hTuple_region);
}