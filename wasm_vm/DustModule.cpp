#include "DustModule.h"
// extern LCDModule* lcdModule;

DustModule::DustModule(HardwareSerial *s)
{
  _serial = s;
  // pms = new PMS(*s);
}

void DustModule::isLongPressed()
{
}

void DustModule::setup()
{
  // pms->activeMode();
  // pms->wakeUp();
  delay(500);

  /*
  data.PM_SP_UG_1_0 = 0;
  data.PM_SP_UG_2_5 = 0;
  data.PM_SP_UG_10_0 = 0;
  data.PM_AE_UG_1_0 = 0;
  data.PM_AE_UG_2_5 = 0;
  data.PM_AE_UG_10_0 = 0;
  */

  // _pm2_5 = _pm2_5_bucket.add(0.0);
  // _pm10 = _pm10_bucket.add(0.0);

  Serial.println("---------------");
  Serial.print("pm2.5> ");
  Serial.println(_pm2_5);
  Serial.print("pm10> ");
  Serial.println(_pm2_5);
  Serial.println("---------------");
}

struct pms5003data
{
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

struct pms5003data dataxx;

void DustModule::loop()
{
  int ct = 0;
  while (this->_serial->peek() != 0x42)
  {
    this->_serial->read();
    delay(10);
    if (ct++ > 500)
    {
      Serial.println("dust sensor not found.");
      ct = 0;
      return;
    }
  }

  if (this->_serial->available() < 32)
  {
    // SERIAL0.println(".......");
    return;
  }

  uint8_t buffer[32];
  uint16_t sum = 0;
  this->_serial->readBytes(buffer, 32);
  // get checksum ready
  for (uint8_t i = 0; i < 30; i++)
  {
    sum += buffer[i];
  }

  // for (uint8_t i=2; i<32; i++) {
  //   SERIAL0.print("0x"); SERIAL0.print(buffer[i], HEX); SERIAL0.print(", ");
  // }
  // SERIAL0.println();

  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i = 0; i < 15; i++)
  {
    buffer_u16[i] = buffer[2 + i * 2 + 1];
    buffer_u16[i] += (buffer[2 + i * 2] << 8);
  }

  // put it into a nice struct :)
  memcpy((void *)&dataxx, (void *)buffer_u16, 30);
  if (sum != dataxx.checksum)
  {
    Serial.println("Checksum FAILED!");
  }
  else
  {
    // Serial.println("CHECKSUM OK.");
    if ((counter++ % 2) != 0)
    {
      return;
    }
    _pm1 = dataxx.pm10_standard;
    _pm2_5 = dataxx.pm25_standard;
    _pm10 = dataxx.pm100_standard;

    if (_pm2_5 > 999)
    {
      _pm2_5 = 999;
    }

    if (_pm10 > 999)
    {
      _pm10 = 999;
    }

    if (_pm1 > 999)
    {
      _pm1 = 999;
      _pm1 = 999;
    }

    // Serial.print("PM 10.0 (ug/m3): ");
    // Serial.println(dataxx.pm100_standard);
    // Serial.print("PM 2.5 (ug/m3): ");
    // Serial.println(dataxx.pm25_standard);
    // Serial.print("PM 1 (ug/m3): ");
    // Serial.println(dataxx.pm10_standard);
  }
}
