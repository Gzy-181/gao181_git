import serial
import time

# 测试串口连接和数据接收
def test_serial_connection(port, baud_rate):
    try:
        # 明确设置串口参数
        ser = serial.Serial(
            port=port,
            baudrate=baud_rate,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=1
        )
        
        print(f"成功连接到串口 {port}，波特率 {baud_rate}")
        print("开始接收数据...")
        print("按 Ctrl+C 停止")
        
        while True:
            if ser.in_waiting > 0:
                # 读取数据
                data = ser.read(ser.in_waiting)
                print(f"接收到原始数据: {data}")
                print(f"接收到解码数据: {data.decode('utf-8', errors='ignore')}")
            time.sleep(0.1)
            
    except Exception as e:
        print(f"错误: {str(e)}")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("串口已关闭")

import serial.tools.list_ports

def get_available_ports():
    ports = serial.tools.list_ports.comports()
    return [port.device for port in ports]

if __name__ == "__main__":
    # 获取可用的串口端口
    available_ports = get_available_ports()
    print(f"可用的串口端口: {available_ports}")
    
    if not available_ports:
        print("没有找到可用的串口端口")
        exit()
    
    # 测试不同的波特率
    test_port = available_ports[0]  # 使用第一个可用的串口
    test_baud_rates = [9600, 115200, 38400, 57600]
    
    for baud_rate in test_baud_rates:
        print(f"\n测试波特率: {baud_rate}")
        test_serial_connection(test_port, baud_rate)
        time.sleep(2)