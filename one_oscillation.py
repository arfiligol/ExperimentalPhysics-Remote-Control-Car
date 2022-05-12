import cv2
import numpy as np
from scipy import optimize #for fitting
import matplotlib.pyplot as plt

#抓取影片
video = cv2.VideoCapture(r'OpenCV/Videos/IMG_0001.MOV')

# 設定影片尺寸
width = 1920
height = 1080

video.set(cv2.CAP_PROP_FRAME_WIDTH, width)
video.set(cv2.CAP_PROP_FRAME_HEIGHT, height)

#寫入影片資訊
fourcc = cv2.VideoWriter_fourcc('m', 'p', '4', 'v')
output = cv2.VideoWriter('OpenCV/video_record.mp4', fourcc, 240, (width, height))

#設定物體lower, upper (just one)
lower = np.array([40, 0, 0])
upper = np.array([100, 50, 20])


#for recording
FPS = 240
#每張圖的時間間隔
time_interval = 1/240
#let time = 0 for begining
time = 0
#建立列表記錄點
data_x = []
data_y = []
data_t = []

#抓取第一張圖
ret, frame = video.read()
#檢測用show
check = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
plt.imshow(check)
plt.show()

#track start
while ret: #全部檢測
    #抓取圖片
    ret, frame = video.read()
    if ret == False:
        break
    
    #blur
    blur = cv2.GaussianBlur(frame, (11, 11), cv2.BORDER_DEFAULT)

    #detect area (only one)
    #找出在範圍內的區域 (已二值化）
    filtered = cv2.inRange(frame, lower, upper)
    #邊緣檢測
    cnt, cntImg = cv2.findContours(filtered, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    for c in cnt:
        #排除面積太小的範圍
        if cv2.contourArea(c) < 2000:
            continue
        #取出四個值 (x, y)為矩陣左上角的坐標, (w, h)是矩陣的寬和高
        (x, y, w, h) = cv2.boundingRect(c)

        #計算矩陣中心加進列表
        center_x = x + (w/2)
        center_y = y + (h/2)
        data_x.append(center_x)
        data_y.append(-center_y)
        data_t.append(time)
        time += time_interval
        
        #畫出綠色邊匡
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        cv2.circle(frame, center=(x + (w//2), y + (h//2)), radius=5, color=(0, 0, 255), thickness=-1)


    #顯示圖片
    cv2.imshow('filtered', frame)
    #紀錄影片
    output.write(frame)
    #停頓加偵測按鍵
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

#釋放video並摧毀所有視窗
video.release()
output.release()
cv2.destroyAllWindows()


#fitting
def fitting_func(x_time, m, r, k, x0, x_dot0, midden):
    return ( x0 * np.cos( (( (4 * m * k) - r ** 2 ) ** (1/2)) * x_time / ( 2 * m ) ) + x_dot0 * np.sin( (( (4 * m * k) - r ** 2 ) ** (1/2)) * x_time / ( 2 * m ) ) ) * np.exp( (-r * x_time) / ( 2 * m ) ) + midden
#give the initial condition
guess = (0.00525, 0.0001, 5, max(data_x), 0, (max(data_x) + min(data_x)) / 2)
popt, pcov = optimize.curve_fit(fitting_func, data_t, data_x, p0=guess, maxfev=1000000)
#data record
data_fitting_x = []
for x_time in data_t:
    value = fitting_func(x_time, *popt)
    data_fitting_x.append(value)



#draw trajectory
fig1 = plt.figure()
plt.title("Trajectory of object\n m:{:} r:{:} k:{:} x0:{:} x_dot0:{:} balance point:{:}".format(*popt))
plt.plot(data_t, data_x, marker=".", markersize=2.5, label="x-axis object1 position")
plt.plot(data_t, data_fitting_x, marker=".", markersize=2.5, label="x-axis object1 fitting")
plt.xlabel("time (s)")
plt.ylabel("x position (pixel)")
plt.legend(loc=1)
plt.show()
name = input("please enter the name of figure:")
fig1.savefig("OpenCV/figure_record/{:}.png".format(name))
