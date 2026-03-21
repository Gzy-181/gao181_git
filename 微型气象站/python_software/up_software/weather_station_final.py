import serial
import tkinter as tk
from tkinter import ttk
import re
import os
import serial.tools.list_ports

class WeatherStationApp:
    def __init__(self, root):
        self.root = root
        self.root.title("微型气象站")
        self.root.geometry("500x350")
        self.root.resizable(False, False)
        
        # 设置背景颜色
        self.root.configure(bg="#f0f8ff")
        
        # 串口设置
        self.serial_port = None
        self.baud_rate = 115200
        self.port_var = tk.StringVar()
        self.baud_var = tk.StringVar(value="115200")
        
        # 数据存储
        self.temperature = "--"
        self.humidity = "--"
        self.light = "--"
        self.pressure = "--"
        self.rain_level = "--"
        
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
        title_label = tk.Label(title_frame, text="微型气象站", font=("Arial", 20, "bold"), bg="#f0f8ff", fg="#3498db")
        title_label.pack()
        
        # 串口选择
        serial_frame = tk.Frame(self.root, bg="#f0f8ff")
        serial_frame.pack(pady=10)
        
        port_label = tk.Label(serial_frame, text="串口:", font=("Arial", 12), bg="#f0f8ff")
        port_label.pack(side=tk.LEFT, padx=5)
        
        port_entry = ttk.Combobox(serial_frame, textvariable=self.port_var, width=10)
        port_entry['values'] = self.get_available_ports()
        port_entry.pack(side=tk.LEFT, padx=5)
        
        baud_label = tk.Label(serial_frame, text="波特率:", font=("Arial", 12), bg="#f0f8ff")
        baud_label.pack(side=tk.LEFT, padx=5)
        
        baud_entry = ttk.Combobox(serial_frame, textvariable=self.baud_var, width=10)
        baud_entry['values'] = ["9600", "115200", "38400", "57600"]
        baud_entry.pack(side=tk.LEFT, padx=5)
        
        connect_btn = tk.Button(serial_frame, text="连接", command=self.connect_serial, bg="#4CAF50", fg="white", font=("Arial", 10, "bold"))
        connect_btn.pack(side=tk.LEFT, padx=5, pady=2)
        
        disconnect_btn = tk.Button(serial_frame, text="断开", command=self.disconnect_serial, bg="#f44336", fg="white", font=("Arial", 10, "bold"))
        disconnect_btn.pack(side=tk.LEFT, padx=5, pady=2)
        
        # 数据显示区域
        data_frame = tk.Frame(self.root, bg="#f0f8ff")
        data_frame.pack(pady=20)
        
        # 创建一个网格布局的框架
        grid_frame = tk.Frame(data_frame, bg="#f0f8ff")
        grid_frame.pack(pady=10)
        
        # 温度
        temp_label = tk.Label(grid_frame, text="温度:", font=("Arial", 12), bg="#f0f8ff")
        temp_label.grid(row=0, column=0, padx=20, pady=15, sticky=tk.W)
        self.temp_value = tk.Label(grid_frame, text=f"{self.temperature}℃", font=("Arial", 16, "bold"), bg="#f0f8ff", fg="#e74c3c")
        self.temp_value.grid(row=0, column=1, padx=20, pady=15, sticky=tk.W)
        
        # 湿度
        hum_label = tk.Label(grid_frame, text="湿度:", font=("Arial", 12), bg="#f0f8ff")
        hum_label.grid(row=1, column=0, padx=20, pady=15, sticky=tk.W)
        self.hum_value = tk.Label(grid_frame, text=f"{self.humidity}%", font=("Arial", 16, "bold"), bg="#f0f8ff", fg="#3498db")
        self.hum_value.grid(row=1, column=1, padx=20, pady=15, sticky=tk.W)
        
        # 光照强度
        light_label = tk.Label(grid_frame, text="光照强度:", font=("Arial", 12), bg="#f0f8ff")
        light_label.grid(row=0, column=2, padx=20, pady=15, sticky=tk.W)
        self.light_value = tk.Label(grid_frame, text=f"{self.light} lx", font=("Arial", 16, "bold"), bg="#f0f8ff", fg="#f39c12")
        self.light_value.grid(row=0, column=3, padx=20, pady=15, sticky=tk.W)
        
        # 气压
        pressure_label = tk.Label(grid_frame, text="气压:", font=("Arial", 12), bg="#f0f8ff")
        pressure_label.grid(row=1, column=2, padx=20, pady=15, sticky=tk.W)
        self.pressure_value = tk.Label(grid_frame, text=f"{self.pressure} hPa", font=("Arial", 16, "bold"), bg="#f0f8ff", fg="#27ae60")
        self.pressure_value.grid(row=1, column=3, padx=20, pady=15, sticky=tk.W)
        
        # 降水等级
        rain_label = tk.Label(grid_frame, text="降水等级:", font=("Arial", 12), bg="#f0f8ff")
        rain_label.grid(row=2, column=0, padx=20, pady=15, sticky=tk.W)
        self.rain_value = tk.Label(grid_frame, text=self.rain_level, font=("Arial", 16, "bold"), bg="#f0f8ff", fg="#9b59b6")
        self.rain_value.grid(row=2, column=1, padx=20, pady=15, sticky=tk.W)
        
        # 状态信息
        self.status_var = tk.StringVar(value="未连接")
        status_label = tk.Label(self.root, textvariable=self.status_var, bg="#f0f8ff", fg="#666", font=("Arial", 10))
        status_label.pack(pady=10)
    
    def get_available_ports(self):
        # 实际探测当前设备的可用串口
        ports = serial.tools.list_ports.comports()
        return [port.device for port in ports]
    
    def connect_serial(self):
        port = self.port_var.get()
        if not port:
            self.status_var.set("请选择串口")
            return
        
        try:
            # 获取用户选择的波特率
            baud_rate = int(self.baud_var.get())
            # 明确设置串口参数，确保与单片机一致
            self.serial_port = serial.Serial(
                port=port,
                baudrate=baud_rate,
                bytesize=serial.EIGHTBITS,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                timeout=1
            )
            self.status_var.set(f"已连接到 {port} (波特率: {baud_rate})")
            self.running = True
            # 启动数据读取
            self.read_serial_data()
        except Exception as e:
            self.status_var.set(f"连接失败: {str(e)}")
    
    def disconnect_serial(self):
        if self.serial_port and self.serial_port.is_open:
            self.serial_port.close()
            self.status_var.set("已断开连接")
        self.running = False
    
    def read_serial_data(self):
        if not self.running or not self.serial_port or not self.serial_port.is_open:
            return
        
        try:
            if self.serial_port.in_waiting > 0:
                line = self.serial_port.readline().decode('utf-8', errors='ignore').strip()
                self.parse_data(line)
        except Exception as e:
            self.status_var.set(f"读取错误: {str(e)}")
        
        # 继续读取数据
        self.root.after(100, self.read_serial_data)
    
    def parse_data(self, line):
        # 解析温度和湿度 (DHT22 OK: T:24.8C H:51.8%)
        dht_match = re.search(r'DHT22 OK: T:([\d.]+)C H:([\d.]+)%', line)
        if dht_match:
            self.temperature = dht_match.group(1)
            self.humidity = dht_match.group(2)
            self.update_display()
        
        # 解析光照强度 (Light: 240.0 lx)
        light_match = re.search(r'Light: ([\d.]+) lx', line)
        if light_match:
            self.light = light_match.group(1)
            self.update_display()
        
        # 解析温度和气压 (Temperature: 24.71 C, Pressure: 1008.66 hPa)
        temp_pressure_match = re.search(r'Temperature: ([\d.]+) C, Pressure: ([\d.]+) hPa', line)
        if temp_pressure_match:
            self.temperature = temp_pressure_match.group(1)
            self.pressure = temp_pressure_match.group(2)
            self.update_display()
        
        # 解析气压 (Press: 1008.15 hPa)
        press_match = re.search(r'Press: ([\d.]+) hPa', line)
        if press_match:
            self.pressure = press_match.group(1)
            self.update_display()
        
        # 解析降水等级 (Rain level: No rain)
        rain_match = re.search(r'Rain level: (.*)', line)
        if rain_match:
            self.rain_level = rain_match.group(1)
            self.update_display()
    
    def update_display(self):
        # 确保在主线程中更新界面
        def update():
            self.temp_value.config(text=f"{self.temperature}℃")
            self.hum_value.config(text=f"{self.humidity}%")
            self.light_value.config(text=f"{self.light} lx")
            self.pressure_value.config(text=f"{self.pressure} hPa")
            self.rain_value.config(text=self.rain_level)
        
        self.root.after(0, update)
    
    def on_closing(self):
        self.disconnect_serial()
        self.root.destroy()

if __name__ == "__main__":
    root = tk.Tk()
    app = WeatherStationApp(root)
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    root.mainloop()