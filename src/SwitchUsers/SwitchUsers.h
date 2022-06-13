#pragma once
#include "ui_SwitchUsers.h"
#include <QWidget>
#include <QMessageBox>
#include <QDebug>

#define cnStr(str) QString::fromLocal8Bit(str)

class SwitchUsers : public QWidget  {
    Q_OBJECT

public:
    explicit SwitchUsers(QWidget* parent = nullptr);
    ~SwitchUsers();

    void Login();

private:
    Ui::SwitchUsers *ui;

    QString Password;
    int Lock_correct;
};