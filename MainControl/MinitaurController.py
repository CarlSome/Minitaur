import serial
import time

serialPort = "COM9"
baudRate = 115200
arduino = serial.Serial(serialPort,baudRate,timeout=0.5)

time.sleep(1)
while 1:
    command = input('输入指令：')
    command = bytes(command, encoding = "utf8")
    arduino.write(command)
    time.sleep(3)   # 等待电机转动完毕才能读取返回的信息。可以通过速度计算得到精准地等待时间
    msg = arduino.read(20)
    print(msg)
