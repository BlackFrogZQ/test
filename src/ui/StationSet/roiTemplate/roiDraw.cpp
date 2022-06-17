#include "ROITool.h"
void CRoiToolDialog::drawTemplate(const CDrawRoiShapeType &p_drawShape)
{
    switch (p_drawShape)
    {
    case cdrstRectangle:
        drawRectangle();
        break;
    case cdrstRectangle2:
        drawRectangle2();
        break;
    case cdrstCircular:
        drawCircular();
        break;
    case cdrstArbitrarily:
        drawArbitrarily();
        break;
    default:
        break;
    }
}

void CRoiToolDialog::drawRectangle()
{
    HTuple hv_Row_Rectangle, hv_Column_Rectangle1, hv_Row_Rectangle2, hv_Column_Rectangle3;
    DrawRectangle1(ROI_WindowHandle, &hv_Row_Rectangle, &hv_Column_Rectangle1, &hv_Row_Rectangle2, &hv_Column_Rectangle3);
    GenRectangle1(&m_template, hv_Row_Rectangle, hv_Column_Rectangle1, hv_Row_Rectangle2, hv_Column_Rectangle3);
}

void CRoiToolDialog::drawRectangle2()
{
    HTuple hv_Row_Rectangle21, hv_Column_Rectangle21, hv_Phi_Rectangle2, hv_Length_Rectangle21, hv_Length_Rectangle22;
    DrawRectangle2(ROI_WindowHandle, &hv_Row_Rectangle21, &hv_Column_Rectangle21, &hv_Phi_Rectangle2, &hv_Length_Rectangle21, &hv_Length_Rectangle22);
    GenRectangle2(&m_template, hv_Row_Rectangle21, hv_Column_Rectangle21, hv_Phi_Rectangle2, hv_Length_Rectangle21, hv_Length_Rectangle22);
}

void CRoiToolDialog::drawCircular()
{
    HTuple hv_Row_Rectangle21, hv_Column_Rectangle21, hv_Phi_Rectangle2;
    DrawCircle(ROI_WindowHandle, &hv_Row_Rectangle21, &hv_Column_Rectangle21, &hv_Phi_Rectangle2);
    GenCircle(&m_template, hv_Row_Rectangle21, hv_Column_Rectangle21, hv_Phi_Rectangle2);
}

void CRoiToolDialog::drawArbitrarily()
{
    DrawRegion(&m_template, ROI_WindowHandle);
}

void CRoiToolDialog::drawContours(const CWorkingProcedureMode &p_stationMode)
{
    switch (p_stationMode)
    {
    case csmCircular:
        drawContoursCircular();
        break;
    case csmLine:
        drawContoursLine();
        break;
    default:
        break;
    }
}

