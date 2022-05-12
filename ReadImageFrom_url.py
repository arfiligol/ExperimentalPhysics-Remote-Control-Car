import cv2 # 匯入opencv函式庫
import time # This package is used to calculate the time we need to run functions.


cap = cv2.VideoCapture("http://192.168.50.219:81/stream") # Instantce an object of VideoCapture, get the video (image) from an url.

while True: 
    if (cap.isOpened()): # If the cap is normally could be use, then...
        ret, img = cap.read() # Read an img from the source, this read() function will return two value. One is the imge data (probably five dimension data), the other is Bool value that whether the image could be read.
        
        cv2.imshow("image", img) # Show the image, with the window title "image", and with the data store in "img" variable.
        cv2.waitKey(1) # Wait for... (up to you).