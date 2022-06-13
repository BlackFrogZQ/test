#include "SwitchUsers.h"

SwitchUsers::SwitchUsers(QWidget* parent)
    : QWidget(parent), 
    ui(new Ui::SwitchUsers)
{
    ui->setupUi(this);

    QStringList baudList;
    baudList << cnStr("工程师")<<cnStr("管理员")<<cnStr("操作员");
    ui->comboBox_Name->addItems(baudList); 
    ui->comboBox_Name->setCurrentIndex(0);

    QString value = ui->comboBox_Name->currentText();
    if(value == cnStr("工程师")){
        Password = cnStr("en123");
    }
    else if(value == cnStr("管理员")){
        Password = cnStr("ad123");
    }
    else if(value == cnStr("操作员")){
        Password = cnStr("op123");
    }

    connect(ui->btn_Login, &QPushButton::clicked, this, [=](){ Login(); });
    connect(ui->btn_Quit, &QPushButton::clicked, this, [=](){close();});
}

SwitchUsers::~SwitchUsers()
{
    delete ui; 
}

void SwitchUsers::Login()
{
    
    QString mString = ui->Edit_Password->text();
    if( mString == Password){
        Lock_correct = 1;
    }
    else{
        QMessageBox::warning(this, tr("warning"), cnStr("密码不正确"));
    }
}