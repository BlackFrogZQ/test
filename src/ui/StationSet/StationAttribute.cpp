#include "StationAttribute.h"

StationAttribute::StationAttribute(QWidget *parent)
    : QDialog(parent),
      m_isAdd(false),
      ui(new Ui::StationAttribute)
{
    ui->setupUi(this);

    // Table_Abute设置
    ui->Table_Abute->setColumnCount(2);
    ui->Table_Abute->setHorizontalHeaderLabels(QStringList() << cnStr("工位标识") << cnStr("工位属性"));
    ui->Table_Abute->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;color: black;}");
    ui->Table_Abute->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Table_Abute->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Table_Abute->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->Table_Abute->verticalHeader()->setHidden(true);

    for (size_t i = 0; i < CStationType::stMax; i++)
    {
        addStationTypeItem(cStationTypeIcon[i], cStationTypeStr[i]);
    }

    // lineEdit_StaName禁用空格
    QRegExp rx = QRegExp("[A-Za-z0-9-\u4e00-\u9fa5]+$");
    QRegExpValidator *validator = new QRegExpValidator(rx);
    ui->stationName->setValidator(validator);
    connect(ui->Table_Abute, &QTableWidget::cellClicked, this, [this]()
            {
                int id = ui->Table_Abute->currentRow();
                (id==CStationType::stMultiCirclePositioning)?ui->multiCircleCountSB->show():ui->multiCircleCountSB->hide();
                ui->stationName->setPlaceholderText(cStationTypeStr[ui->Table_Abute->currentRow()]); });
    connect(ui->btn_ON, &QPushButton::clicked, this, [this]()
            { m_isAdd = true;close(); });
    connect(ui->btn_OFF, &QPushButton::clicked, this, [this]()
            { close(); });
}

StationAttribute::~StationAttribute()
{
    delPtr(ui);
}

bool StationAttribute::addStation()
{
    m_isAdd = false;
    this->exec();
    return m_isAdd;
}

CStation StationAttribute::getAddStation(const int &p_id)
{
    m_station.workingProcedures.clear();
    m_station.id = p_id;
    m_station.type = CStationType(ui->Table_Abute->currentRow());
    m_station.name = ui->stationName->text().trimmed();
    m_station.workingProcedures.append(
        CStationWorkingProcedure(0, CWorkingProcedureMode::csmTemplate, cnStr("模板")));
    switch (m_station.type)
    {
    case stCircularPositioning:
        addCircularPositioning();
        break;
    case stMultiCirclePositioning:
        addMultiCirclePositioning();
        break;
    case stOneLineCrossingTwoSidesPositioning:
        addOneLineCrossingTwoSidesPositioning();
        break;
    default:
        break;
    }
    return m_station;
}

void StationAttribute::addCircularPositioning()
{
    m_station.workingProcedures.append(CStationWorkingProcedure(m_station.workingProcedures.size(), CWorkingProcedureMode::csmCircular, cnStr("圆")));
}

void StationAttribute::addMultiCirclePositioning()
{
    int id = m_station.workingProcedures.size();
    for (size_t i = 0; i < ui->multiCircleCountSB->value(); i++)
    {
        m_station.workingProcedures.append(CStationWorkingProcedure(id + i, CWorkingProcedureMode::csmCircular, cnStr("圆")));
    }
}

void StationAttribute::addOneLineCrossingTwoSidesPositioning()
{
    int id = m_station.workingProcedures.size();
    for (size_t i = 0; i < 4; i++)
    {
        m_station.workingProcedures.append(
            CStationWorkingProcedure(i + id, CWorkingProcedureMode::csmLine, cnStr("线")));
    }
}

void StationAttribute::addStationTypeItem(const QString &p_icon, const QString &p_cnName)
{
    QLabel *lb = new QLabel;
    lb->setPixmap(QPixmap(p_icon));
    lb->setAlignment(Qt::AlignHCenter);
    int row = ui->Table_Abute->rowCount();
    ui->Table_Abute->insertRow(row);
    ui->Table_Abute->setCellWidget(row, 0, lb);
    QTableWidgetItem *pItem = new QTableWidgetItem(p_cnName);
    pItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->Table_Abute->setItem(row, 1, pItem);
    ui->Table_Abute->resizeRowsToContents();
}

