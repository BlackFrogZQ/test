#include "LightSet.h"

LightSet::LightSet(QWidget* parent)
    : QWidget(parent), 
    ui(new Ui::LightSet)
{
    ui->setupUi(this);

    //设置状态栏
    QStatusBar *stBar = new QStatusBar(this);;
    layout()->addWidget(stBar);

    stBar->setSizeGripEnabled(false);
    stBar->setStyleSheet(QString("QStatusBar::item{border: 0px}"));

    QLabel * label_statubar = new QLabel(QString::fromLocal8Bit("提示信息："),this);
    QLabel * label_statubar_light = new QLabel(this);
    label_statubar_light->setText(QStringLiteral("请打开光源串口!"));
    stBar->addWidget(label_statubar);
    stBar->addWidget(label_statubar_light);

    QPalette red;
    red.setColor(QPalette::WindowText,Qt::red);
    label_statubar_light->setPalette(red);

    //设置各种控件
    QStringList PortNameList;
    PortNameList = getPortNameList();
    ui->ComboBox_PortName->addItems(PortNameList);
    ui->ComboBox_PortName->setCurrentIndex(0);

    QStringList baudList;
    baudList << "1200"<< "2400"<< "4800"<< "9600"<< "19200"<< "38400"<< "115200";
    ui->ComboBox_Bound->addItems(baudList);
    ui->ComboBox_Bound->setCurrentIndex(4);

    //光源参数部分
    connect(ui->Button_OpenPort, &QPushButton::clicked, this, [=](){openPort(label_statubar_light);});
    //通道1
    void (QSpinBox::*spSignal_sp_A)(int) = &QSpinBox::valueChanged;
    connect(ui->SpinBox_Light_A, spSignal_sp_A, [=](){
            int p = ui->SpinBox_Light_A->value();
            ui->Slider_Light_A->setValue(p);
            hv_Raw[0] = p;
            if (0 != (HTuple(hv_Raw>10).TupleAnd(hv_Raw<100))){
                hv_string_Raw = ("0"+hv_Raw)+"";
                hv_output = ("SA0"+hv_string_Raw)+"#";}
            else if (0 != (hv_Raw<10)){
                hv_string_Raw = ("00"+hv_Raw)+"";
                hv_output = ("SA0"+hv_string_Raw)+"#";}
            else{
                hv_string_Raw = hv_Raw+"";
                hv_output = ("SA0"+hv_string_Raw)+"#";}
            TupleOrds(hv_output, &hv_Ords);
            if(lock_SerialPort == true){
                WriteSerial(hv_SerialHandle, hv_Ords);
            } });
    void (QSlider::*slSignal_sl_A)(int) = &QSlider::valueChanged;
    connect(ui->Slider_Light_A, slSignal_sl_A, [=](){
            int l = ui->Slider_Light_A->value();
            ui->SpinBox_Light_A->setValue(l); });

    //通道2
    void (QSpinBox::*spSignal_sp_B)(int) = &QSpinBox::valueChanged;
    connect(ui->SpinBox_Light_B, spSignal_sp_B, [=](){
            int p = ui->SpinBox_Light_B->value();
            ui->Slider_Light_B->setValue(p);
            hv_Raw[0] = p;
            if (0 != (HTuple(hv_Raw>10).TupleAnd(hv_Raw<100))){
                hv_string_Raw = ("0"+hv_Raw)+"";
                hv_output = ("SB0"+hv_string_Raw)+"#";}
            else if (0 != (hv_Raw<10)){
                hv_string_Raw = ("00"+hv_Raw)+"";
                hv_output = ("SB0"+hv_string_Raw)+"#";}
            else{
                hv_string_Raw = hv_Raw+"";
                hv_output = ("SB0"+hv_string_Raw)+"#";}
            TupleOrds(hv_output, &hv_Ords);
            if(lock_SerialPort == true){
                WriteSerial(hv_SerialHandle, hv_Ords);
            } });
    void (QSlider::*slSignal_sl_B)(int) = &QSlider::valueChanged;
    connect(ui->Slider_Light_B, slSignal_sl_B, [=](){
            int l = ui->Slider_Light_B->value();
            ui->SpinBox_Light_B->setValue(l); });

    //通道3
    void (QSpinBox::*spSignal_sp_C)(int) = &QSpinBox::valueChanged;
    connect(ui->SpinBox_Light_C, spSignal_sp_C, [=](){
            int p = ui->SpinBox_Light_C->value();
            ui->Slider_Light_C->setValue(p);
            hv_Raw[0] = p;
            if (0 != (HTuple(hv_Raw>10).TupleAnd(hv_Raw<100))){
                hv_string_Raw = ("0"+hv_Raw)+"";
                hv_output = ("SC0"+hv_string_Raw)+"#";}
            else if (0 != (hv_Raw<10)){
                hv_string_Raw = ("00"+hv_Raw)+"";
                hv_output = ("SC0"+hv_string_Raw)+"#";}
            else{
                hv_string_Raw = hv_Raw+"";
                hv_output = ("SC0"+hv_string_Raw)+"#";}
            TupleOrds(hv_output, &hv_Ords);
            if(lock_SerialPort == true){
                WriteSerial(hv_SerialHandle, hv_Ords);
            } });
    void (QSlider::*slSignal_sl_C)(int) = &QSlider::valueChanged;
    connect(ui->Slider_Light_C, slSignal_sl_C, [=](){
            int l = ui->Slider_Light_C->value();
            ui->SpinBox_Light_C->setValue(l); });

    //通道4
    void (QSpinBox::*spSignal_sp_D)(int) = &QSpinBox::valueChanged;
    connect(ui->SpinBox_Light_D, spSignal_sp_D, [=](){
            int p = ui->SpinBox_Light_D->value();
            ui->Slider_Light_D->setValue(p);
            hv_Raw[0] = p;
            if (0 != (HTuple(hv_Raw>10).TupleAnd(hv_Raw<100))){
                hv_string_Raw = ("0"+hv_Raw)+"";
                hv_output = ("SD0"+hv_string_Raw)+"#";}
            else if (0 != (hv_Raw<10)){
                hv_string_Raw = ("00"+hv_Raw)+"";
                hv_output = ("SD0"+hv_string_Raw)+"#";}
            else{
                hv_string_Raw = hv_Raw+"";
                hv_output = ("SD0"+hv_string_Raw)+"#";}
            TupleOrds(hv_output, &hv_Ords);
            if(lock_SerialPort == true){
                WriteSerial(hv_SerialHandle, hv_Ords);
            } });
    void (QSlider::*slSignal_sl_D)(int) = &QSlider::valueChanged;
    connect(ui->Slider_Light_D, slSignal_sl_D, [=](){
            int l = ui->Slider_Light_D->value();
            ui->SpinBox_Light_D->setValue(l); });
}

