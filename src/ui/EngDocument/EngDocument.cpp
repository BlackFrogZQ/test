#include "EngDocument.h"
#include "src/system/projectManager/projectManager.h"

EngDocument::EngDocument(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::EngDocument)
{
    ui->setupUi(this);
    this->setWindowTitle(cnStr("工程文档"));

    //设置Table_Eng
    ui->Table_Eng->setColumnCount(1);
    ui->Table_Eng->setHorizontalHeaderLabels(QStringList() << cnStr("工程名称"));
    ui->Table_Eng->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;color: black;}");
    ui->Table_Eng->setRowCount(0);
    ui->Table_Eng->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Table_Eng->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Table_Eng->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->Table_Eng->verticalHeader()->setHidden(true);
    ui->Table_Eng->resizeRowsToContents();

    //设置lineEdit_CurrName
    ui->lineEdit_CurrName->setReadOnly(true);
    ui->lineEdit_CurrName->setStyleSheet("QLineEdit { background: rgb(0, 255, 255); selection-background-color: rgb(233, 99, 0); }");

    // lineEdit_ProName禁用空格
    QRegExp rx = QRegExp("[A-Za-z0-9-\u4e00-\u9fa5]+$");
    QRegExpValidator *validator = new QRegExpValidator(rx);
    ui->lineEdit_ProName->setValidator(validator);

    connect(ui->btn_AddPro, &QPushButton::clicked, this, &EngDocument::slotAddProject);
    connect(ui->btn_LoadPro, &QPushButton::clicked, this, &EngDocument::slotLoadProject);
    connect(ui->btn_DeletePro, &QPushButton::clicked, this, &EngDocument::slotDeleteProject);
    connect(ui->sanfPB, &QPushButton::clicked, this, &EngDocument::slotScanfProject);
    connect(ui->btn_EngQuit, &QPushButton::clicked, this, [this]()
            { close(); });
    slotScanfProject();
}

EngDocument::~EngDocument()
{
    delete ui;
}

void EngDocument::slotAddProject()
{
    QString ModelName = ui->lineEdit_ProName->text().trimmed();
    if (ModelName.size() == 0)
    {
        warningBox(this, tr("warning"), cnStr("工程名为空，请先输入要创建的工程名"));
        return;
    }
    projectManager()->addProject(ModelName);
    updateProjectTable();
}

void EngDocument::slotLoadProject()
{
    if (isSelectRow())
    {
        projectManager()->setCurrentProject(ui->Table_Eng->currentRow());
        ui->lineEdit_CurrName->setText(projectManager()->getCurrentProject()->name);
    }
}

void EngDocument::slotDeleteProject()
{
    if (!isSelectRow())
    {
        return;
    }

    if (QMessageBox::Yes ==
        QMessageBox::question(this, cnStr("提示"), cnStr("是否删除？"), QMessageBox::No | QMessageBox::Yes, QMessageBox::No))
    {
        slotLoadProject();
        projectManager()->removeCurrentProject();
        updateProjectTable();
    }
}

void EngDocument::slotScanfProject()
{
    projectManager()->loadProject();
    updateProjectTable();
}

void EngDocument::updateProjectTable()
{
    QStringList projectNames = projectManager()->getProjectNames();
    ui->Table_Eng->setRowCount(projectNames.size());
    for (size_t i = 0; i < projectNames.size(); i++)
    {
        ui->Table_Eng->setItem(i, 0, getTableWidgetItem(projectNames[i]));
    }
    if (ui->Table_Eng->rowCount() > 0)
    {
        ui->Table_Eng->selectRow(ui->Table_Eng->rowCount() - 1);
        slotLoadProject();
    }
    ui->lineEdit_ProName->clear();
}

bool EngDocument::isSelectRow()
{
    int id = ui->Table_Eng->currentRow();
    if (id < 0 || id >= ui->Table_Eng->rowCount())
    {
        warningBox(this, tr("warning"), cnStr("未选择，无法加载"));
        return false;
    }
    return true;
}
