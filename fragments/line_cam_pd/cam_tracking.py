import cv2 as cv
import math


def get_trend(cnt, dy=100, img=None):
    """Получение отклонения и угла наклона линии

    Args:
        cnt (_type_): _description_
        dy (int, optional): _description_. Defaults to 100.
        img (_type_, optional): _description_. Defaults to None.

    Returns:
        _type_: _description_
    """
    try:
        # получение данных о прямой, всеченной в контур
        vx, vy, x, y = cv.fitLine(cnt, cv.DIST_L2, 0, 0.01, 0.01)
        vx, vy, x, y = vx[0], vy[0], x[0], y[0]

        line_tang = vx/vy  # тангенс угла наклона линии
        radians = math.atan(line_tang)  # угол наклона/производная ошибка

        m = 150  # некоторый коэфф., отвечающий за длину нарисованной линии

        x1 = x - vx*m  # координаты двух точек, по которым будет строиться линия
        y1 = y - vy*m
        x2 = x + vx*m
        y2 = y + vy*m

        dx = line_tang * dy  # отклонение линии/пропорциональная ошибка
        x_cross = y1-x1/line_tang  # Координата пересечения линии и низа кадра

        if img != None:
            cv.line(img, (int(x1), int(y1)), (int(x2), int(y2)),
                    (0, 255, 0), 2)  # построение линии
            cv.circle(img, (int(x), int(y)), 3, (255, 0, 0), 4)
            # cv.circle(img, (int((rows-dy-b)*line_tang),
            #                 int(rows-dy)), 3, (255, 0, 0), 4)

        return (dx, radians)

    except Exception as e:
        print(e)
        pass


def PD_reg(Kp=1, Kd=0, d_x=0, dif_dx=0, norm_speed=200):
    """Расчёт скоростей моторов через PD регулятор

    Args:
        Kp (int, optional): _description_. Defaults to 1.
        Kd (int, optional): _description_. Defaults to 0.
        d_x (int, optional): _description_. Defaults to 0.
        dif_dx (int, optional): _description_. Defaults to 0.
        norm_speed (int, optional): _description_. Defaults to 200.

    Returns:
        _type_: _description_
    """

    reaction = Kp*d_x + Kd*dif_dx  # настройка ПИД-регулятора
    l_speed = norm_speed + reaction
    r_speed = norm_speed - reaction

    return (l_speed, r_speed)


if __name__ == "__main__":

    vid = cv.VideoCapture('output.avi')
    while (vid.isOpened()):
        ret, frame = vid.read()
        if (ret == True):
            gray_frame = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
            bin_frame = cv.threshold(gray_frame, 80, 255, 1)[1]
            contours = cv.findContours(
                bin_frame, cv.RETR_LIST,
                cv.CHAIN_APPROX_SIMPLE)[0]
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
