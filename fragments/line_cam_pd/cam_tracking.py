import cv2 as cv
import numpy as np
import math

def get_trend(img, cnt):
    try:
        vx, vy, x, y = cv.fitLine(cnt, cv.DIST_L2, 0, 0.01, 0.01) #получение данных о прямой, всеченной в контур
        vx, vy, x, y = vx[0], vy[0], x[0], y[0] #оставляем только числовые данные
        m = 150 #некоторый коэфф., отвечающий за длину нарисованной линии
        x1 = x - vx*m
        y1 = y - vy*m
        x2 = x + vx*m
        y2 = y + vy*m #координаты двух точек, по которым будет строиться линия
        cv.line(img, (int(x1), int(y1)), (int(x2),int(y2)), (0,255,0), 2) #построение линии
        line_tang = (x2-x1)/(y2-y1) #поиск угла наклона линии
        rows = img.shape[0] #поиск высоты изображения
        dy=rows/3 #поиск расстояния от верхнего края изображения, до точки
        dx = line_tang * dy #отклонение линии/пропорциональная ошибка
        b=y1-x1/line_tang 
        cv.circle(img, (int((rows-dy-b)*line_tang),int(rows-dy)), 3, (255,0,0), 4)
        Kp = 1
        Kd = 0
        angle = math.atan(line_tang) #угол наклона/производная ошибка
        du = Kp*dx + Kd*angle #настройка ПИД-регулятора
        #print(angle)
        return du
    except Exception as e:
        print(e)
        pass

vid = cv.VideoCapture('resourses/output.avi')
while (vid.isOpened()):
    ret, frame = vid.read()
    if (ret == True):
        gray_frame = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
        bin_frame = cv.threshold(gray_frame, 80, 255, 1)[1]
        contours = cv.findContours(bin_frame, cv.RETR_LIST, cv.CHAIN_APPROX_SIMPLE)[0]
        output = cv.drawContours(frame, contours, -1, (0, 0, 255), 2)
        max_ar = 0
        max_cn = 0
        for cnt in contours:
            area = cv.contourArea(cnt)
            if area >= max_ar:
                max_ar = area
                max_cn = cnt
        du = get_trend(output, max_cn)
        print(du)
        cv.imshow("cam", output)
        k = cv.waitKey(5)
        if k == ord('q') or k == 27: 
            break
    else:
        break
vid.release()
cv.destroyAllWindows()