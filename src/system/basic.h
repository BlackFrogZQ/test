#pragma once
#include <QDebug>
#include <QDir>
#include <QFileInfo>

#define iToStr(num) QString::number(num)
#define cnStr(str) QString::fromLocal8Bit(str)
#define cnHStr(str) QString::fromLocal8Bit(str).toStdString().data()

#define cnStr(str) QString::fromLocal8Bit(str)

#define delPtr(ptr)     \
    if (ptr != nullptr) \
    {                   \
        delete ptr;     \
        ptr = nullptr;  \
    }

static inline QStringList scanfDir(const QString &p_dir)
{
    QFileInfo info(p_dir);
    QStringList dirs;
    if (!info.isDir())
    {
        return dirs;
    }
    QFileInfoList folderList = QDir(p_dir).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    for (const auto &folder : folderList)
    {
        dirs.append(folder.absoluteFilePath());
    }
    return dirs;
}

static inline bool removeDir(const QString &p_dir)
{
    QFileInfo info(p_dir);
    if (!info.isDir())
    {
        return false;
    }
    return QDir(p_dir).removeRecursively();
}

static inline bool createDir(const QString &p_absDir)
{
    return QDir().mkpath(p_absDir);
}

static inline bool renameDir(const QString &p_absOldDir, const QString &p_absNewDir)
{
    return QDir(p_absOldDir).rename(p_absOldDir, p_absNewDir);
}