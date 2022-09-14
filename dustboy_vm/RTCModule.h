// #include <_Time.h>
#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>

// RtcDS3231<TwoWire> Rtc(Wire);
/* for normal hardware wire use above */

#ifndef CMMC_RTC_MODULE_H
#define CMMC_RTC_MODULE_H

class RTCModule
{
public:
  RTCModule(TwoWire *);
  void printTime();
  bool isSynced();
  void printDateTime(const RtcDateTime &);
  char datestring[20];
  char filenamestring[20];
  char timestring[20];
  RtcDS3231<TwoWire> *Rtc;
  void setup();
  void loop();
  bool toggle = 0;
  RtcDateTime *getCurrentRtcDateTime();
  RtcTemperature temperature;

private:
  char _timeStringBuffer[100];
  time_t *_t;
  struct tm timeinfo;
  bool _synced = false;
  RtcDateTime _now;
};

#endif
