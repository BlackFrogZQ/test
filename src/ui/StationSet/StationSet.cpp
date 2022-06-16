#include "StationSet.h"

StationSet::StationSet(QWidget* parent)
    : QDialog(parent),
    ui(new Ui::StationSet)
{
    ui->setupUi(this);

    mStationAbute = NULL;
    mStationAbute = new StationAttribute;

    //设置Table_Eng属性
    ui->Table_Temp->setColumnCount(5);
    ui->Table_Temp->setHorizontalHeaderLabels(QStringList()<< cnStr("工位")<< cnStr("工序")<< cnStr("模式")<< cnStr("名称")<< cnStr("状态"));
    ui->Table_Temp->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;color: black;}");
    ui->Table_Temp->setRowCount(0);
    ui->Table_Temp->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Table_Temp->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Table_Temp->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->Table_Temp->verticalHeader()->setHidden(true);
    ui->Table_Temp->resizeRowsToContents();
    //Table_Eng从文件读取数据
    QFile f("./src/StationSet/SaveTemplateRecord.txt");
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream output(&f);
    output.setCodec("UTF-8");
    QHash<QString, QString> varHash;
    QString lineStr;
    QStringList lineList;
    varHash.clear();
    lineList.clear();
    ui->Table_Temp->setRowCount(0);
    //遍历文件
    while (!output.atEnd()){
        QString str = output.readLine();
        if (str.size() == 0){continue;}
        lineList = str.split('*');

        int rowCount = ui->Table_Temp->rowCount();
        ui->Table_Temp->insertRow(rowCount);

        for (int i = 0; i < lineList.size(); i++){
            QTableWidgetItem *item = new QTableWidgetItem(lineList[i]);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->Table_Temp->setItem(rowCount, i, item);
            if(i == 0){
                if(lineList[i] != cnStr(" ")){
                    Row_Station++;
                }
            }
        }
    }
    f.close();

    connect(ui->btn_AddStation, &QPushButton::clicked, this, [=](){ShowStation();});
    connect(mStationAbute, &StationAttribute::Send_Abute, this, &StationSet::UpdateTable, Qt::QueuedConnection);
    connect(ui->btn_EditStation, &QPushButton::clicked, this, [=](){CreateModel();});
    connect(ui->btn_ExitEdit, &QPushButton::clicked, this, [=](){close();});
}

StationSet::~StationSet()
{
    if(mStationAbute){
        delete mStationAbute;
        mStationAbute = NULL;
    }
    delete ui; 
}

void StationSet::ShowStation()
{
    mStationAbute->setWindowModality(Qt::ApplicationModal);
    mStationAbute->show();
}

