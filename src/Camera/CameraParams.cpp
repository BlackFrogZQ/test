#include "CameraParams.h"

CameraParams::CameraParams(QWidget* parent)
    : QWidget(parent), 
    ui(new Ui::CameraParams)
{
    ui->setupUi(this);

    //增益
    void (QSpinBox::*spSignal_GainRaw)(int) = &QSpinBox::valueChanged;
    connect(ui->SpinBox_GainRaw, spSignal_GainRaw, [=](){
            GainRaw = ui->SpinBox_GainRaw->value();
            ui->Slider_GainRaw->setValue(GainRaw);
        });
    void (QSlider::*slSignal_GainRaw)(int) = &QSlider::valueChanged;
    connect(ui->Slider_GainRaw, slSignal_GainRaw, [=](){
            GainRaw = ui->Slider_GainRaw->value();
            ui->SpinBox_GainRaw->setValue(GainRaw); 
            emit Send_CameraParams(GainRaw, ExposureTime);
        });
    //曝光
    void (QSpinBox::*spSignal_ExposureTime)(int) = &QSpinBox::valueChanged;
    connect(ui->SpinBox_ExposureTime, spSignal_ExposureTime, [=](){
            ExposureTime = ui->SpinBox_ExposureTime->value();
            ui->Slider_ExposureTime->setValue(ExposureTime);
            emit Send_CameraParams(GainRaw, ExposureTime);
        });
    void (QSlider::*slSignal_ExposureTime)(int) = &QSlider::valueChanged;
    connect(ui->Slider_ExposureTime, slSignal_ExposureTime, [=](){
            ExposureTime = ui->Slider_ExposureTime->value();
            ui->SpinBox_ExposureTime->setValue(ExposureTime);
            emit Send_CameraParams(GainRaw, ExposureTime);
        });
}

CameraParams::~CameraParams()
{
    delete ui; 
}