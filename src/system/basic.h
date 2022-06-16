#pragma once
#include <QDebug>

#define cnStr(str) QString::fromLocal8Bit(str)
#define cnHStr(str) QString::fromLocal8Bit(str).toStdString().data()

#define cnStr(str) QString::fromLocal8Bit(str)

#define delPtr(ptr)     \
    if (ptr != nullptr) \
    {                   \
        delete ptr;     \
        ptr = nullptr;  \
    }
