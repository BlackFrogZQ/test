#pragma once
#include "../basic.h"

enum CStationMode : uint8_t
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
    CStationMode mode;
    QString name;
    CStationWorkingProcedure(const int &p_id, const CStationMode &p_mode, const QString &p_name) : id(p_id), mode(p_mode), name(p_name){};
};
