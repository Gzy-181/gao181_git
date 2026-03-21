import re

# 模拟新的数据格式
test_data = [
    "DHT22 OK: T:24.8C H:51.8%",
    "Light: 240.0 lx",
    "Temperature: 24.71 C, Pressure: 1008.66 hPa",
    "DHT22 OK: T:25.2C H:53.5%",
    "Light: 320.5 lx",
    "Temperature: 25.15 C, Pressure: 1009.20 hPa"
]

# 数据存储
temperature = "--"
humidity = "--"
light = "--"
pressure = "--"

def parse_data(line):
    global temperature, humidity, light, pressure
    
    # 解析温度和湿度 (DHT22 OK: T:24.8C H:51.8%)
    dht_match = re.search(r'DHT22 OK: T:([\d.]+)C H:([\d.]+)%', line)
    if dht_match:
        temperature = dht_match.group(1)
        humidity = dht_match.group(2)
        print(f"解析到温度: {temperature}℃, 湿度: {humidity}%")
    
    # 解析光照强度 (Light: 240.0 lx)
    light_match = re.search(r'Light: ([\d.]+) lx', line)
    if light_match:
        light = light_match.group(1)
        print(f"解析到光照强度: {light} lx")
    
    # 解析温度和气压 (Temperature: 24.71 C, Pressure: 1008.66 hPa)
    temp_pressure_match = re.search(r'Temperature: ([\d.]+) C, Pressure: ([\d.]+) hPa', line)
    if temp_pressure_match:
        temperature = temp_pressure_match.group(1)
        pressure = temp_pressure_match.group(2)
        print(f"解析到温度: {temperature}℃, 气压: {pressure} hPa")

# 测试解析功能
print("测试新数据格式解析功能...")
for line in test_data:
    print(f"处理数据: {line}")
    parse_data(line)
    print(f"当前数据: 温度={temperature}℃, 湿度={humidity}%, 光照={light} lx, 气压={pressure} hPa")
    print()

print("测试完成！")