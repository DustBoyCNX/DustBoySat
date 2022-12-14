#ifndef LCD_Module_H
#define LCD_Module_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <TimeLib.h>
#include <Arduino.h>
#include <CMMC_Interval.h>

class LCD_Module
{
  uint8_t page = 0;

protected:
  void paintRunPage();

public:
  U8G2_ST7920_128X64_F_SW_SPI *u8g2;
  LCD_Module();
  void setTime(uint8_t hh, uint8_t mm, uint8_t ss);
  void setDustValue(uint16_t, uint16_t);
  void setup();
  void loop();
  void printConnectWiFi();
  void setTitle(const char*title) {
    strcpy(this->_title, title);
  }

  void setLabel(const char*label) {
    strcpy(this->_label, label);
  }

  void setDisplayValue(uint16_t pmValue) {
    this->_pmValue = pmValue;
  }

  void wipe() {
    this->setDisplayValue(0);
    this->setLabel(" ");
    this->setTitle(" ");
    this->loop();
    this->u8g2->clearDisplay();
  }

private:
  uint8_t hh = 0;
  uint8_t mm = 0;
  uint8_t ss = 0;
  uint16_t _pm10 = 0;
  uint16_t _pm2_5 = 0;
  CMMC_Interval ti;
  char _title[20] = "";
  char _label[20] = "";
  uint16_t _pmValue = 0;
};

#endif
