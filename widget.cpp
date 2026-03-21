#include "widget.h"
#include "./ui_widget.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->webcon_tytle->setStyleSheet("color:rgb(0,170,128)");//设置字体颜色
    setWindowTitle("微型气象站");
    setWindowIcon(QIcon(":/icon.png"));
    setLayout(ui->gridLayout);
    serialport=new QSerialPort(this);
    ui->baundrate_comboBox->setCurrentIndex(2);//初始默认115200

    // 连接数据接收信号到槽函数
    connect(serialport, &QSerialPort::readyRead, this, &Widget::readSerialPortData);

    // 初始化串口刷新定时器
    initSerialRefreshTimer();

    // 初始刷新一次串口列表
    updateSerialPortComboBox();

    // 初始化日期时间定时器
    initDateTimeTimer();

    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for(auto seria:serialPortInfos)
    {
        qDebug()<<"portname:"<<seria.portName();
        ui->serial_comboBox->addItem(seria.portName());
    }
}

Widget::~Widget()
{
    stopSerialRefreshTimer();

    // 停止日期时间定时器
    if (dateTimeTimer) {
        dateTimeTimer->stop();
        delete dateTimeTimer;
    }
    delete ui;
}

void Widget::on_serial_open_button_clicked(bool checked)
{
    // 设置波特率
    bool ok;
    int baudRate = ui->baundrate_comboBox->currentText().toInt();
    serialport->setBaudRate(baudRate);
    QString portName = ui->serial_comboBox->currentData().toString();
    if (portName.isEmpty()) {
        portName = ui->serial_comboBox->currentText();
    }

    serialport->setPortName(portName);

    // 设置数据位
    serialport->setDataBits(QSerialPort::Data8);

    // 设置停止位
    serialport->setStopBits(QSerialPort::OneStop);

    // 设置校验位
    serialport->setParity(QSerialPort::NoParity);

    // 设置流控制
    serialport->setFlowControl(QSerialPort::NoFlowControl);

    // 打开串口
    if (serialport->open(QIODevice::ReadWrite)) {
        //ui->pushButton->setText("关闭串口");
        ui->serial_open_button->setDisabled(true);
        ui->serial_open_button->setText("已连接");
        ui->serial_open_button->setStyleSheet("color:rgb(85, 170, 127)");
        qDebug()<<"已连接";
    } else {
        QMessageBox::warning(this, "错误", "无法打开串口: " + serialport->errorString());
    }
}



void Widget::on_serial_close_button_clicked(bool checked)
{
    if (serialport && serialport->isOpen()) {
        // 关闭串口
        serialport->close();

        // 更新界面状态
        ui->serial_open_button->setEnabled(true);
        ui->serial_open_button->setText("连接");
        ui->serial_open_button->setStyleSheet("color:rgb(0, 0, 0)");
    }
}


// 初始化串口刷新定时器
void Widget::initSerialRefreshTimer()
{
    serialRefreshTimer = new QTimer(this);
    serialRefreshTimer->setInterval(1000);  // 1秒间隔
    connect(serialRefreshTimer, &QTimer::timeout, this, &Widget::refreshSerialPorts);

    // 启动定时器
    serialRefreshTimer->start();

}

// 停止串口刷新定时器
void Widget::stopSerialRefreshTimer()
{
    if (serialRefreshTimer) {
        serialRefreshTimer->stop();
        disconnect(serialRefreshTimer, &QTimer::timeout, this, &Widget::refreshSerialPorts);
        delete serialRefreshTimer;
        serialRefreshTimer = nullptr;
    }
}

// 更新串口下拉框
void Widget::updateSerialPortComboBox()
{
    // 保存当前选中的串口
    QString currentPort = ui->serial_comboBox->currentData().toString();

    // 清空下拉框
    ui->serial_comboBox->clear();

    // 获取所有可用串口
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    if (serialPortInfos.isEmpty()) {
        ui->serial_comboBox->addItem("无可用串口", "");
        ui->serial_comboBox->setEnabled(false);
        ui->serial_open_button->setEnabled(false);
        return;
    }

    // 填充串口列表，只显示端口名
    for (const auto &serialPortInfo : serialPortInfos) {
        QString portName = serialPortInfo.portName();

        // 只添加端口名，数据也是端口名
        ui->serial_comboBox->addItem(portName, portName);
    }

    // 恢复之前选中的串口（如果还存在）
    if (!currentPort.isEmpty()) {
        int index = ui->serial_comboBox->findData(currentPort);
        if (index >= 0) {
            ui->serial_comboBox->setCurrentIndex(index);
        }
    }

    // 更新按钮状态
    bool hasPorts = serialPortInfos.size() > 0;
    ui->serial_comboBox->setEnabled(hasPorts);
    ui->serial_open_button->setEnabled(hasPorts);

    // 如果当前没有选中串口，选中第一个
    if (hasPorts && ui->serial_comboBox->currentIndex() < 0) {
        ui->serial_comboBox->setCurrentIndex(0);
    }
}

