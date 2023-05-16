import serial
import time

with serial.Serial('COM5', 115200, timeout=1) as ser:
    print("start")
    while True:
        try:
            line = ser.readline()   # read a '\n' terminated line
            line = line.decode()
            if len(line)>0:
                print(line, end="")
        
        except:
            pass
        
        time.sleep(0.1)