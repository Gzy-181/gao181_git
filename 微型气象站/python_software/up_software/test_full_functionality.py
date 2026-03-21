import serial
import time
import random

# 模拟串口数据发送
def simulate_serial_data():
    # 模拟数据格式
    data_patterns = [
        "DHT22 OK: T:24.8C H:51.8%",
        "Light: 240.0 lx",
        "Press: 1008.15 hPa",
        "Rain level: No rain",
        "DHT22 OK: T:25.2C H:53.5%",
        "Light: 320.5 lx",
        "Press: 1009.20 hPa",
        "Rain level: Light rain"
    ]
    
    while True:
        # 随机选择数据发送
        for pattern in data_patterns:
            if "DHT22 OK" in pattern:
                # 生成随机温度和湿度
                temp = round(random.uniform(20, 30), 1)
                hum = round(random.uniform(50, 80), 1)
                print(f"DHT22 OK: T:{temp}C H:{hum}%")
            elif "Light" in pattern:
                # 生成随机光照强度
                light = round(random.uniform(100, 500), 1)
                print(f"Light: {light} lx")
            elif "Press" in pattern:
                # 生成随机气压
                press = round(random.uniform(1000, 1020), 2)
                print(f"Press: {press} hPa")
            elif "Rain level" in pattern:
                # 随机选择降水等级
                rain_levels = ["No rain", "Light rain", "Moderate rain", "Heavy rain"]
                rain = random.choice(rain_levels)
                print(f"Rain level: {rain}")
            else:
                print(pattern)
            time.sleep(1)

if __name__ == "__main__":
    print("模拟串口数据发送...")
    print("按 Ctrl+C 停止")
    try:
        simulate_serial_data()
    except KeyboardInterrupt:
        print("\n模拟结束")