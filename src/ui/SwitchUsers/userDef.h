#pragma once
#include "../basic.h"

enum CUserType : uint8_t
{
    cutEngineer,
    cutAdministrators,
    cutOperator,
    cutMax
};
const QStringList cUserTypeStr = {cnStr("工程师"), cnStr("管理员"), cnStr("操作员")};