void CRoiToolDialog::drawContoursCircular()
{
    HObject tempImage;
    Rgb1ToGray(m_image, &tempImage);
    SetColor(ROI_WindowHandle, "red");
    DispObj(tempImage, ROI_WindowHandle);

    ReduceDomain(tempImage, m_template, &tempImage);
    {
        HTuple w, h;
        HObject ho_ImageReduced_template;
        GetImageSize(m_image, &w, &h);
        //图像增强
        Emphasize(ho_ImageReduced_template, &tempImage, w, h, 1);
    }

    //中值滤波(保留了边缘)
    MedianImage(tempImage, &tempImage, "circle", 5, "mirrored");

    //基于亚像素的边缘提取
    EdgesSubPix(tempImage, &tempImage, "canny", 1, 20, 40);

    //合并属于同一个圆上的轮廓
    UnionCocircularContoursXld(tempImage, &tempImage, 0.5, 0.1, 0.2, 30, 10, 10, "true", 1);
    SelectContoursXld(tempImage, &tempImage, "contour_length", 50, 20000, -0.5, 0.5);
    SelectShapeXld(tempImage, &tempImage, "circularity", "and", 0.2, 1);

    //获取圆度最大的曲线
    HTuple hv_Circularity, hv_Number;
    CircularityXld(tempImage, &hv_Circularity);
    CountObj(tempImage, &hv_Number);
    HTuple hv_CIRCULARITY = 0;
    {
        HTuple end_val42 = hv_Number - 1;
        HTuple step_val42 = 1;
        HTuple hv_Index;
        for (hv_Index = 0; hv_Index.Continue(end_val42, step_val42); hv_Index += step_val42)
        {
            if (0 != (int(HTuple(hv_Circularity[hv_Index]) > hv_CIRCULARITY)))
            {
                hv_CIRCULARITY = HTuple(hv_Circularity[hv_Index]);
            }
        }
    }
    SelectShapeXld(tempImage, &tempImage, "circularity", "and", hv_CIRCULARITY, hv_CIRCULARITY);
    CountObj(tempImage, &hv_Number);

    if (hv_Number != 1)
    {
        return;
    }

    HTuple hv_Row, hv_Column, hv_Radius, hv_StartPhi, hv_EndPhi, hv_PointOrder;
    //拟合成圆
    FitCircleContourXld(tempImage, "algebraic", -1, 0, 0, 3, 2, &hv_Row, &hv_Column, &hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
    GenCircleContourXld(&m_contoursImage, hv_Row, hv_Column, hv_Radius, 0, HTuple(360).TupleRad(), "positive", 1);
    DispObj(m_contoursImage, ROI_WindowHandle);
}

void CRoiToolDialog::drawContoursLine()
{
    HObject tempImage;
    Rgb1ToGray(m_image, &tempImage);
    SetColor(ROI_WindowHandle, "red");
    DispObj(tempImage, ROI_WindowHandle);
    ReduceDomain(tempImage, m_template, &tempImage);

    //图像增强
    Emphasize(tempImage, &tempImage, 11, 11, 30);
    //中值滤波(保留了边缘)
    MedianImage(tempImage, &tempImage, "circle", 5, "mirrored");

    //基于亚像素的边缘提取
    // edges_sub_pix (ImageMedian_Line, Edges, 'canny', 1, 20, 40)
    LinesGauss(tempImage, &tempImage, 1.5, 3, 8, "dark", "true", "bar-shaped", "true");

    //合并位于同一条直线上的轮廓
    UnionCollinearContoursXld(tempImage, &tempImage, 15, 5, 2, 0.1, "attr_keep");
    SelectContoursXld(tempImage, &tempImage, "contour_length", 10, 20000, -0.5, 0.5);

    //获取最长的直线
    HTuple hv_Length_Line, hv_Number1_Line;
    LengthXld(tempImage, &hv_Length_Line);
    CountObj(tempImage, &hv_Number1_Line);
    HTuple hv_Length_max = 0;
    {
        HTuple end_val42 = hv_Number1_Line - 1;
        HTuple step_val42 = 1;
        HTuple hv_Index_Line;
        for (hv_Index_Line = 0; hv_Index_Line.Continue(end_val42, step_val42); hv_Index_Line += step_val42)
        {
            if (0 != (int(HTuple(hv_Length_Line[hv_Index_Line]) > hv_Length_max)))
            {
                hv_Length_max = HTuple(hv_Length_Line[hv_Index_Line]);
            }
        }
    }
    SelectContoursXld(tempImage, &tempImage, "contour_length", hv_Length_max - 1, hv_Length_max, -0.5, 0.5);
    {
        HTuple hv_Number2_Line;
        CountObj(tempImage, &hv_Number2_Line);
        if (0 != (int(hv_Number2_Line != 1)))
        {
            return;
        }
    }
    HTuple hv_RowBegin, hv_ColBegin, hv_RowEnd, hv_ColEnd, hv_Nr, hv_Nc, hv_Dist;

    //拟合成直线
    FitLineContourXld(tempImage, "tukey", -1, 0, 5, 2, &hv_RowBegin, &hv_ColBegin, &hv_RowEnd, &hv_ColEnd, &hv_Nr, &hv_Nc, &hv_Dist);
    //生成区域的直线
    GenRegionLine(&tempImage, hv_RowBegin, hv_ColBegin, hv_RowEnd, hv_ColEnd);
    //生成轮廓的直线
    GenContourPolygonXld(&m_template, hv_RowBegin.TupleConcat(hv_RowEnd), hv_ColBegin.TupleConcat(hv_ColEnd));
    DispObj(tempImage, ROI_WindowHandle);
}
