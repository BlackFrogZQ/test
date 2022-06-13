#include "EngDocument.h"

EngDocument::EngDocument(QWidget* parent)
    : QWidget(parent), 
    ui(new Ui::EngDocument)
{
    ui->setupUi(this);
    this->setWindowTitle(cnStr("工程文档"));

    //设置Table_Eng
    ui->Table_Eng->setColumnCount(1);
    ui->Table_Eng->setHorizontalHeaderLabels(QStringList()<< cnStr("工程名称"));
    ui->Table_Eng->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;color: black;}");
    ui->Table_Eng->setRowCount(0);
    ui->Table_Eng->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Table_Eng->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Table_Eng->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->Table_Eng->verticalHeader()->setHidden(true);
    ui->Table_Eng->resizeRowsToContents();
    //向Table_Eng中添加已经保存的模板
    //从文件读取数据
    QFile f("./src/EngDocument/SaveEngDocument.txt");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream output(&f);
    output.setCodec("UTF-8");
    QHash<QString, QString> varHash;
    QString lineStr;      //文件的每一行的字符串
    QStringList lineList; //整行字符串，分割处理为单个字符串，存入到表中
    varHash.clear();
    lineList.clear();  //操作前，清空
    //遍历文件
    while (!output.atEnd()){
        QString str = output.readLine().trimmed();
        if (str.size() == 0){
            continue;}
        lineList = str.split(' ');
        int rowCount = ui->Table_Eng->rowCount();
        ui->Table_Eng->insertRow(rowCount);
        for (int i = 0; i < lineList.size(); i++){
            QTableWidgetItem *item = new QTableWidgetItem(lineList[i]);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->Table_Eng->setItem(rowCount, i, item);
        }
    }
    f.close();

    //设置lineEdit_CurrName
    ui->lineEdit_CurrName->setReadOnly(true);
    ui->lineEdit_CurrName->setStyleSheet("QLineEdit { background: rgb(0, 255, 255); selection-background-color: rgb(233, 99, 0); }");
    QFile f_CurrName("./src/EngDocument/CurrentProject.txt");
    f_CurrName.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray line = f_CurrName.readLine().trimmed();
    QString str(line);
    ui->lineEdit_CurrName->setText(str);
    f_CurrName.close();

    //lineEdit_ProName禁用空格
    QRegExp rx = QRegExp("[A-Za-z0-9-\u4e00-\u9fa5]+$");
    QRegExpValidator* validator = new QRegExpValidator(rx);
    ui->lineEdit_ProName->setValidator(validator);

    connect(ui->btn_AddPro, &QPushButton::clicked, this, [=](){AddProject();});
    connect(ui->btn_LoadPro, &QPushButton::clicked, this, [=](){LoadProject();});
    connect(ui->btn_DeletePro, &QPushButton::clicked, this, [=](){DeleteProject();});
    connect(ui->btn_EngQuit, &QPushButton::clicked, this, [=](){close();});
}

EngDocument::~EngDocument()
{
    delete ui; 
}

void EngDocument::AddProject()
{
    QString ModelName = ui->lineEdit_ProName->text();
    if (ui->lineEdit_ProName->text().isEmpty())
    {
        QMessageBox::warning(this, tr("warning"), cnStr("工程名为空，请先输入要创建的工程名"));
        return;
    }

    //行数不够再添加一行
    int iRow = ui->Table_Eng->rowCount();
    ui->Table_Eng->insertRow(iRow); //总行数增加1

    ModelName = ModelName.trimmed();                  //返回一个字符串,移除从一开始到结尾的空白。也去掉头尾的空白
    ModelName = ModelName.remove(QRegExp("\\s"));     //删除所有空格
    ModelName = ModelName.remove(QRegExp("\\s* +$")); //去除字符串后面空格
    ui->Table_Eng->setItem(iRow, 0, new QTableWidgetItem(ModelName));
    ui->Table_Eng->item(iRow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    //将表格数据保存到文本中
    QString filename = "./src/EngDocument/SaveEngDocument.txt";
    QFile file("./src/EngDocument/SaveEngDocument.txt");
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("double file edit"), tr("no write ").arg(filename).arg(file.errorString()));
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    int romCount = ui->Table_Eng->rowCount();
    for (int i = 0; i < romCount; i++)
    {
        QString rowstring;
        rowstring += ui->Table_Eng->item(i, 0)->text().trimmed();
        rowstring = rowstring.trimmed() + "\n";
        out << rowstring;
    }
    file.close();
}

void EngDocument::LoadProject()
{
    //lineEdit_CurrName读取加载行表格的内容
    QString rowstring;
    int Current_Row=ui->Table_Eng->currentRow();
    rowstring = ui->Table_Eng->item(Current_Row, 0)->text().trimmed();
    ui->lineEdit_CurrName->setText(rowstring);

    //将更新后的表格数据保存到文本中
    QString filename = "./src/EngDocument/CurrentProject.txt";
    QFile file("./src/EngDocument/CurrentProject.txt");
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, tr("double file edit"), tr("no write ").arg(filename).arg(file.errorString()));
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    QString CurrName_Text;
    CurrName_Text = ui->lineEdit_CurrName->text().trimmed();
    out << rowstring;
    file.close();

    //发送当前工程到主界面
    emit Send_CurrentProject(Current_Row, CurrName_Text);
}

void EngDocument::DeleteProject()
{
    if(QMessageBox::Yes == QMessageBox::question(this, cnStr("提示"), cnStr("是否删除本行模板？"), 
    QMessageBox::No | QMessageBox::Yes, QMessageBox::No)){
        //删除选择的行
        int rowCount_Sum = ui->Table_Eng->rowCount();
        int current_Row=ui->Table_Eng->currentRow();
        ui->Table_Eng->removeRow(current_Row);

        //将更新后的表格数据保存到文本中
        QString filename = "./src/EngDocument/SaveEngDocument.txt";
        QFile file("./src/EngDocument/SaveEngDocument.txt");
        if (!file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this, tr("double file edit"), tr("no write ").arg(filename).arg(file.errorString()));
            return;
        }
        QTextStream out(&file);
        out.setCodec("UTF-8");
        int romCount = ui->Table_Eng->rowCount();
        for (int i = 0; i < romCount; i++){
            QString rowstring;
            rowstring += ui->Table_Eng->item(i, 0)->text().trimmed();
            rowstring = rowstring.trimmed() + "\n";
            out << rowstring;
        }
        file.close();

        //如果删除的是最后一行，则lineEdit_CurrName的内容为更新后表格最后一行的内容
        if(current_Row == rowCount_Sum-1){
            QString rowstring;
            rowstring = ui->Table_Eng->item(current_Row-1, 0)->text().trimmed();
            ui->lineEdit_CurrName->setText(rowstring);

            QString filename = "./src/EngDocument/CurrentProject.txt";
            QFile file("./src/EngDocument/CurrentProject.txt");
            if (!file.open(QFile::WriteOnly | QFile::Text)){
                QMessageBox::warning(this, tr("double file edit"), tr("no write ").arg(filename).arg(file.errorString()));
                return;
            }
            QTextStream out(&file);
            out.setCodec("UTF-8");
            QString CurrName_Text;
            CurrName_Text = ui->lineEdit_CurrName->text().trimmed();
            out << rowstring;
            file.close();
        }
    }
}