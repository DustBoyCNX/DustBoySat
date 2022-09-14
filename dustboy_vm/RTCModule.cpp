#include "RTCModule.h"
// #include <_Time.h>

#define countof(a) (sizeof(a) / sizeof(a[0]))

void RTCModule::printDateTime(const RtcDateTime &dt)
{
  snprintf_P(filenamestring,
             countof(filenamestring),
             PSTR("%02u%02u%02u.csv"),
             dt.Year(),
             dt.Month(),
             dt.Day());

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u"),
             dt.Day(),
             dt.Month(),
             dt.Year());

  snprintf_P(timestring,
             countof(timestring),
             PSTR("%02u:%02u:%02u"),
             dt.Hour(),
             dt.Minute(),
             dt.Second());

  // Serial.println(datestring);
  Serial.println(timestring);
}

RTCModule::RTCModule(TwoWire *w)
{
  this->Rtc = new RtcDS3231<TwoWire>(*w);
}

void RTCModule::setup()
{
  // this->Rtc->Begin(21, 22);
  this->Rtc->Enable32kHzPin(false);
  this->Rtc->SetSquareWavePin(DS3231SquareWavePin_ModeNone);
}

void RTCModule::loop()
{
  if (!this->Rtc->IsDateTimeValid())
  {
    if (this->Rtc->LastError() != 0)
    {
      Serial.print("RTC communications error = ");
      Serial.println(this->Rtc->LastError());
    }
    else
    {
      Serial.println("RTC lost confidence in the DateTime!");
    }
  }
  else
  {
    // Serial.println("VALID RTC..");
  }

  _now = this->Rtc->GetDateTime();

  printDateTime(_now);
  Serial.println("-------");

  temperature = this->Rtc->GetTemperature();
  // temp.Print(Serial);
  // you may also get the temperature as a float and print it
  // Serial.print(temperature.AsCentiDegC());
  // Serial.println("C");
}

// String RTCModule::getTimeString()
// {
//   return String("");
// }

RtcDateTime *RTCModule::getCurrentRtcDateTime()
{
  return &this->_now;
}