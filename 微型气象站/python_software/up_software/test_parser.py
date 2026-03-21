import re

# 模拟串口数据
test_data = [
    "BH1750 read error: 1",
    "Acc: X:-2866 Y:251 Z:-2891",
    "Acc Change: 58",
    "Wind: Low",
    "Rain level: No rain",
    "DHT22 OK: T:27.5C H:74.0%",
    "BH1750 read error: 1",
    "Acc: X:-2859 Y:270 Z:-2897",
    "Acc Change: 32",
    "Wind: Low",
    "Rain level: No rain",
    "BH1750 read error: 1",
    "Acc: X:-2863 Y:273 Z:-2898",
    "Acc Change: 8",
    "Wind: Low",
    "DHT22 OK: T:27.5C H:74.0%"
]

# 数据存储
temperature = "--"
humidity = "--"
rain_level = "No rain"
wind_level = "Low"

def parse_data(line):
    global temperature, humidity, rain_level, wind_level
    
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

# 测试解析功能
print("测试数据解析功能...")
for line in test_data:
    print(f"处理数据: {line}")
    parse_data(line)
    print(f"当前数据: 温度={temperature}℃, 湿度={humidity}%, 降水={rain_level}, 风力={wind_level}")
    print()

print("测试完成！")