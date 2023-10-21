#pragma once
#define sensors_H
#include <Arduino.h>
#include <GyverFilters.h>

#define BUTTON_PIN 12
#define L_SENS_PIN A2
#define R_SENS_PIN A1

#define SENSOR_RANGE 100

#define FILT_COEF 0.5
#define FILT_STEP 1

void init_filters();

int midArifm(int pin);

void sensor_calibration();

int get_error();
