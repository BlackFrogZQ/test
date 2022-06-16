#pragma once
#include "ui_EngDocument.h"
#include "../basic.h"

class EngDocument : public QDialog  {
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