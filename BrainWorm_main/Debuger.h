#pragma once
#define DEBUG 
#ifdef  DEBUG
  #define EN_OFFSET true
  #define OFFSET    0.05
  #define DEBUG_PRINT(x) Serial.println(String(int(millis()/1000))+"  >>  "+x)
  #define DEBUG_INIT()   Serial.begin(115200);
  #else
  #define DEBUG_PRINT(x)
  #define DEBUG_INIT()

#endif