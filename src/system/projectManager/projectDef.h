#pragma once
#include "StationSetDef.h"

static inline QString getSaveProjectDir()
{
    return QDir::currentPath() + "/1.Project";
}

struct CProject
{
    QString name;
    QVector<CStation> stations;
    void removeStation(const int &id)
    {
        if (id < 0 && id >= this->stations.size())
        {
            return;
        }

        QString dir = getSaveProjectDir() + "/" + this->getDirName() + "/" + this->stations[id].getDirName();
        if (removeDir(dir))
        {
            this->stations.removeAt(id);
            stationSrot();
        }
    }

    QString getDirName() const
    {
        return name + "_project";
    };

    bool loadDirName(const QString &p_dir)
    {
        QStringList strs = QFileInfo(p_dir).baseName().split("_");
        if (strs.size() == 2 && strs.last() == "project")
        {
            name = strs.first();
            strs = scanfDir(p_dir);
            stations.clear();
            stations.reserve(strs.size());
            for (const auto &str : strs)
            {
                CStation station;
                if (station.loadDirName(str))
                {
                    stations.append(station);
                }
            }
            return true;
        }
        return false;
    }

    void stationSrot()
    {
        for (size_t i = 0; i < stations.size(); i++)
        {
            if (stations[i].id != i)
            {
                QString dir = getSaveProjectDir() + "/" + this->getDirName();
                QString old = dir + "/" + stations[i].getDirName();
                stations[i].id = i;
                renameDir(old, dir + "/" + stations[i].getDirName());
            }
        }
    }
};
