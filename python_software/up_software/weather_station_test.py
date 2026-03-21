import serial
import tkinter as tk
from tkinter import ttk
import re
import time
from PIL import Image, ImageTk
import os
import random

class WeatherStationApp:
    def __init__(self, root):
        self.root = root
        self.root.title("微型气象站")
        self.root.geometry("400x350")
        self.root.resizable(False, False)
        
        # 设置背景颜色
        self.root.configure(bg="#f0f8ff")
        
        # 串口设置
        self.serial_port = None
        self.baud_rate = 9600
        self.port_var = tk.StringVar()
        
        # 数据存储
        self.temperature = "--"
        self.humidity = "--"
        self.rain_level = "No rain"
        self.wind_level = "Low"
        
        # 图标路径
        self.icons_dir = "icons"
        self.icon_images = {}
        self.load_icons()
        
        # 创建UI
        self.create_widgets()
        
        # 启动数据读取线程
        self.running = False
        self.update_thread = None
    
    def load_icons(self):
        # 创建图标目录
        if not os.path.exists(self.icons_dir):
            os.makedirs(self.icons_dir)
        
        # 这里可以添加图标加载逻辑
        # 暂时使用文字替代图标
    
    def create_widgets(self):
        # 标题
        title_frame = tk.Frame(self.root, bg="#f0f8ff")
        title_frame.pack(pady=10)
        title_label = tk.Label(title_frame, text="微型气象站", font=("Arial", 18, "bold"), bg="#f0f8ff")
        title_label.pack()
        
        # 串口选择
        serial_frame = tk.Frame(self.root, bg="#f0f8ff")
        serial_frame.pack(pady=10)
        
        port_label = tk.Label(serial_frame, text="串口:", bg="#f0f8ff")
        port_label.pack(side=tk.LEFT, padx=5)
        
        port_entry = ttk.Combobox(serial_frame, textvariable=self.port_var, width=10)
        port_entry['values'] = self.get_available_ports()
        port_entry.pack(side=tk.LEFT, padx=5)
        
        connect_btn = tk.Button(serial_frame, text="连接", command=self.connect_serial, bg="#4CAF50", fg="white")
        connect_btn.pack(side=tk.LEFT, padx=5)
        
        disconnect_btn = tk.Button(serial_frame, text="断开", command=self.disconnect_serial, bg="#f44336", fg="white")
        disconnect_btn.pack(side=tk.LEFT, padx=5)
        
        # 测试按钮
        test_btn = tk.Button(serial_frame, text="测试数据", command=self.test_data, bg="#2196F3", fg="white")
        test_btn.pack(side=tk.LEFT, padx=5)
        
        # 数据显示区域
        data_frame = tk.Frame(self.root, bg="#f0f8ff")
        data_frame.pack(pady=20)
        
        # 温度和湿度
        temp_hum_frame = tk.Frame(data_frame, bg="#f0f8ff")
        temp_hum_frame.pack(pady=10)
        
        temp_label = tk.Label(temp_hum_frame, text="温度:", font=("Arial", 12), bg="#f0f8ff")
        temp_label.grid(row=0, column=0, padx=10, pady=5, sticky=tk.W)
        self.temp_value = tk.Label(temp_hum_frame, text=f"{self.temperature}℃", font=("Arial", 12, "bold"), bg="#f0f8ff")
        self.temp_value.grid(row=0, column=1, padx=10, pady=5, sticky=tk.W)
        
        hum_label = tk.Label(temp_hum_frame, text="湿度:", font=("Arial", 12), bg="#f0f8ff")
        hum_label.grid(row=1, column=0, padx=10, pady=5, sticky=tk.W)
        self.hum_value = tk.Label(temp_hum_frame, text=f"{self.humidity}%", font=("Arial", 12, "bold"), bg="#f0f8ff")
        self.hum_value.grid(row=1, column=1, padx=10, pady=5, sticky=tk.W)
        
        # 降水和风力
        weather_frame = tk.Frame(data_frame, bg="#f0f8ff")
        weather_frame.pack(pady=10)
        
        rain_label = tk.Label(weather_frame, text="降水:", font=("Arial", 12), bg="#f0f8ff")
        rain_label.grid(row=0, column=0, padx=10, pady=5, sticky=tk.W)
        self.rain_value = tk.Label(weather_frame, text=self.rain_level, font=("Arial", 12, "bold"), bg="#f0f8ff")
        self.rain_value.grid(row=0, column=1, padx=10, pady=5, sticky=tk.W)
        
        wind_label = tk.Label(weather_frame, text="风力:", font=("Arial", 12), bg="#f0f8ff")
        wind_label.grid(row=1, column=0, padx=10, pady=5, sticky=tk.W)
        self.wind_value = tk.Label(weather_frame, text=self.wind_level, font=("Arial", 12, "bold"), bg="#f0f8ff")
        self.wind_value.grid(row=1, column=1, padx=10, pady=5, sticky=tk.W)
        
        # 状态信息
        self.status_var = tk.StringVar(value="未连接")
        status_label = tk.Label(self.root, textvariable=self.status_var, bg="#f0f8ff", fg="#666")
        status_label.pack(pady=10)
    
    def get_available_ports(self):
        # 简单实现，返回常见串口
        return ["COM1", "COM2", "COM3", "COM4", "COM5"]
    
    def connect_serial(self):
        port = self.port_var.get()
        if not port:
            self.status_var.set("请选择串口")
            print("请选择串口")  # 调试信息
            return
        
        try:
            print(f"尝试连接到串口 {port}，波特率 {self.baud_rate}")  # 调试信息
            self.serial_port = serial.Serial(port, self.baud_rate, timeout=1)
            print(f"成功连接到串口 {port}")  # 调试信息
            self.status_var.set(f"已连接到 {port}")
            self.running = True
            print("启动数据读取线程")  # 调试信息
            self.root.after(100, self.read_serial_data)
        except Exception as e:
            self.status_var.set(f"连接失败: {str(e)}")
            print(f"连接失败: {str(e)}")  # 调试信息
    
    def disconnect_serial(self):
        if self.serial_port and self.serial_port.is_open:
            self.serial_port.close()
            self.status_var.set("已断开连接")
        self.running = False
    
    def read_serial_data(self):
        if not self.running or not self.serial_port or not self.serial_port.is_open:
            print(f"退出读取: running={self.running}, serial_port={self.serial_port}, is_open={self.serial_port.is_open if self.serial_port else False}")
            return
        
        try:
            print(f"检查串口数据: in_waiting={self.serial_port.in_waiting}")  # 调试信息
            if self.serial_port.in_waiting > 0:
                line = self.serial_port.readline().decode('utf-8', errors='ignore').strip()
                print(f"接收到数据: {line}")  # 调试信息
                self.parse_data(line)
            else:
                print("没有数据可读")  # 调试信息
        except Exception as e:
            self.status_var.set(f"读取错误: {str(e)}")
            print(f"读取错误: {str(e)}")  # 调试信息
        
        print("继续读取数据")  # 调试信息
        self.root.after(100, self.read_serial_data)
    
    def parse_data(self, line):
        # 解析温度和湿度
        dht_match = re.search(r'DHT22 OK: T:([\d.]+)C H:([\d.]+)%', line)
        if dht_match:
            self.temperature = dht_match.group(1)
            self.humidity = dht_match.group(2)
            print(f"解析到温度: {self.temperature}℃, 湿度: {self.humidity}%")  # 调试信息
            self.update_display()
        
        # 解析降水
        rain_match = re.search(r'Rain level: (.*)', line)
        if rain_match:
            self.rain_level = rain_match.group(1)
            print(f"解析到降水: {self.rain_level}")  # 调试信息
            self.update_display()
        
        # 解析风力
        wind_match = re.search(r'Wind: (.*)', line)
        if wind_match:
            self.wind_level = wind_match.group(1)
            print(f"解析到风力: {self.wind_level}")  # 调试信息
            self.update_display()
    
    def update_display(self):
        # 确保在主线程中更新界面
        def update():
            self.temp_value.config(text=f"{self.temperature}℃")
            self.hum_value.config(text=f"{self.humidity}%")
            self.rain_value.config(text=self.rain_level)
            self.wind_value.config(text=self.wind_level)
            print(f"界面已更新: 温度={self.temperature}℃, 湿度={self.humidity}%, 降水={self.rain_level}, 风力={self.wind_level}")  # 调试信息
        
        self.root.after(0, update)
    
    def test_data(self):
        # 生成随机测试数据
        self.temperature = str(round(random.uniform(20, 30), 1))
        self.humidity = str(round(random.uniform(60, 80), 1))
        self.rain_level = random.choice(["No rain", "Light rain", "Moderate rain", "Heavy rain"])
        self.wind_level = random.choice(["Low", "Medium", "High"])
        
        # 更新界面
        self.update_display()
        
        # 打印调试信息
        print(f"测试数据: 温度={self.temperature}℃, 湿度={self.humidity}%, 降水={self.rain_level}, 风力={self.wind_level}")
    
    def on_closing(self):
        self.disconnect_serial()
        self.root.destroy()

if __name__ == "__main__":
    root = tk.Tk()
    app = WeatherStationApp(root)
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    root.mainloop()