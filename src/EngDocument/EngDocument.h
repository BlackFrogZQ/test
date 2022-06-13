#pragma once
#include "ui_EngDocument.h"
#include <QWidget>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDebug>
#define cnStr(str) QString::fromLocal8Bit(str)

class EngDocument : public QWidget  {
    Q_OBJECT

public:
    explicit EngDocument(QWidget* parent = nullptr);
    ~EngDocument();

    void AddProject();
    void LoadProject();
    void DeleteProject();

signals:
    void Send_CurrentProject(int current_Row, QString CurrName_Text);

private:
    Ui::EngDocument *ui;
};