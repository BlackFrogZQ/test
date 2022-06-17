#pragma once
#include "ui_EngDocument.h"
#include "../basic.h"

class EngDocument : public QDialog
{
    Q_OBJECT
public:
    EngDocument(QWidget *parent = nullptr);
    ~EngDocument();

protected slots:
    void slotAddProject();
    void slotLoadProject();
    void slotDeleteProject();
    void slotScanfProject();
protected:
    void updateProjectTable();
    bool isSelectRow();
private:
    Ui::EngDocument *ui;
};