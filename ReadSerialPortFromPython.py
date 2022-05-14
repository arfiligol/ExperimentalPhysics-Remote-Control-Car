import serial

COM_PORT = "COM3" # Select the port we are listen
BAUD_RATES = 115200 # The baud rates we are using
ser = serial.Serial(COM_PORT, BAUD_RATES) # initialize the Serial port

try:
    while True:
        moveCommand = input('Input the movement command: ')
        ser.write(moveCommand.encode())

except KeyboardInterrupt:
    ser.close()
    print("See you!")

except Exception as e:
    print(e)
