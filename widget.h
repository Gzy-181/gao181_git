#ifndef WIDGET_H
#define WIDGET_H

#include <QSerialPort>
#include <QWidget>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_serial_open_button_clicked(bool checked);

    void on_serial_close_button_clicked(bool checked);

    void refreshSerialPorts();

    void readSerialPortData(); // 串口数据接收槽函数

    void updateSystemDateTime();  // 新增：更新系统日期时间
    void processBinaryFrame();
    float bytesToFloat(const QByteArray &bytes);
    QString getRainStateString(quint8 rainCode);
    QString getDataTypeString(quint8 dataType);

private:
    Ui::Widget *ui;
    QSerialPort *serialport;
    QTimer *serialRefreshTimer;  // 串口刷新定时器
    void initSerialRefreshTimer();  // 初始化串口刷新定时器
    void stopSerialRefreshTimer();   // 停止串口刷新定时器
    void updateSerialPortComboBox();// 更新串口下拉框
    QByteArray receiveBuffer;  // 接收缓冲区
    QTimer *dateTimeTimer;       // 新增：日期时间更新定时器
    void initDateTimeTimer();    // 新增：初始化日期时间定时器
};
#endif // WIDGET_H
