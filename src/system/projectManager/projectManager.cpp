#include "projectManager.h"
SINGLETON_GARBO(CProjectManager);

CProjectManager::CProjectManager() : m_pCurrentProject(nullptr)
{
    loadProject();
}

CProjectManager::~CProjectManager()
{
}



void CProjectManager::loadProject()
{
    m_projects.clear();
    QStringList projectNames = scanfDir(getSaveProjectDir());
    for (const auto &projectName : projectNames)
    {
        CProject project;
        if (project.loadDirName(projectName))
        {
            m_projects.append(project);
        }
    }
    m_pCurrentProject = (m_projects.size() > 0) ? &(m_projects.first()) : nullptr;
}

void CProjectManager::removeCurrentProject()
{
    if (m_projects.size() == 0 || m_pCurrentProject == nullptr)
    {
        return;
    }
    if (removeProject(m_pCurrentProject))
    {
        m_pCurrentProject = (m_projects.size() > 0) ? &(m_projects.first()) : nullptr;
    }
}

void CProjectManager::setCurrentProject(const int &id)
{
    assert(id >= 0 && id < m_projects.size());
    m_pCurrentProject = &(m_projects[id]);
}

CProject *CProjectManager::getCurrentProject() const
{
    assert(m_pCurrentProject != nullptr);
    return m_pCurrentProject;
}

QStringList CProjectManager::getProjectNames() const
{
    QStringList names;
    for (const auto &project : m_projects)
    {
        names.append(project.name);
    }
    return names;
}

void CProjectManager::addProject(const QString &name)
{
    for (size_t i = 0; i < m_projects.size(); i++)
    {
        if (m_projects[i].name == name)
        {
            setCurrentProject(i);
            return;
        }
    }

    CProject project;
    project.name = name;
    m_projects.append(project);
    createDir(getSaveProjectDir() + "/" + project.getDirName());
    setCurrentProject(m_projects.size() - 1);
}

bool CProjectManager::removeProject(const CProject *const p_pProject)
{
    assert(p_pProject != nullptr);
    for (size_t i = 0; i < m_projects.size(); i++)
    {
        if (p_pProject == (&(m_projects[i])))
        {
            if (removeDir(getSaveProjectDir() + "/" + p_pProject->getDirName()))
            {
                m_projects.removeAt(i);
                return true;
            }
        }
    }
    return false;
}

CProjectManager *projectManager()
{
    return CProjectManager::instance();
}
