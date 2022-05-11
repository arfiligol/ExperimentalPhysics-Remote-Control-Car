import serial
import cv2


COM_PORT = "COM4" # Select the port we are listen
BAUD_RATES = 115200 # The baud rates we are using
ser = serial.Serial(COM_PORT, BAUD_RATES) # initialize the Serial port

x = 0

try:
    while True:
        print(x)
        data_raw = ser.readline() # read one line
        #data = data_raw.decode() # The default is using UTF-8 to decode
        print('接收到的原始資料:',  data_raw)
        #print('接收到的資料:', data, end="")
        x += 1

except KeyboardInterrupt:
    ser.close()
    print("See you!")

except Exception as e:
    print(e)
