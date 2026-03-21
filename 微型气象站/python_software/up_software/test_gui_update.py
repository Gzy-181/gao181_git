import tkinter as tk
from tkinter import ttk
import time
import threading

class TestWeatherStationApp:
    def __init__(self, root):
        self.root = root
        self.root.title("微型气象站测试")
        self.root.geometry("400x350")
        self.root.resizable(False, False)
        
        # 设置背景颜色
        self.root.configure(bg="#f0f8ff")
        
        # 数据存储
        self.temperature = "--"
        self.humidity = "--"
        self.rain_level = "No rain"
        self.wind_level = "Low"
        
        # 创建UI
        self.create_widgets()
        
        # 启动模拟数据线程
        self.running = True
        self.test_thread = threading.Thread(target=self.simulate_data)
        self.test_thread.daemon = True
        self.test_thread.start()
    
    def create_widgets(self):
        # 标题
        title_frame = tk.Frame(self.root, bg="#f0f8ff")
        title_frame.pack(pady=10)
        title_label = tk.Label(title_frame, text="微型气象站测试", font=("Arial", 18, "bold"), bg="#f0f8ff")
        title_label.pack()
        
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
        self.status_var = tk.StringVar(value="测试模式")
        status_label = tk.Label(self.root, textvariable=self.status_var, bg="#f0f8ff", fg="#666")
        status_label.pack(pady=10)
    
    def simulate_data(self):
        import random
        
        while self.running:
            # 模拟数据更新
            self.temperature = str(round(random.uniform(20, 30), 1))
            self.humidity = str(round(random.uniform(60, 80), 1))
            self.rain_level = random.choice(["No rain", "Light rain", "Moderate rain", "Heavy rain"])
            self.wind_level = random.choice(["Low", "Medium", "High"])
            
            # 更新界面
            self.update_display()
            
            # 打印调试信息
            print(f"模拟数据更新: 温度={self.temperature}℃, 湿度={self.humidity}%, 降水={self.rain_level}, 风力={self.wind_level}")
            
            # 等待一段时间
            time.sleep(1)
    
    def update_display(self):
        # 确保在主线程中更新界面
        def update():
            self.temp_value.config(text=f"{self.temperature}℃")
            self.hum_value.config(text=f"{self.humidity}%")
            self.rain_value.config(text=self.rain_level)
            self.wind_value.config(text=self.wind_level)
        
        self.root.after(0, update)
    
    def on_closing(self):
        self.running = False
        self.root.destroy()

if __name__ == "__main__":
    root = tk.Tk()
    app = TestWeatherStationApp(root)
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    root.mainloop()