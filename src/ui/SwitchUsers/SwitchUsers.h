#pragma once
#include "ui_SwitchUsers.h"
#include "userDef.h"


class CSwitchUsersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSwitchUsersDialog(QWidget *parent = nullptr);
    ~CSwitchUsersDialog();
    bool login();
    CUserType getUserType() const;

protected slots:
    void slotsLoginPB();
    bool isPassword();
private:
    Ui::SwitchUsers *ui;
};