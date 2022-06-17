#pragma once
#include "src/system/basic.h"
#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QTableWidgetItem>

static inline int warningBox(QWidget *parent, const QString &title,
                             const QString &text)
{
    return QMessageBox::warning(parent, title, text);
}

static inline QTableWidgetItem *getTableWidgetItem(const QString &p_text, int p_textAlignment = Qt::AlignCenter)
{
    QTableWidgetItem *pItem = new QTableWidgetItem(p_text);
    pItem->setTextAlignment(p_textAlignment);
    return pItem;
}