LightSet::~LightSet()
{
    delete ui; 
}

//这个函数将返回QList<QSerialPortInfo>类型的内容，返回内容是可用的串口信息
QStringList LightSet::getPortNameList()
{
    QStringList m_serialPortName;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        m_serialPortName << info.portName();
        qDebug() << "serialPortName:" << info.portName();
    }
    return m_serialPortName;
}

//打开串口
void LightSet::openPort(QLabel * label_statubar_light)
{
    if (lock_SerialPort == true) //如果串口已经打开了 先给他关闭了
    {
        CloseSerial(hv_SerialHandle);

        ui->Button_OpenPort->setText(cnStr("打开串口"));

        QPalette black;
        black.setColor(QPalette::WindowText, Qt::black);
        label_statubar_light->setPalette(black);
        label_statubar_light->setText(cnStr("光源串口关闭成功!"));

        lock_SerialPort = false;
        return;
    }

    QString str_Serial;
    str_Serial = ui->ComboBox_PortName->currentText();
    hv_output_SerialPort[2] = str_Serial.toStdString().c_str();

    OpenSerial(hv_output_SerialPort[2], &hv_SerialHandle);
    SetSerialParam(hv_SerialHandle, ui->ComboBox_Bound->currentText().toInt(), 8, "none", "none", 1, 0, "unchanged");

    if (QString(str_Serial).isNull())
    {
        qDebug() << QString(cnStr("打开失败!"));
        return;
    }

    ui->Button_OpenPort->setText(cnStr("关闭串口"));

    QPalette black;
    black.setColor(QPalette::WindowText, Qt::black);
    label_statubar_light->setPalette(black);
    label_statubar_light->setText(cnStr("光源串口打开成功!"));

    lock_SerialPort = true;
}