// 定时器触发的刷新函数
void Widget::refreshSerialPorts()
{
    // 如果串口已打开，不更新下拉框，只记录日志
    if (serialport && serialport->isOpen()) {
        return;
    }

    // 更新串口下拉框
    updateSerialPortComboBox();
}


// 修改 readSerialPortData 函数
void Widget::readSerialPortData()
{
    if (!serialport || !serialport->isOpen()) {
        return;
    }

    QByteArray data = serialport->readAll();

    if (data.isEmpty()) {
        return;
    }

    // 追加到缓冲区
    receiveBuffer.append(data);

    // 二进制数据帧解析（不再使用字符串解析）
    processBinaryFrame();
}

/**
 * 处理二进制数据帧
 * 帧格式：帧头(0xAA) + 数据类型(1字节) + 数据长度(1字节) + 数据内容(N字节) + 校验和(1字节) + 帧尾(0x55)
 * 数据类型：0x01=温度, 0x02=湿度, 0x03=气压, 0x04=光照, 0x05=降雨状态
 */
/**
 * 处理二进制数据帧（带详细日志输出）
 * 帧格式：帧头(0xAA) + 数据类型(1字节) + 数据长度(1字节) + 数据内容(N字节) + 校验和(1字节) + 帧尾(0x55)
 * 数据类型：0x01=温度, 0x02=湿度, 0x03=气压, 0x04=光照, 0x05=降雨状态
 */
void Widget::processBinaryFrame()
{
    static int frameCount = 0;      // 帧计数器
    static int errorCount = 0;      // 错误计数器
    static int successCount = 0;    // 成功计数器

    // 持续处理直到缓冲区中找不到完整的帧
    while (receiveBuffer.size() >= 9) {  // 最小帧长度：帧头(1)+类型(1)+长度(1)+数据(4)+校验和(1)+帧尾(1)=9
        frameCount++;
        // 查找帧头0xAA
        int frameStart = -1;
        for (int i = 0; i < receiveBuffer.size(); i++) {
            if (static_cast<quint8>(receiveBuffer.at(i)) == 0xAA) {
                frameStart = i;
                break;
            }
        }

        // 没有找到帧头，清空缓冲区
        if (frameStart == -1) {
            errorCount++;
            receiveBuffer.clear();
            return;
        }

        // 移除帧头之前的无效数据
        if (frameStart > 0) {
            receiveBuffer.remove(0, frameStart);
        }

        // 检查是否有足够数据构成最小帧
        if (receiveBuffer.size() < 9) {
            return;
        }

        // 解析帧结构
        quint8 frameHeader = static_cast<quint8>(receiveBuffer.at(0));    // 应该是0xAA
        quint8 dataType = static_cast<quint8>(receiveBuffer.at(1));       // 数据类型
        quint8 dataLength = static_cast<quint8>(receiveBuffer.at(2));     // 数据长度
        // 验证帧头
        if (frameHeader != 0xAA) {
            errorCount++;
            receiveBuffer.remove(0, 1);
            continue;
        }

        // 计算总帧长度
        int totalFrameLength = 5 + dataLength;  // 固定5字节+数据长度

        // 检查是否有完整帧
        if (receiveBuffer.size() < totalFrameLength) {
            return;  // 等待更多数据
        }

        // 检查帧尾
        quint8 frameTail = static_cast<quint8>(receiveBuffer.at(totalFrameLength - 1));
        if (frameTail != 0x55) {
            errorCount++;
            receiveBuffer.remove(0, 1);
            continue;
        }

        // 验证校验和
        quint8 checksum = 0;
        for (int i = 0; i < totalFrameLength - 2; i++) {
            checksum ^= static_cast<quint8>(receiveBuffer.at(i));
        }

        quint8 frameChecksum = static_cast<quint8>(receiveBuffer.at(totalFrameLength - 2));
        if (checksum != frameChecksum) {
            errorCount++;
            receiveBuffer.remove(0, totalFrameLength);
            continue;
        }

        // 提取数据内容
        QByteArray dataContent = receiveBuffer.mid(3, dataLength);
        // 根据数据类型解析并更新UI
        switch (dataType) {
        case 0x01: { // 温度
            if (dataLength == 4) {
                float temperature = bytesToFloat(dataContent);
                qDebug() << "[温度数据] 解析值:" << temperature << "℃";
                ui->lineEdit_T->setText(QString::number(temperature, 'f', 1) + "℃");
                ui->lineEdit_T->setStyleSheet("background-color: #e6f7ff; font-weight: bold;");
            }
            break;
        }
        case 0x02: { // 湿度
            if (dataLength == 4) {
                float humidity = bytesToFloat(dataContent);
                qDebug() << "[湿度数据] 解析值:" << humidity << "%";
                ui->lineEdit_H->setText(QString::number(humidity, 'f', 1) + "%");
                ui->lineEdit_H->setStyleSheet("background-color: #e6ffe6; font-weight: bold;");
            }
            break;
        }
        case 0x03: { // 气压
            if (dataLength == 4) {
                float pressure = bytesToFloat(dataContent);
                ui->lineEdit_hpa->setText(QString::number(pressure, 'f', 1) + "hpa");
                ui->lineEdit_hpa->setStyleSheet("background-color: #fff0e6; font-weight: bold;");
                qDebug() << "[气压数据] 解析值:" << pressure << "hpa";
            }
            break;
        }
        case 0x04: { // 光照
            if (dataLength == 4) {
                float light = bytesToFloat(dataContent);
                ui->lineEdit_light->setText(QString::number(light, 'f', 1) + "lx");
                ui->lineEdit_light->setStyleSheet("background-color: #f0f0ff; font-weight: bold;");
                qDebug() << "[光照数据] 解析值:" << light << "%";
            }
            break;
        }
        case 0x05: { // 降雨状态
            if (dataLength == 4) {
                quint8 rainCode = static_cast<quint8>(dataContent.at(0));
                QString rainStr = getRainStateString(rainCode);
                qDebug() << "[光照数据] 解析值:" << rainCode;
                ui->lineEdit_rainlevel->setText(rainStr);
                // 根据降雨状态设置不同背景色
                QString bgColor = (rainCode == 0) ? "#e6ffe6" :  // 无雨-绿色
                                      (rainCode == 1) ? "#ffffe6" :  // 小雨-黄色
                                      (rainCode == 2) ? "#ffe6cc" :  // 中雨-橙色
                                      "#ffcccc";                     // 大雨-红色
                ui->lineEdit_rainlevel->setStyleSheet(QString("background-color: %1; font-weight: bold;").arg(bgColor));
            }
            break;
        }
        default: {
            errorCount++;
            qWarning() << "[错误] 未知数据类型:"
                       << QString("0x%1").arg(dataType, 2, 16, QChar('0')).toUpper();
            break;
        }
        }

        // 成功处理一帧
        successCount++;
        // 移除已处理的数据帧
        receiveBuffer.remove(0, totalFrameLength);
    }
}