void StationSet::UpdateTable(QString Abute, QString AbuteName, int NUM)
{
    //行数不够再添加一行
    int iRow = ui->Table_Temp->rowCount();
    ui->Table_Temp->insertRow(iRow);

    //更新表格
    QString Station = cnStr("工位:") + QString::number(Row_Station);
    Row_Station++;
    ui->Table_Temp->setItem(iRow, 0, new QTableWidgetItem(Station));
    ui->Table_Temp->item(iRow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->Table_Temp->setItem(iRow, 1, new QTableWidgetItem(cnStr(" ")));
    ui->Table_Temp->item(iRow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->Table_Temp->setItem(iRow, 2, new QTableWidgetItem(Abute));
    ui->Table_Temp->item(iRow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->Table_Temp->setItem(iRow, 3, new QTableWidgetItem(AbuteName));
    ui->Table_Temp->item(iRow, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    if(Abute == cnStr("圆定位")){
        for(int i = 1; i <= 2; i++){
            if(i == 1){
                int iRow = ui->Table_Temp->rowCount();
                ui->Table_Temp->insertRow(iRow);
                QString process = cnStr("Nodel") + QString::number(i);
                ui->Table_Temp->setItem(iRow, 0, new QTableWidgetItem(cnStr(" ")));
                ui->Table_Temp->item(iRow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 1, new QTableWidgetItem(process));
                ui->Table_Temp->item(iRow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 2, new QTableWidgetItem(cnStr("模板")));
                ui->Table_Temp->item(iRow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 3, new QTableWidgetItem(cnStr("模板")));
                ui->Table_Temp->item(iRow, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
            else{
                int iRow = ui->Table_Temp->rowCount();
                ui->Table_Temp->insertRow(iRow);
                QString process = cnStr("Nodel") + QString::number(i);
                ui->Table_Temp->setItem(iRow, 0, new QTableWidgetItem(cnStr(" ")));
                ui->Table_Temp->item(iRow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 1, new QTableWidgetItem(process));
                ui->Table_Temp->item(iRow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 2, new QTableWidgetItem(cnStr("圆")));
                ui->Table_Temp->item(iRow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 3, new QTableWidgetItem(cnStr("圆")));
                ui->Table_Temp->item(iRow, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
        }
    }
    else if(Abute == cnStr("多组圆定位")){
        for(int i = 1; i <= NUM; i++){
            if(i == 1){
                int iRow = ui->Table_Temp->rowCount();
                ui->Table_Temp->insertRow(iRow);
                QString process = cnStr("Nodel") + QString::number(i);
                ui->Table_Temp->setItem(iRow, 0, new QTableWidgetItem(cnStr(" ")));
                ui->Table_Temp->item(iRow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 1, new QTableWidgetItem(process));
                ui->Table_Temp->item(iRow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 2, new QTableWidgetItem(cnStr("模板")));
                ui->Table_Temp->item(iRow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 3, new QTableWidgetItem(cnStr("模板")));
                ui->Table_Temp->item(iRow, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
            else{
                int iRow = ui->Table_Temp->rowCount();
                ui->Table_Temp->insertRow(iRow);
                QString process = cnStr("Nodel") + QString::number(i);
                ui->Table_Temp->setItem(iRow, 0, new QTableWidgetItem(cnStr(" ")));
                ui->Table_Temp->item(iRow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 1, new QTableWidgetItem(process));
                ui->Table_Temp->item(iRow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 2, new QTableWidgetItem(cnStr("圆")));
                ui->Table_Temp->item(iRow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                QString Sideline = cnStr("圆") + QString::number(i-1);
                ui->Table_Temp->setItem(iRow, 3, new QTableWidgetItem(Sideline));
                ui->Table_Temp->item(iRow, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
        }
    }
    else if(Abute == cnStr("一线交两边定位")){
        for(int i = 1; i <= 4; i++){
            if(i == 1){
                int iRow = ui->Table_Temp->rowCount();
                ui->Table_Temp->insertRow(iRow);
                QString process = cnStr("Nodel") + QString::number(i);
                ui->Table_Temp->setItem(iRow, 0, new QTableWidgetItem(cnStr(" ")));
                ui->Table_Temp->item(iRow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 1, new QTableWidgetItem(process));
                ui->Table_Temp->item(iRow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 2, new QTableWidgetItem(cnStr("模板")));
                ui->Table_Temp->item(iRow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 3, new QTableWidgetItem(cnStr("模板")));
                ui->Table_Temp->item(iRow, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
            else{
                int iRow = ui->Table_Temp->rowCount();
                ui->Table_Temp->insertRow(iRow);
                QString process = cnStr("Nodel") + QString::number(i);
                ui->Table_Temp->setItem(iRow, 0, new QTableWidgetItem(cnStr(" ")));
                ui->Table_Temp->item(iRow, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 1, new QTableWidgetItem(process));
                ui->Table_Temp->item(iRow, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->Table_Temp->setItem(iRow, 2, new QTableWidgetItem(cnStr("线")));
                ui->Table_Temp->item(iRow, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                QString Sideline = cnStr("线") + QString::number(i-1);
                ui->Table_Temp->setItem(iRow, 3, new QTableWidgetItem(Sideline));
                ui->Table_Temp->item(iRow, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
        }
    }

    //保存表格内容到txt
    QString filename = "./src/StationSet/SaveTemplateRecord.txt";
    QFile file("./src/StationSet/SaveTemplateRecord.txt");
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("double file edit"), tr("no write ").arg(filename).arg(file.errorString()));
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    int romCount = ui->Table_Temp->rowCount();
    for (int i = 0; i < romCount; i++)
    {
        QString rowstring;
        for (int j = 0; j < 4; j++)
        {
            if(ui->Table_Temp->item(i, j)->text() == NULL){
                rowstring += ui->Table_Temp->item(i, j)->text()+ " " + "*";
            }
            else{
                rowstring += ui->Table_Temp->item(i, j)->text() + "*";
            }
        }
        rowstring = rowstring + "\n";
        out << rowstring;
    }
    file.close();
}

void StationSet::CreateModel()
{
    if(ui->Table_Temp->item(ui->Table_Temp->currentRow(), 2)->text() == cnStr("模板")){
        QString s, Station_Num;
        s = ui->Table_Temp->item(ui->Table_Temp->currentRow()-1, 0)->text();//获取工位号
        Station_Num =s[3];//获取工位号
        emit Send_ShowModelTool(Station_Num);
    }
    else if(ui->Table_Temp->item(ui->Table_Temp->currentRow(), 2)->text() == cnStr("圆")){
        QString s, Station_Num;
        int ROI_StationNum;
        //获取工位号
        for(int i=1;i<30;i++){
            if(ui->Table_Temp->item(ui->Table_Temp->currentRow()-i, 0)->text() == cnStr(" ")){
                continue;
            }
            else{
                s = ui->Table_Temp->item(ui->Table_Temp->currentRow()-i, 0)->text();
                Station_Num =s[3];
                break;
            }
        }
        //获取工序号
        for(int i=1;i<30;i++){
            if(ui->Table_Temp->item(ui->Table_Temp->currentRow()-i, 2)->text() == cnStr("模板")){
                ROI_StationNum = i;
                break;
            }
        }
        CircularOrLine = cnStr("圆");
        emit Send_ShowROITool(CircularOrLine, Station_Num, ROI_StationNum);
    }
    else if(ui->Table_Temp->item(ui->Table_Temp->currentRow(), 2)->text() == cnStr("线")){
        QString s, Station_Num;
        int ROI_StationNum;
        //获取工位号
        for(int i=1;i<30;i++){
            if(ui->Table_Temp->item(ui->Table_Temp->currentRow()-i, 0)->text() == cnStr(" ")){
                continue;
            }
            else{
                s = ui->Table_Temp->item(ui->Table_Temp->currentRow()-i, 0)->text();
                Station_Num =s[3];
                break;
            }
        }
        //获取工序号
        for(int i=1;i<30;i++){
            if(ui->Table_Temp->item(ui->Table_Temp->currentRow()-i, 2)->text() == cnStr("模板")){
                ROI_StationNum = i;
                break;
            }
        }
        CircularOrLine = cnStr("线");
        emit Send_ShowROITool(CircularOrLine, Station_Num, ROI_StationNum);
    }
    else{
        return;
    }
}

void StationSet::ObtainStationAbute(int MaxScore_Place)
{
    int rowCount_Sum = ui->Table_Temp->rowCount();
    for(int i = 0; i < rowCount_Sum; i++){
        QString s, Station_Num, MScoreAbuteName;
        s = ui->Table_Temp->item(i, 0)->text();
        Station_Num =s[3];
        QString str = QString::number(MaxScore_Place);  
        if(Station_Num == str){
            MScoreAbuteName = ui->Table_Temp->item(i, 2)->text();//获取工位属性

            int WorkStep_Sum = 0;//获取工步总数
            for(int j = i+2; j < rowCount_Sum; j++){
                if(ui->Table_Temp->item(j, 0)->text() == cnStr(" ")){
                    WorkStep_Sum++;
                }
                else{
                    break;
                }
            }

            emit Send_MScoreAbuteName(MScoreAbuteName, MaxScore_Place, WorkStep_Sum);
            break;
        }
    }
}