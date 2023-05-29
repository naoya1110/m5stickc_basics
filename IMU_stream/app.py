import numpy as np
import matplotlib.pyplot as plt
import serial
from serial.tools import list_ports
import time
from datetime import datetime

import flet as ft
from flet.matplotlib_chart import MatplotlibChart



def draw_figure(ax_acc, ax_fft, t_list, 
                accX_list, accY_list, accZ_list):
    
    ax_acc.clear()
    ax_fft.clear()
    
    line_accX, = ax_acc.plot(t_list, accX_list, lw=1, label="accX", color="#1f77b4")
    line_accY, = ax_acc.plot(t_list, accY_list, lw=1, label="accY", color="#ff7f0e")
    line_accZ, = ax_acc.plot(t_list, accZ_list, lw=1, label="accZ", color="#2ca02c")
    
    ax_acc.grid(True)
    ax_acc.legend(loc=4)
    ax_acc.set_xlim(0, max(t_list))

    ax_acc.set_ylim(-2, 2)
    ax_acc.set_yticks(np.arange(-2, 2.1, 0.5))
    ax_acc.set_ylabel("Acceleration (G)")
    ax_acc.set_xlabel("Time (s)")
    
    return ax_acc



def main(page: ft.page):
    
    ports = list_ports.comports()
    devices = [info.device for info in ports]
    dropdown = ft.Dropdown(height=60, width=150,
                        options=[ft.dropdown.Option(portname) for portname in devices])


    fig = plt.figure(figsize=(8, 8))        # グラフのサイズを指定
    plt.rcParams["font.size"] = 10    # フォントサイズ
    ax_acc = fig.add_subplot(2, 1, 1)
    ax_fft = fig.add_subplot(2, 1, 2)

    t_list = [0,1]
    accX_list = [0,0]
    accY_list = [1,1]
    accZ_list = [-1,-1]
    ax_acc = draw_figure(ax_acc, ax_fft, t_list, accX_list, accY_list, accZ_list)

    is_receiving_data = False

    page.title = "IMU data viewer"
    page.window_width = 700
    page.window_height=700
    page.bgcolor = "WHITE"

    app_title = ft.Text("IMU data viewer",size=40)
    current_time = ft.Text(size=30)
    message = ft.Text(size=30)
    plot = MatplotlibChart(fig, expand=True)


    page.add(app_title)
    page.add(current_time)
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
                        
                        if text_data=="start":
                            is_receiving_data = True
                            message.value = "Receiving data"
                            message.color = "blue"
                            page.update()

                            t_list = []
                            accX_list = []
                            accY_list = []
                            accZ_list = []
                            
                        elif text_data == "end":
                            print("end receiving data")
                            is_receiving_data = False
                            message.value = "data received"
                            message.color = "blue"
                            
                            ax_acc = draw_figure(ax_acc, ax_fft, t_list, accX_list, accY_list, accZ_list)
                        
                            plot.value = fig
                            page.update()
                            page.update()
                        
                        else:
                            if is_receiving_data:
                                t, accX, accY, accZ = text_data.split(" ")
                                t_list.append(int(t))
                                accX_list.append(float(accX))
                                accY_list.append(float(accY))
                                accZ_list.append(float(accZ))
                                print(t, accX, accY, accX)





                    except ValueError:
                        pass

                    except KeyboardInterrupt:
                        # print(t_list)
                        print("Keyboard Interrupt")
                        print(len(t_list))
                        ser.close()
                        break
                    


ft.app(target=main)