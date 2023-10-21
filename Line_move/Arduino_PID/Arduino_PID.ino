#include "motors.h"
#include "sensors.h"
#include <GyverPID.h>

#define NORMAL_SPEED 255
#define PID_TIMER 10
#define KP 6
#define KD 0.3
#define KI 0
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
    if ((digitalRead(BUT_PIN)==1)&(flag==0)){
      R_F_Black = analogRead(R_SENS_PIN);
      L_F_Black = analogRead(L_SENS_PIN);
      flag++;
    }
    if ((digitalRead(BUT_PIN)==1)&(flag==1)){
      R_F_White = analogRead(R_SENS_PIN);
      L_F_White = analogRead(L_SENS_PIN);
      flag++;
    }
  
// Чтение значений с датчиков линии
    int leftSensorValue = filt.filteredTime(map(analogRead(L_SENS_PIN),L_F_White,L_F_Black,0,1023));
    int rightSensorValue = filt.filteredTime(map(analogRead(R_SENS_PIN),R_F_White,R_F_Black,0,1023));
    int err =leftSensorValue-rightSensorValue;

    PID.input = err;

    int pid_ret = PID.getResultTimer();

    int l_speed = NORMAL_SPEED + pid_ret;
    int r_speed = NORMAL_SPEED - pid_ret;
    SetTankSpeed(l_speed, r_speed);
 }