#include "gyro.h"

float degrees = 0.0;
float speed_1;
float speed_0 = 0.0;
float time_0;
float time_1;
const float k = 16.41;
#ifdef USE_SPI
ICM_20948_SPI myICM; 
#else
ICM_20948_I2C myICM; 
#endif

void initGyro() {
  
  while (!SERIAL_PORT)
  {
  };

#ifdef USE_SPI
  SPI_PORT.begin();
#else
  WIRE_PORT.begin();
  WIRE_PORT.setClock(400000);
#endif

  bool initialized = false;
  while (!initialized)
  {

#ifdef USE_SPI
    myICM.begin(CS_PIN, SPI_PORT);
#else
    myICM.begin(WIRE_PORT, AD0_VAL);
#endif

    SERIAL_PORT.print(F("Initialization of the sensor returned: "));
    SERIAL_PORT.println(myICM.statusString());
    if (myICM.status != ICM_20948_Stat_Ok)
    {
      SERIAL_PORT.println("Trying again...");
      delay(500);
    }
    else
    {
      initialized = true;
    }
  }

  ICM_20948_fss_t myFSS;
  myFSS.g = dps2000; 
  myICM.setFullScale((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), myFSS);
  // ICM_20948_dlpcfg_t myDLPcfg;
  //myDLPcfg.g = gyr_d361bw4_n376bw5; // (ICM_20948_GYRO_CONFIG_1_DLPCFG_e)


 // myICM.setDLPFcfg((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), myDLPcfg);
  myICM.sleep(false);
  myICM.lowPower(false);
}

float get_degrees(float degrees_0, long int times){
  if (myICM.dataReady())
  {
  myICM.getAGMT();         
  speed_1 =  myICM.agmt.gyr.axes.z;
  if (speed_1 < 100 and speed_1 > -100) speed_1 = 0.0;
  time_1 = millis();
  float res = degrees_0 + (speed_0+speed_1)/2*(time_1-times)/1000/k;
  speed_0 = speed_1;
  time_0 = time_1;
  return res, time_0;
  }
  else
  {
    SERIAL_PORT.println("Sorry, no data. Let's try again");
    return 0.0;
  }
}



