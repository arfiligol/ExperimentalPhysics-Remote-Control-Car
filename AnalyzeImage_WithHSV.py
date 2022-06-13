from shutil import move
import cv2 # OpenCV
import numpy as np
import matplotlib.pyplot as plt
import time # Use for calculate the time running a function need.
import serial # Use for connecting arduino and send command to it


# Setup serial port connection configuration
COM_PORT = "/dev/cu.usbserial-14120" # Select the port we are listen
BAUD_RATES = 115200 # The baud rates we are using
arduinoBoard = serial.Serial(COM_PORT, BAUD_RATES) # initialize the Serial port
def sendCommand(command):
    global arduinoBoard
    arduinoBoard.write(str(command).encode())





# For click mouse check things
def mouse_click(event, x, y, flags, para):
    global lower, upper
    # If we click the mouse
    if event == cv2.EVENT_LBUTTONDOWN:
        # Show the value of the color where the mouse click
        print("BGR:", BGR_frame[y, x])
        print("HSV:", HSV_frame[y, x])

        # check the value whether it is higher than lower or upper
        for i in range(0, 3):
            if (HSV_frame[y, x][i] < lower[i]):
                lower[i] = HSV_frame[y, x][i]
            elif (HSV_frame[y, x][i] > upper[i]):
                upper[i] = HSV_frame[y, x][i]
        print("Lower: " + str(lower))
        print("Upper: " + str(upper))


# Get the picture from the test image
video = cv2.VideoCapture("http://192.168.4.1:81/stream")


# setup the threshold
lower = np.array([150, 34, 171])
upper = np.array([210, 122, 255])
print(lower)

# catch the video
ret, frame = video.read()


# Create an variable to store value
turningTimes = 0
turnRight = True


try:
    while ret:
        # Starting time
        start = time.time()


        # Read a picture
        ret, frame = video.read()
        # If read the picture
        if ret:
            # Convert BGR to HSV
            HSV_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

            # Binary the frame
            filtered = cv2.inRange(HSV_frame, lower, upper)
            cnt, cntImg = cv2.findContours(filtered, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE) # The parameters need to be check since we don't understant their meaning
            
            # If cnt has detect any contour, then execute the movement
            if (cnt):
                # Check the biggest area contour to be the laser point
                if len(cnt) > 1:
                    areaList = []
                    for c in cnt:
                        areaList.append(cv2.contourArea(c))
                    maximumArea = max(areaList)
                else:
                    if cnt:
                        maximumArea = cv2.contourArea(cnt[0])
                    else:
                        pass
                
                for c in cnt:
                    # If the contour areas is more then one, only deal the biggest area
                    if cv2.contourArea(c) < maximumArea:
                        continue
                    else:
                        # If the area is small then 100, regard it as noise
                        if cv2.contourArea(c) < 100:
                            continue
                    
                    #print(cv2.contourArea(c))

                    # Get the values of (x, y) and the width and height of the contour
                    (x, y, w, h) = cv2.boundingRect(c)

                    # Give the center of the counter
                    center = (x + w/2, y + h/2)
                    #print(center)
                    if (center[0] > HSV_frame.shape[1]/2 + 25):
                        if (center[0] > HSV_frame.shape[1]*3/4 - 10):
                            moveCommand = '6'
                            sendCommand(moveCommand)
                            print("Turn right")
                        else:
                            moveCommand = '9'
                            sendCommand(moveCommand)
                            print("Front right")
                    elif (center[0] < HSV_frame.shape[1]/2 - 25):
                        if (center[0] < HSV_frame.shape[1]/4 + 10):
                            moveCommand = '4'
                            sendCommand(moveCommand)
                            print("Turn left")
                        else:
                            moveCommand = '7'
                            sendCommand(moveCommand)
                            print("Front left")
                    else:
                        moveCommand = '8'
                        sendCommand(moveCommand)
                        print("Move forward")
                        

                    cv2.rectangle(HSV_frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
                    cv2.circle(HSV_frame, center=(x + (w//2), y + (h//2)), radius=1, color=(0, 0, 255), thickness=-1)

            else: # If didn't detect any contour, then stop the car and start turning servo motor
                # Stop the car
                if (turningTimes < 100):
                    turningTimes += 1
                else:
                    moveCommand = '5'
                    sendCommand(moveCommand)
                    print("Stop")

                # if (turnRight):
                #     if (turningTimes < 80):
                #         turnCommand = 'a ' + str(turningTimes)
                #         sendCommand(turnCommand)
                #         time.sleep(0.001)
                #         print(turnCommand)
                #         turningTimes += 1
                #     else:
                #         if (turningTimes < 160):
                #             turnCommand = 'a ' + str(160 - turningTimes)
                #             sendCommand(turnCommand)
                #             time.sleep(0.001)
                #             print(turnCommand)
                #             turningTimes += 1
                #         else:
                #             turnRight = False
                #             turningTimes = 0
                # else:
                #     if (turningTimes < 80):
                #         turnCommand = 'b ' + str(turningTimes)
                #         sendCommand(turnCommand)
                #         time.sleep(0.001)
                #         print(turnCommand)
                #         turningTimes += 1
                #     else:
                #         if (turningTimes < 160):
                #             turnCommand = 'b ' + str(160 - turningTimes)
                #             sendCommand(turnCommand)
                #             time.sleep(0.001)
                #             print(turnCommand)
                #             turningTimes += 1
                #         else:
                #             turnRight = True
                #             turningTimes = 0

                
            # Analyze end time
            end = time.time()

            # Check if we click the mouse
            cv2.namedWindow("RGB")
            cv2.setMouseCallback("RGB", mouse_click)

            
            # Show the image with binarying
            cv2.imshow('The result', filtered)

            # Show the image with HSV color
            cv2.imshow('HSV', HSV_frame)

            # Show the image with RGB color
            BGR_frame = cv2.cvtColor(HSV_frame, cv2.COLOR_HSV2BGR)
            cv2.imshow('RGB', BGR_frame)
            if cnt:
                cv2.waitKey(1)
            else:
                moveCommand = '5'
                sendCommand(moveCommand)
                cv2.waitKey(20)

            #print("The time it need: " + str(end - start))

except Exception as e:
    print(e)

finally:
    arduinoBoard.close()
