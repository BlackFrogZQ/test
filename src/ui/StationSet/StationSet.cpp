#include "StationSet.h"
#include "StationAttribute.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
CSetStationDialog::CSetStationDialog(QWidget *parent)
    : QDialog(parent),
      m_stationCount(0),
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
    // Table_Eng从文件读取数据
    QFile f("./src/StationSet/SaveTemplateRecord.txt");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream output(&f);
    output.setCodec("UTF-8");
    QStringList lineList;
    //遍历文件
    while (!output.atEnd())
    {
        QString str = output.readLine();
        if (str.size() == 0)
        {
            continue;
        }
        lineList = str.split('*');

        int rowCount = ui->Table_Temp->rowCount();
        ui->Table_Temp->insertRow(rowCount);

        for (int i = 0; i < lineList.size(); i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(lineList[i]);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->Table_Temp->setItem(rowCount, i, item);
            if (i == 0)
            {
                if (lineList[i] != cnStr(" "))
                {
                    m_stationCount++;
                }
            }
        }
    }
    f.close();

    connect(ui->btn_AddStation, &QPushButton::clicked, this, &CSetStationDialog::slotAddStationPb);
    connect(ui->btn_EditStation, &QPushButton::clicked, this, [=]()
            { CreateModel(); });
    connect(ui->btn_ExitEdit, &QPushButton::clicked, this, [=]()
            { close(); });
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
        addStationTitle(stationAttributeDialog.getStationType(),stationAttributeDialog.getStationName());
        switch (stationAttributeDialog.getStationType())
        {
        case stCircularPositioning:
            addCircularPositioning();
            break;
        case stMultiCirclePositioning:
            addMultiCirclePositioning(stationAttributeDialog.getMultiCircleCount());
            break;
        case stOneLineCrossingTwoSidesPositioning:
            addOneLineCrossingTwoSidesPositioning();
            break;
        default:
            break;
        }
    }
}

void CSetStationDialog::addCircularPositioning()
{
    addStationWorkingProcedure(
        CStationWorkingProcedure(2, CStationMode::csmCircular, cnStr("圆")));
}

void CSetStationDialog::addMultiCirclePositioning(const int &p_multiCircleCount)
{
    for (size_t i = 0; i < p_multiCircleCount; i++)
    {
        addStationWorkingProcedure(
            CStationWorkingProcedure(i + 2, CStationMode::csmCircular, cnStr("圆")));
    }
}

void CSetStationDialog::addOneLineCrossingTwoSidesPositioning()
{
    for (size_t i = 0; i < 4; i++)
    {
        addStationWorkingProcedure(
            CStationWorkingProcedure(i + 2, CStationMode::csmLine, cnStr("线")));
    }
}

void CSetStationDialog::addStationWorkingProcedure(const CStationWorkingProcedure &p_workingProcedure)
{
    int iRow = ui->Table_Temp->rowCount();
    ui->Table_Temp->insertRow(iRow);
    ui->Table_Temp->setItem(iRow, 1, getTableWidgetItem(cnStr("工序%1").arg(p_workingProcedure.id)));
    ui->Table_Temp->setItem(iRow, 2, getTableWidgetItem(cStationModeStr[p_workingProcedure.mode]));
    ui->Table_Temp->setItem(iRow, 3, getTableWidgetItem(p_workingProcedure.name));
}

void CSetStationDialog::addStationTitle(const CStationType &p_stationType, const QString &p_stationName)
{
    int iRow = ui->Table_Temp->rowCount();
    ui->Table_Temp->insertRow(iRow);
    ui->Table_Temp->setItem(iRow, 0, getTableWidgetItem(cnStr("工位%1").arg(m_stationCount)));
    ui->Table_Temp->setItem(iRow, 2, getTableWidgetItem(cStationTypeStr[p_stationType]));
    ui->Table_Temp->setItem(iRow, 3, getTableWidgetItem(p_stationName));
    addStationWorkingProcedure(
        CStationWorkingProcedure(1, CStationMode::csmTemplate, cnStr("模板")));
    m_stationCount++;
}

