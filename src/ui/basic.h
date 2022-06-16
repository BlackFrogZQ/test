#pragma once
#include "src/system/basic.h"
#include <QDialog>
#include <QString>
#include <QMessageBox>

static inline int warningBox(QWidget *parent, const QString &title,
                             const QString &text)
{
    return QMessageBox::warning(parent, title, text);
}