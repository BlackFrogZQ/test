#pragma once
#include "ui_StationSet.h"
#include "ModelTool.h"
#include "src/system/projectManager/StationSetDef.h"

class QTableWidgetItem;
struct CProject;
class CSetStationDialog : public QDialog
{
    Q_OBJECT

public:
    CSetStationDialog(QWidget *parent = nullptr);
    ~CSetStationDialog();

protected slots:
    void slotAddStationPb();
    void slotCreateModel();
    void slotDelStationPb();
    void slotCurrentChange(int row);

protected:
    void addStationWorkingProcedure(const CStationWorkingProcedure &p_workingProcedure);
    void showRoiToolDialog(const CWorkingProcedureMode &p_stationType);
    void showModelToolDialog();
    void updateProject();

protected:
    CProject *m_project;
    int m_stationId;
    int m_workingProcedureId;
private:
    Ui::StationSet *ui;
};