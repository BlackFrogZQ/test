#pragma once
#include "../basic.h"

enum CStationType : uint8_t
{
    stCircularPositioning,
    stMultiCirclePositioning,
    stOneLineCrossingTwoSidesPositioning,
    stMax
};

const QStringList cStationTypeIcon = {
    ":/res/CircularPositioning.png",
    ":/res/MultiCirclePositioning.png",
    ":/res/OneLineCrossingTwoSidesPositioning.png",
};

const QStringList cStationTypeStr = {
    cnStr("圆定位"),
    cnStr("多组圆定位"),
    cnStr("一线交两边定位")
};