// void StationSet::UpdateTable(QString Abute, QString AbuteName, int NUM)
// {
//     //保存表格内容到txt
//     QString filename = "./src/StationSet/SaveTemplateRecord.txt";
//     QFile file("./src/StationSet/SaveTemplateRecord.txt");
//     if (!file.open(QFile::WriteOnly | QFile::Text))
//     {
//         QMessageBox::warning(this, tr("double file edit"), tr("no write ").arg(filename).arg(file.errorString()));
//         return;
//     }
//     QTextStream out(&file);
//     out.setCodec("UTF-8");
//     int romCount = ui->Table_Temp->rowCount();
//     for (int i = 0; i < romCount; i++)
//     {
//         QString rowstring;
//         for (int j = 0; j < 4; j++)
//         {
//             if (ui->Table_Temp->item(i, j)->text() == NULL)
//             {
//                 rowstring += ui->Table_Temp->item(i, j)->text() + " " + "*";
//             }
//             else
//             {
//                 rowstring += ui->Table_Temp->item(i, j)->text() + "*";
//             }
//         }
//         rowstring = rowstring + "\n";
//         out << rowstring;
//     }
//     file.close();
// }

void CSetStationDialog::CreateModel()
{
    if (ui->Table_Temp->item(ui->Table_Temp->currentRow(), 2)->text() == cnStr("模板"))
    {
        QString s, Station_Num;
        s = ui->Table_Temp->item(ui->Table_Temp->currentRow() - 1, 0)->text(); //获取工位号
        Station_Num = s[3];                                                    //获取工位号
        emit Send_ShowModelTool(Station_Num);
    }
    else if (ui->Table_Temp->item(ui->Table_Temp->currentRow(), 2)->text() == cnStr("圆"))
    {
        QString s, Station_Num;
        int ROI_StationNum;
        //获取工位号
        for (int i = 1; i < 30; i++)
        {
            if (ui->Table_Temp->item(ui->Table_Temp->currentRow() - i, 0) != nullptr)
            {
                s = ui->Table_Temp->item(ui->Table_Temp->currentRow() - i, 0)->text();
                Station_Num = s[3];
                break;
            }
        }
        //获取工序号
        for (int i = 1; i < 30; i++)
        {
            if (ui->Table_Temp->item(ui->Table_Temp->currentRow() - i, 2)->text() == cnStr("模板"))
            {
                ROI_StationNum = i;
                break;
            }
        }
        CircularOrLine = cnStr("圆");
        // emit Send_ShowROITool(CircularOrLine, Station_Num, ROI_StationNum);
    }
    else if (ui->Table_Temp->item(ui->Table_Temp->currentRow(), 2)->text() == cnStr("线"))
    {
        QString s, Station_Num;
        int ROI_StationNum;
        //获取工位号
        for (int i = 1; i < 30; i++)
        {
            if (ui->Table_Temp->item(ui->Table_Temp->currentRow() - i, 0)->text() == cnStr(" "))
            {
                continue;
            }
            else
            {
                s = ui->Table_Temp->item(ui->Table_Temp->currentRow() - i, 0)->text();
                Station_Num = s[3];
                break;
            }
        }
        //获取工序号
        for (int i = 1; i < 30; i++)
        {
            if (ui->Table_Temp->item(ui->Table_Temp->currentRow() - i, 2)->text() == cnStr("模板"))
            {
                ROI_StationNum = i;
                break;
            }
        }
        CircularOrLine = cnStr("线");
        // emit Send_ShowROITool(CircularOrLine, Station_Num, ROI_StationNum);
    }
}

void CSetStationDialog::ObtainStationAbute(int MaxScore_Place)
{
    int rowCount_Sum = ui->Table_Temp->rowCount();
    for (int i = 0; i < rowCount_Sum; i++)
    {
        QString s, Station_Num, MScoreAbuteName;
        s = ui->Table_Temp->item(i, 0)->text();
        Station_Num = s[3];
        QString str = QString::number(MaxScore_Place);
        if (Station_Num == str)
        {
            MScoreAbuteName = ui->Table_Temp->item(i, 2)->text(); //获取工位属性

            int WorkStep_Sum = 0; //获取工步总数
            for (int j = i + 2; j < rowCount_Sum; j++)
            {
                if (ui->Table_Temp->item(j, 0)->text() == cnStr(" "))
                {
                    WorkStep_Sum++;
                }
                else
                {
                    break;
                }
            }

            emit Send_MScoreAbuteName(MScoreAbuteName, MaxScore_Place, WorkStep_Sum);
            break;
        }
    }
}

QTableWidgetItem *CSetStationDialog::getTableWidgetItem(const QString &p_text, int p_textAlignment)
{
    QTableWidgetItem *pItem = new QTableWidgetItem(p_text);
    pItem->setTextAlignment(p_textAlignment);
    return pItem;
}
