
#ifndef CMMC_DUST_MODULE_H
#define CMMC_DUST_MODULE_H

#include <WiFi.h>
#include <Arduino.h>
#include "PMS.h"

// #DEFINE

// static    CMMC_Bucket _pm10_bucket;
// static    CMMC_Bucket _pm2_5_bucket;
// static    CMMC_Bucket _pm1_bucket;

class DustModule
{
public:
  DustModule(HardwareSerial *);
  uint16_t _pm10 = 0;
  uint16_t _pm2_5 = 0;
  uint16_t _pm1 = 0;
  void setup();
  void loop();
  void isLongPressed();
  void configLoop();
  const char *name()
  {
    return "DustModule";
  }

  PMS::DATA data;

protected:
private:
  PMS *pms;
  Stream *_serial;
  uint32_t counter = 0;
};

#endif
