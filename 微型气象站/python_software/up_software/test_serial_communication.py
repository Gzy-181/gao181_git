import serial
import time
import threading

# 模拟串口数据
test_data = [
    "BH1750 read error: 1\n",
    "Acc: X:-2866 Y:251 Z:-2891\n",
    "Acc Change: 58\n",
    "Wind: Low\n",
    "Rain level: No rain\n",
    "DHT22 OK: T:27.5C H:74.0%\n",
    "BH1750 read error: 1\n",
    "Acc: X:-2859 Y:270 Z:-2897\n",
    "Acc Change: 32\n",
    "Wind: Low\n",
    "Rain level: No rain\n",
    "BH1750 read error: 1\n",
    "Acc: X:-2863 Y:273 Z:-2898\n",
    "Acc Change: 8\n",
    "Wind: Low\n",
    "DHT22 OK: T:27.5C H:74.0%\n"
]

# 模拟串口类
class MockSerial:
    def __init__(self):
        self.data = test_data.copy()
        self.is_open = True
        self.in_waiting = 0
        self.index = 0
    
    def readline(self):
        if self.index < len(self.data):
            line = self.data[self.index].encode('utf-8')
            self.index += 1
            return line
        return b''
    
    def close(self):
        self.is_open = False

# 测试数据读取和解析
def test_serial_reading():
    import re
    
    # 数据存储
    temperature = "--"
    humidity = "--"
    rain_level = "No rain"
    wind_level = "Low"
    
    def parse_data(line):
        nonlocal temperature, humidity, rain_level, wind_level
        
        # 解析温度和湿度
        dht_match = re.search(r'DHT22 OK: T:([\d.]+)C H:([\d.]+)%', line)
        if dht_match:
            temperature = dht_match.group(1)
            humidity = dht_match.group(2)
            print(f"解析到温度: {temperature}℃, 湿度: {humidity}%")
        
        # 解析降水
        rain_match = re.search(r'Rain level: (.*)', line)
        if rain_match:
            rain_level = rain_match.group(1)
            print(f"解析到降水: {rain_level}")
        
        # 解析风力
        wind_match = re.search(r'Wind: (.*)', line)
        if wind_match:
            wind_level = wind_match.group(1)
            print(f"解析到风力: {wind_level}")
    
    # 创建模拟串口
    mock_serial = MockSerial()
    
    # 模拟读取数据
    print("测试串口数据读取...")
    for i in range(len(test_data)):
        line = mock_serial.readline().decode('utf-8', errors='ignore').strip()
        print(f"接收到数据: {line}")
        parse_data(line)
        print(f"当前数据: 温度={temperature}℃, 湿度={humidity}%, 降水={rain_level}, 风力={wind_level}")
        print()
        time.sleep(0.1)
    
    mock_serial.close()
    print("测试完成！")

if __name__ == "__main__":
    test_serial_reading()