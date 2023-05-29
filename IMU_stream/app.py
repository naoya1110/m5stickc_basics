import numpy as np
import matplotlib.pyplot as plt
import serial
from serial.tools import list_ports
import time
from datetime import datetime

import flet as ft
from flet.matplotlib_chart import MatplotlibChart
import os
import pandas as pd
from scipy.fft import fft, fftfreq



def draw_figure(ax_acc, ax_fft, t_list, accX_list, accY_list, accZ_list):
    
    ax_acc.clear()
    ax_fft.clear()
    
    line_accX, = ax_acc.plot(t_list, accX_list, lw=1, label="accX", color="#1f77b4")
    line_accY, = ax_acc.plot(t_list, accY_list, lw=1, label="accY", color="#ff7f0e")
    line_accZ, = ax_acc.plot(t_list, accZ_list, lw=1, label="accZ", color="#2ca02c")
    
    dt = 0.5E-3
    t_vals = np.arange(2*dt, 1+2*dt, dt)
    accX_interp = np.interp(t_vals, t_list, accX_list)
    accY_interp = np.interp(t_vals, t_list, accY_list)
    accZ_interp = np.interp(t_vals, t_list, accZ_list)
    
    n = t_vals.shape[0]
    freq = fftfreq(n, dt)[:n//2]
    fftX = np.abs(fft(accX_interp)[:n//2])/(n/ 2)
    fftY = np.abs(fft(accY_interp)[:n//2])/(n/ 2)
    fftZ = np.abs(fft(accZ_interp)[:n//2])/(n/ 2)    
    
    
    line_fftX, = ax_fft.plot(freq, fftX, lw=1, label="accX", color="#1f77b4")
    line_fftY, = ax_fft.plot(freq, fftY, lw=1, label="accY", color="#ff7f0e")
    line_fftZ, = ax_fft.plot(freq, fftZ, lw=1, label="accZ", color="#2ca02c")
    
    ax_acc.grid(True)
    ax_acc.legend(loc=4)
    ax_acc.set_xlim(0, max(t_list))

    ax_acc.set_ylim(-2, 2)
    ax_acc.set_yticks(np.arange(-2, 2.1, 0.5))
    ax_acc.set_ylabel("Acceleration (G)")
    ax_acc.set_xlabel("Time (s)")
    
    ax_fft.grid(True)
    ax_fft.legend(loc=4)
    #ax_fft.set_xlim(0, max(t_list))

    ax_fft.set_yscale("log")
    ax_fft.set_ylabel("Acceleration (G)")
    ax_fft.set_xlabel("Frequency (Hz)")
    
    return ax_acc, ax_fft



def main(page: ft.page):
    
    ports = list_ports.comports()
    devices = [info.device for info in ports]
    dropdown = ft.Dropdown(height=60, width=200,
                        options=[ft.dropdown.Option(portname) for portname in devices])
    save_btn = ft.Checkbox(label="Save Data", value=False)
    memo_txt = ft.TextField(label="memo", text_size=20, width=300)

    DIR = "imu_data"
    if not os.path.exists(DIR):
        os.makedirs(DIR)

    fig = plt.figure(figsize=(8, 8))        # グラフのサイズを指定
    plt.rcParams["font.size"] = 10    # フォントサイズ
    plt.tight_layout()
    ax_acc = fig.add_subplot(2, 1, 1)
    ax_fft = fig.add_subplot(2, 1, 2)

    t_list = np.linspace(0,1,10)
    accX_list = [0]*10
    accY_list = [1]*10
    accZ_list = [-1]*10
    ax_acc, ax_fft = draw_figure(ax_acc, ax_fft, t_list, accX_list, accY_list, accZ_list)

    is_receiving_data = False

    page.title = "IMU data viewer"
    page.window_width = 700
    page.window_height=700
    page.bgcolor = "WHITE"

    app_title = ft.Text("IMU data viewer",size=40)
    current_time = ft.Text(size=20)
    message = ft.Text(size=20)
    plot = MatplotlibChart(fig, expand=True)


    page.add(app_title)
    page.add(current_time)
    page.add(memo_txt)
    page.add(save_btn)
    page.add(dropdown)
    page.add(message)
    page.add(plot)
    
    plot.value = fig
    now_str = datetime.now().strftime("%Y/%m/%d %H:%M:%S")
    current_time.value = now_str
    message.value = "Please select port"
    message.color = "red"
    page.update() 
    
    while True:
        
        selected_port = dropdown.value

        if selected_port:
            message.value = "Port selected"
            page.update()
            
            with serial.Serial(selected_port, 115200, timeout=1) as ser:
                print(ser)
                    
                while True:
                    try:
                        line = ser.readline()   # read a '\n' terminated line
                        text_data = line.decode().split("\n")[0]
                        print(text_data)
                        
                        if "start" in text_data:
                            is_receiving_data = True
                            message.value = "Receiving data"
                            message.color = "blue"
                            page.update()

                            t_list = []
                            accX_list = []
                            accY_list = []
                            accZ_list = []
                            now_str = datetime.now().strftime("%Y/%m/%d %H:%M:%S")
                            current_time.value = now_str
                            
                        elif is_receiving_data and ("end" in text_data):
                            print("end receiving data")
                            is_receiving_data = False
                            message.value = "data received"
                            message.color = "blue"
                            
                            ax_acc, ax_fft = draw_figure(ax_acc, ax_fft, t_list, accX_list, accY_list, accZ_list)

                            now_str = datetime.now().strftime("%Y/%m/%d %H:%M:%S")
                            current_time.value = now_str
                            
                            plot.value = fig
                            
                            if save_btn.value == True:
                                df = pd.DataFrame()
                                df["time(us)"]=t_list
                                df["accX(G)"]=accX_list
                                df["accY(G)"]=accY_list
                                df["accZ(G)"]=accZ_list
                                now_str = datetime.now().strftime("%Y%m%d_%H%M%S")
                                df.to_csv(os.path.join(DIR, f"{now_str}_{memo_txt.value}.csv"), index=False)
                            
                            page.update()
                        
                        else:
                            if is_receiving_data:
                                t, accX, accY, accZ = text_data.split(" ")
                                t_list.append(int(t)/1E6)
                                accX_list.append(float(accX))
                                accY_list.append(float(accY))
                                accZ_list.append(float(accZ))
                                print(t, accX, accY, accX)
                                
                        if text_data == "":
                            now_str = datetime.now().strftime("%Y/%m/%d %H:%M:%S")
                            current_time.value = now_str
                            page.update()





                    except ValueError:
                        pass

                    except KeyboardInterrupt:
                        # print(t_list)
                        print("Keyboard Interrupt")
                        print(len(t_list))
                        ser.close()
                        break
                    


ft.app(target=main)