/**
 * 将字节数组转换为浮点数（小端格式）
 */
float Widget::bytesToFloat(const QByteArray &bytes)
{
    if (bytes.size() != 4) {
        qWarning() << "[转换错误] bytesToFloat期望4字节输入，实际" << bytes.size() << "字节";
        return 0.0f;
    }

    float value;
    memcpy(&value, bytes.constData(), 4);

    // 打印转换结果
    QByteArray bytesReverse = bytes;
    std::reverse(bytesReverse.begin(), bytesReverse.end());
    float valueBigEndian;
    memcpy(&valueBigEndian, bytesReverse.constData(), 4);
    return value;
}

/**
 * 将降雨状态编码转换为字符串
 */
QString Widget::getRainStateString(quint8 rainCode)
{
    QString result;
    switch (rainCode) {
    case 0: result = "No rain"; break;
    case 1: result = "Light rain"; break;
    case 2: result = "Moderate rain"; break;
    case 3: result = "Heavy rain"; break;
    default: result = "Unknown"; break;
    }
    return result;
}

/**
 * 获取数据类型的描述字符串
 */
QString Widget::getDataTypeString(quint8 dataType)
{
    switch (dataType) {
    case 0x01: return "温度";
    case 0x02: return "湿度";
    case 0x03: return "气压";
    case 0x04: return "光照";
    case 0x05: return "降雨状态";
    default: return "未知类型";
    }
}

// 初始化日期时间定时器
void Widget::initDateTimeTimer()
{
    dateTimeTimer = new QTimer(this);
    dateTimeTimer->setInterval(1000);  // 1秒更新一次
    connect(dateTimeTimer, &QTimer::timeout, this, &Widget::updateSystemDateTime);

    // 立即执行一次
    updateSystemDateTime();

    // 启动定时器
    dateTimeTimer->start();

}

// 获取并更新系统日期时间
void Widget::updateSystemDateTime()
{
    // 获取当前日期时间
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // 格式化日期
    QString dateStr = currentDateTime.toString("yyyy-MM-dd");

    // 格式化时间
    QString timeStr = currentDateTime.toString("hh:mm:ss");

    // 更新UI显示（假设你的UI中有这些控件）
    ui->label_date->setText(dateStr);
    ui->label_time->setText(timeStr);

}

