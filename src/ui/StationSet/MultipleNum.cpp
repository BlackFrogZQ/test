#include "MultipleNum.h"

MultipleNum::MultipleNum(QWidget* parent)
    : QWidget(parent), 
    ui(new Ui::MultipleNum)
{
    ui->setupUi(this);

    connect(ui->btn_NumON, &QPushButton::clicked, this, [=](){Obtain_Num();});
    connect(ui->btn_NumOFF, &QPushButton::clicked, this, [=](){close();});
}

MultipleNum::~MultipleNum()
{
    delete ui; 
}

void MultipleNum::Obtain_Num()
{
    NUM_tem = ui->spinBox_Num->value();
    emit Send_Num(NUM_tem);
    close();
}
