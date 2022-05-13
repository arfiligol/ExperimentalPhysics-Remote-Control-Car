import cv2 # OpenCV
import numpy as np
import matplotlib.pyplot as plt
import time # Use for calculate the time running a function need.

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
video = cv2.VideoCapture("http://192.168.50.219:81/stream")

# # config the size of video
width = 240
height = 240

video.set(cv2.CAP_PROP_FRAME_WIDTH, width)
video.set(cv2.CAP_PROP_FRAME_HEIGHT, height)

# setup the threshold
lower = np.array([150, 60, 171])
upper = np.array([210, 122, 255])
print(lower)

# catch the video
ret, frame = video.read()



while ret:
    # Starting time
    start = time.time()


    # Read a picture
    ret, frame = video.read()
    
    # Convert BGR to HSV
    HSV_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Binary the frame
    filtered = cv2.inRange(HSV_frame, lower, upper)
    cnt, cntImg = cv2.findContours(filtered, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE) # The parameters need to be check since we don't understant their meaning
    
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
            print("Turn right")
        elif (center[0] < HSV_frame.shape[1]/2 - 25):
            print("Turn left")
        else:
            print("Move forward")
            

        cv2.rectangle(HSV_frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        cv2.circle(HSV_frame, center=(x + (w//2), y + (h//2)), radius=1, color=(0, 0, 255), thickness=-1)

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
        cv2.waitKey(0)

    #print("The time it need: " + str(end - start))