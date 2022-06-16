#include "SwitchUsers.h"

const QStringList cUserInitPassword = {cnStr("en123"), cnStr("ad123"), cnStr("op123")};

CSwitchUsersDialog::CSwitchUsersDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::SwitchUsers)
{
    ui->setupUi(this);

    ui->comboBox_Name->addItems(cUserTypeStr);
    ui->comboBox_Name->setCurrentIndex(0);
    connect(ui->btn_Login, &QPushButton::clicked, this, &CSwitchUsersDialog::slotsLoginPB);
    connect(ui->btn_Quit, &QPushButton::clicked, this, [this]()
            { close(); });
}

CSwitchUsersDialog::~CSwitchUsersDialog()
{
    delete ui;
}

bool CSwitchUsersDialog::login()
{
    this->exec();
    return isPassword();
}

CUserType CSwitchUsersDialog::getUserType() const
{
    return CUserType(ui->comboBox_Name->currentIndex());
}

void CSwitchUsersDialog::slotsLoginPB()
{
    if (isPassword())
    {
        this->close();
    }
    else
    {
        warningBox(this, "warning", cnStr("密码不正确"));
    }
}

bool CSwitchUsersDialog::isPassword()
{
    //实际可根据currentId CUserType 获取密码在校验
    return ui->Edit_Password->text() == cUserInitPassword[ui->comboBox_Name->currentIndex()];
}
