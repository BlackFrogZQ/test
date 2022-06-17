#pragma once
#include "StationAttributeDef.h"

enum CWorkingProcedureMode : uint8_t
{
    csmTemplate,
    csmLine,
    csmCircular,
    csmMax
};
const QStringList cStationModeStr = {cnStr("模板"), cnStr("线"), cnStr("圆")};

struct CStationWorkingProcedure
{
    int id;
    CWorkingProcedureMode mode;
    QString name;
    CStationWorkingProcedure(const int &p_id = -1, const CWorkingProcedureMode &p_mode = csmMax, const QString &p_name = "") : id(p_id), mode(p_mode), name(p_name){};

    QString getDirName() const
    {
        return iToStr(id) + "_" + iToStr(mode) + "_" + name + "_workingProcedure";
    };

    bool loadDirName(const QString &p_dir)
    {
        QStringList strs = QFileInfo(p_dir).baseName().split("_");
        if (strs.size() == 4 && strs.last() == "workingProcedure")
        {
            id = strs[0].toInt();
            mode = CWorkingProcedureMode(strs[1].toInt());
            name = strs[2];
            return true;
        }
        return false;
    }

    //非csmTemplate时使用
    QString getRoiTemplateName() const
    {
        return getDirName() + "/" + "roi.hobj";
    }

    // csmTemplate时使用
    QString getTemplateShapeName() const
    {
        return getDirName() + "/" + "shape.smd.shm";
    }

    QString getTemplateCentralPosName() const
    {
        return getDirName() + "/" + "centralPos.tup";
    }
};

struct CStation
{
    int id;
    CStationType type;
    QString name;
    QVector<CStationWorkingProcedure> workingProcedures;

    QString getDirName() const
    {
        return iToStr(id) + "_" + iToStr(type) + "_" + name + "_station";
    };

    bool loadDirName(const QString &p_dir)
    {
        QStringList strs = QFileInfo(p_dir).baseName().split("_");
        if (strs.size() == 4 && strs.last() == "station")
        {
            id = strs[0].toInt();
            type = CStationType(strs[1].toInt());
            name = strs[2];
            strs = scanfDir(p_dir);
            workingProcedures.clear();
            workingProcedures.reserve(strs.size());
            for (const auto &str : strs)
            {
                CStationWorkingProcedure wp;
                if (wp.loadDirName(str))
                {
                    workingProcedures.append(wp);
                }
            }
            return true;
        }
        return false;
    }
    
};