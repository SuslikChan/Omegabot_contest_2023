#pragma once
#define gyro_H
#include <Arduino.h>
#include "ICM_20948.h" 

// #define USE_SPI       
#define SERIAL_PORT Serial
#define SPI_PORT SPI 
#define CS_PIN 10  
#define WIRE_PORT Wire
#define AD0_VAL 0


void initGyro();
float get_degrees(float degrees_0, long int times);
void fuck_angls();
