#include "StationSet.h"
#include "roiTemplate/ROITool.h"
#include "StationAttribute.h"
#include "src/system/projectManager/projectManager.h"

CSetStationDialog::CSetStationDialog(QWidget *parent)
    : QDialog(parent),
      m_stationId(-1),
      m_workingProcedureId(-1),
      ui(new Ui::StationSet)
{
    ui->setupUi(this);

    //设置Table_Eng属性
    ui->Table_Temp->setColumnCount(5);
    ui->Table_Temp->setHorizontalHeaderLabels(QStringList() << cnStr("工位") << cnStr("工序") << cnStr("模式") << cnStr("名称") << cnStr("状态"));
    ui->Table_Temp->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;color: black;}");
    ui->Table_Temp->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Table_Temp->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Table_Temp->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->Table_Temp->verticalHeader()->setHidden(true);
    ui->Table_Temp->resizeRowsToContents();
    connect(ui->btn_AddStation, &QPushButton::clicked, this, &CSetStationDialog::slotAddStationPb);
    connect(ui->btn_EditStation, &QPushButton::clicked, this, &CSetStationDialog::slotCreateModel);
    connect(ui->btn_DeleteStation, &QPushButton::clicked, this, &CSetStationDialog::slotDelStationPb);
    connect(ui->btn_ExitEdit, &QPushButton::clicked, this, [=]()
            { close(); });

    connect(ui->Table_Temp, &QTableWidget::cellClicked, this, &CSetStationDialog::slotCurrentChange);
    m_project = projectManager()->getCurrentProject();
    updateProject();
}

CSetStationDialog::~CSetStationDialog()
{
    delete ui;
}

void CSetStationDialog::slotAddStationPb()
{
    StationAttribute stationAttributeDialog;
    if (stationAttributeDialog.addStation())
    {
        m_project->stations.append(stationAttributeDialog.getAddStation(m_project->stations.size()));
        QString dir = getSaveProjectDir() + "/" + m_project->getDirName() + "/" + m_project->stations.last().getDirName() + "/";
        for (const auto &wb : m_project->stations.last().workingProcedures)
        {
            createDir(dir + wb.getDirName());
        }
        updateProject();
    }
}

void CSetStationDialog::slotDelStationPb()
{
    if (m_stationId == -1)
    {
        return;
    }
    m_project->removeStation(m_stationId);
    updateProject();
}

void CSetStationDialog::addStationWorkingProcedure(const CStationWorkingProcedure &p_workingProcedure)
{
    int iRow = ui->Table_Temp->rowCount();
    ui->Table_Temp->insertRow(iRow);
    ui->Table_Temp->setItem(iRow, 1, getTableWidgetItem(iToStr(p_workingProcedure.id)));
    ui->Table_Temp->setItem(iRow, 2, getTableWidgetItem(cStationModeStr[p_workingProcedure.mode]));
    ui->Table_Temp->setItem(iRow, 3, getTableWidgetItem(p_workingProcedure.name));
}

void CSetStationDialog::slotCreateModel()
{
    CStationWorkingProcedure wp = m_project->stations[m_stationId].workingProcedures[m_workingProcedureId];
    switch (wp.mode)
    {
    case csmCircular:
    case csmLine:
        showRoiToolDialog(wp.mode);
        break;
    case csmTemplate:
        showModelToolDialog();
    default:
        break;
    }
}

void CSetStationDialog::slotCurrentChange(int row)
{
    int item = 0;
    for (int i = 0; i < m_project->stations.size(); i++)
    {
        const CStation &station = m_project->stations[i];
        if (row == item)
        {
            m_workingProcedureId = 0;
            assert(m_workingProcedureId >= 0 && m_workingProcedureId < station.workingProcedures.size());
            m_stationId = i;
            return;
        }
        item++;
        if (row < (item + station.workingProcedures.size()))
        {
            m_workingProcedureId = row - item;
            assert(m_workingProcedureId >= 0 && m_workingProcedureId < station.workingProcedures.size());
            m_stationId = i;
            return;
        }
        //每一个工位都多行
        item += station.workingProcedures.size();
    }
    m_stationId = -1;
    m_workingProcedureId = -1;
}

void CSetStationDialog::showRoiToolDialog(const CWorkingProcedureMode &p_stationMode)
{
    CRoiToolDialog roiToolDialog;
    roiToolDialog.setStationMode(p_stationMode);
    roiToolDialog.exec();
}

void CSetStationDialog::showModelToolDialog()
{
}

void CSetStationDialog::updateProject()
{
    m_project->stationSrot();
    ui->Table_Temp->setRowCount(0);

    for (const auto &station : m_project->stations)
    {
        int iRow = ui->Table_Temp->rowCount();
        ui->Table_Temp->insertRow(iRow);
        ui->Table_Temp->setItem(iRow, 0, getTableWidgetItem(iToStr(station.id)));
        ui->Table_Temp->setItem(iRow, 2, getTableWidgetItem(cStationTypeStr[station.type]));
        ui->Table_Temp->setItem(iRow, 3, getTableWidgetItem(station.name));
        for (const auto &wp : station.workingProcedures)
        {
            addStationWorkingProcedure(wp);
        }
    }
}
