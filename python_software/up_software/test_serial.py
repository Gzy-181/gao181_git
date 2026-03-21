import serial
import time
import random

# 模拟串口数据发送
def simulate_serial_data():
    # 模拟数据格式
    data_patterns = [
        "BH1750 read error: 1",
        "Acc: X:-2866 Y:251 Z:-2891",
        "Acc Change: 58",
        "Wind: Low",
        "Rain level: No rain",
        "DHT22 OK: T:27.5C H:74.0%"
    ]
    
    while True:
        # 随机选择数据发送
        for pattern in data_patterns:
            if "DHT22 OK" in pattern:
                # 生成随机温度和湿度
                temp = round(random.uniform(20, 30), 1)
                hum = round(random.uniform(60, 80), 1)
                print(f"DHT22 OK: T:{temp}C H:{hum}%")
            else:
                print(pattern)
            time.sleep(0.5)

if __name__ == "__main__":
    print("模拟串口数据发送...")
    print("按 Ctrl+C 停止")
    try:
        simulate_serial_data()
    except KeyboardInterrupt:
        print("\n模拟结束")