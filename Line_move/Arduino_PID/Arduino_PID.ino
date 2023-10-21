#include "motors.h"
#include "sensors.h"
#include <GyverPID.h>

#define NORMAL_SPEED 255
#define PID_TIMER 10
#define KP 4,9
#define KD 0.3
#define KI 0.1
#define NORM_PID 0

GyverPID PID(0, 0, 0, PID_TIMER);

void InitPID(){
  PID.setDirection(NORMAL);// ставим "направление" регулятора- NORMAL=>выход регулятора напрямую влияет на датчик
  PID.setpoint = NORM_PID;// ставим значение, к которму будет стремиться регулятор
  PID.setLimits(-NORMAL_SPEED*2, NORMAL_SPEED*2); 
  PID.Kp = KP;
  PID.Ki = KI;
  PID.Kd = KD;
}

void setup(){

  Serial.begin(9600);

  InitMotors();
  init_filters();
  InitPID();
  sensor_calibration();

}


void loop() {
    PID.input = get_error();
    int pid_ret = PID.getResultTimer();

    int l_speed = NORMAL_SPEED + pid_ret;
    int r_speed = NORMAL_SPEED - pid_ret;
    SetTankSpeed(l_speed, r_speed);
 }