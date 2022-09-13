#include "LCD_Module.h"
// #include "SDCard_Module.hpp"
// extern SDCard_Module sdcardModule;
uint16_t th_PM25AQI(uint16_t val)
{
  uint16_t out;
  if (val <= 25)
  {
    out = map(val, 0, 25, 0, 25);
    out = 1;
  }
  else if (val > 25 && val <= 37)
  {
    out = map(val, 26, 37, 26, 50);
    out = 2;
  }
  else if (val > 37 && val <= 50)
  {
    out = map(val, 38, 50, 51, 100);
    out = 3;
  }
  else if (val > 50 && val <= 90)
  {
    out = map(val, 51, 90, 101, 200);
    out = 4;
  }
  else if (val > 90)
  {
    out = val - 90 + 200;
    out = 5;
  }
  out -= 1;
  return out;
}
LCD_Module::LCD_Module()
{
  u8g2 = new U8G2_ST7920_128X64_F_SW_SPI(U8G2_R0, /* clock=*/18, /* MOSI=*/23, /* MISO=*/19);
  u8g2->begin();
  String taskMessage = "[LCD_Module] Task running on core ";
  taskMessage = taskMessage + xPortGetCoreID();
}

void LCD_Module::setTime(uint8_t hh, uint8_t mm, uint8_t ss)
{
  this->hh = hh;
  this->mm = mm;
  this->ss = ss;
}

void LCD_Module::setDustValue(uint16_t pm2_5, uint16_t pm10)
{
  _pm10 = pm10;
  _pm2_5 = pm2_5;
}

void LCD_Module::setup()
{
}

void LCD_Module::loop()
{
  static LCD_Module *that = this;
  // ti.every_ms(1, []() {
  // });
  that->paintRunPage();
}

int counter = 0;

void LCD_Module::printConnectWiFi()
{
  u8g2->clearBuffer();
  u8g2->firstPage();

  do
  {
    u8g2->println("Connecting WiFi..");
  } while (u8g2->nextPage());
}
void LCD_Module::paintRunPage()
{
  // char m_str[3];
  // strcpy(m_str, u8x8_u8toa(m, 2)); /* convert m to a string with two digits */

  u8g2->clearBuffer();
  u8g2->firstPage();

  do
  {
    counter++;
    // u8g2->drawXBM(90, -2, 32, 16, a_bits);
    int pad = 10;
    // _pm2_5 = 888;
    if (_pm2_5 >= 100 || _pm10 >= 100)
    {
      pad = 0;
    }
    static char buffer[15] = {0};
    sprintf(buffer, "%02d:%02d", hh, mm);
    u8g2->setFont(u8g2_font_p01type_tf);
    u8g2->setCursor(5, 7);
    u8g2->print(buffer);

    // https://github.com/olikraus/u8g2/wiki/fntgrpunifont
    /*sdcardModule.is_no_sdcard*/
    if (false)
    {
      u8g2->setFont(u8g2_font_unifont_t_symbols);
      u8g2->drawGlyph(30, 9, 9749);
    }

    u8g2->setFont(u8g2_font_tenthinguys_tf);
    u8g2->drawStr(65, 10, "DustBoy");
    u8g2->drawLine(2, 38, 124, 38);

    u8g2->setCursor(90, 6);
    u8g2->setFont(u8g2_font_u8glib_4_hf);

    u8g2->setFont(u8g2_font_koleeko_tf);
    u8g2->setCursor(2, 25 + 2 + 2);
    u8g2->print("PM2.5");
    u8g2->setCursor(40 + pad, 30 + 2 + 2);
    u8g2->setFont(u8g2_font_luBS19_tn);

    u8g2->print(_pm2_5);
    u8g2->setFont(u8g2_font_siji_t_6x10);
    u8g2->print("ug/m3");

    u8g2->setFont(u8g2_font_battery19_tn);

    int width = 20;
    int padding = 4;
    for (size_t i = 0; i < 5; i++)
    {
      /* code */
      if (i <= th_PM25AQI(_pm2_5))
      {
        u8g2->drawBox(6 + (padding + width) * i, 42, width, 20);
        u8g2->setDrawColor(0);
        u8g2->setFont(u8g2_font_luBS14_tn);
        u8g2->drawStr(10 + (padding + width) * i, 59, String(i + 1).c_str());
        u8g2->setDrawColor(1);
      }
      else
      {
        u8g2->drawFrame(6 + (padding + width) * i, 42, width, 20);
      }
      // u8g2->setColorIndex(0);
      // u8g2->setColorIndex(1);
    }

  } while (u8g2->nextPage());
}