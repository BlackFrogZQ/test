#include "StationAttribute.h"

StationAttribute::StationAttribute(QWidget* parent)
    : QWidget(parent), 
    ui(new Ui::StationAttribute)
{
    ui->setupUi(this);
    mNum = NULL;
    mNum = new MultipleNum;

    //Table_Abute设置
    ui->Table_Abute->setColumnCount(2);
    ui->Table_Abute->setHorizontalHeaderLabels(QStringList()<< cnStr("工位标识")<< cnStr("工位属性"));
    ui->Table_Abute->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;color: black;}");
    ui->Table_Abute->setRowCount(5);
    ui->Table_Abute->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Table_Abute->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Table_Abute->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->Table_Abute->verticalHeader()->setHidden(true);

    QLabel *Label_Match_SingleCircle = new QLabel();
    Label_Match_SingleCircle->setPixmap(QPixmap("./res/CircularPositioning.png"));
    Label_Match_SingleCircle->setAlignment(Qt::AlignHCenter);
    ui->Table_Abute->setCellWidget(0, 0, Label_Match_SingleCircle);
    ui->Table_Abute->resizeRowsToContents();
    ui->Table_Abute->setItem(0, 1, new QTableWidgetItem(cnStr("圆定位")));
    ui->Table_Abute->item(0, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    QLabel *Label_MultiCircle = new QLabel();
    Label_MultiCircle->setPixmap(QPixmap(cnStr("./res/多圆定位.png")));
    Label_MultiCircle->setAlignment(Qt::AlignHCenter);
    ui->Table_Abute->setCellWidget(1, 0, Label_MultiCircle);
    ui->Table_Abute->resizeRowsToContents();
    ui->Table_Abute->setItem(1, 1, new QTableWidgetItem(cnStr("多组圆定位")));
    ui->Table_Abute->item(1, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    QLabel *Label_OneLineMeetsTwoSides = new QLabel();
    Label_OneLineMeetsTwoSides->setPixmap(QPixmap(cnStr("./res/一线交两边定位.png")));
    Label_OneLineMeetsTwoSides->setAlignment(Qt::AlignHCenter);
    ui->Table_Abute->setCellWidget(2, 0, Label_OneLineMeetsTwoSides);
    ui->Table_Abute->resizeRowsToContents();
    ui->Table_Abute->setItem(2, 1, new QTableWidgetItem(cnStr("一线交两边定位")));
    ui->Table_Abute->item(2, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    //lineEdit_StaName禁用空格
    QRegExp rx = QRegExp("[A-Za-z0-9-\u4e00-\u9fa5]+$");
    QRegExpValidator* validator = new QRegExpValidator(rx);
    ui->lineEdit_StaName->setValidator(validator);

    connect(ui->Table_Abute, &QTableWidget::cellClicked, this,  [=](){CurrentRow_Abute();});
    connect(ui->btn_ON, &QPushButton::clicked, this, [=](){AddStation();});
    connect(mNum, &MultipleNum::Send_Num, this, &StationAttribute::Receive_Num, Qt::QueuedConnection);
    connect(ui->btn_OFF, &QPushButton::clicked, this, [=](){close();});
}

StationAttribute::~StationAttribute()
{
    if(mNum){
        delete mNum;
        mNum = NULL;
    }
    delete ui; 
}

void StationAttribute::CurrentRow_Abute()
{
    SelectAbute_Row=ui->Table_Abute->currentRow();
    QString StrollName = ui->Table_Abute->item(SelectAbute_Row, 1)->text().trimmed();
    ui->lineEdit_StaName->setText(StrollName);
}

void StationAttribute::Receive_Num(int NUM_tem)
{
    NUM = NUM_tem;
    emit Send_Abute(Abute, AbuteName, NUM);
    NUM = 0;
    close();
}

void StationAttribute::AddStation()
{
    switch (SelectAbute_Row)
    {
    case 0:
        Abute = cnStr("圆定位");
        break;
    case 1:
        Abute = cnStr("多组圆定位");
        mNum->setWindowModality(Qt::ApplicationModal);
        mNum->show();
        NUM = 1;
        break;
    case 2:
        Abute = cnStr("一线交两边定位");
        break;
    }

    AbuteName = ui->lineEdit_StaName->text();
    if(NUM != 0){
        return;
    }
    else{
        emit Send_Abute(Abute, AbuteName, NUM);
        close();
    }
}