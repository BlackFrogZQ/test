#pragma once
#include "projectDef.h"
#include "../sigleClass.h"

class CProjectManager
{
    SINGLETON(CProjectManager)

public:
    void loadProject();
    void removeCurrentProject();
    void setCurrentProject(const int &id);
    CProject *getCurrentProject() const;
    QStringList getProjectNames() const;
    // 添加项目并切换到当前,同名则替换
    void addProject(const QString &name);

protected:
    bool removeProject(const CProject *const p_pProject);

protected:
    QVector<CProject> m_projects;
    CProject *m_pCurrentProject;
};

CProjectManager *projectManager();
