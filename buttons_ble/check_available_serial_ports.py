from serial.tools import list_ports

ports = list_ports.comports()    # ポートデータを取得

for port in ports:
    print(port)