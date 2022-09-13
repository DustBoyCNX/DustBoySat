#include <Arduino.h>
#include "../RTCModule.h"
#include "../LCD_Module.h"
#include "../DustModule.h"
#include "../modules/BootPref.hpp"
#include "../modules/ModePref.hpp"

#include <Ticker.h>
#include <rom/rtc.h>
#include "../utils.h"
#include "../NTP.hpp"
#include <Button2.h>
static Ticker btnscanT;

const uint8_t APP_MAJOR = 1;
const uint8_t APP_MINOR = 8;

// static Button2 *pBtns = nullptr;
static uint8_t g_btns[] = {0};

// ESP.deepSleep(1);
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <Button2.h>
#include <Ticker.h>

// extern DustModule *dustModule;
LCD_Module *lcd;
class App
{
public:
    // RTCModule *rtcModule;
    void attachCb() {
        // for (int i = 0; i < sizeof(g_btns) / sizeof(g_btns[0]); ++i)
        // {
        //     pBtns[i].loop();
        // }
    }
    void setup()
    {
        // pinMode(0, INPUT);
        // esp_log_level_set("*", ESP_LOG_VERBOSE); // set all components to ERROR level
        // RESET_REASON cpu0_reset_reason;
        // RESET_REASON cpu1_reset_reason;
        // cpu0_reset_reason = rtc_get_reset_reason(0);
        // cpu1_reset_reason = rtc_get_reset_reason(1);
        // print_reset_reason(rtc_get_reset_reason(0));
        lcd = new LCD_Module();
        lcd->setTime(APP_MAJOR, APP_MINOR, 0);
        lcd->loop();
        // rtcModule = new RTCModule(&Wire);
        // modePref.setup();
        // Serial.printf("Current Mode=%u\r\n", modePref.currentMode());
        // bootPref.setup();
        // Serial.println(ModePref::MODE_CONFIG, ModePref::MODE_RUN);
        // uint8_t args = sizeof(g_btns) / sizeof(g_btns[0]);
        // pBtns = new Button2[args];
        // for (int i = 0; i < args; ++i)
        // {
        //     pBtns[i] = Button2(g_btns[i]);
        //     // pBtns[i].setPressedHandler(button_callback);

        //     pBtns[i].setTripleClickHandler([&](Button2 &b) -> void {
        //         Serial.printf("%u Click \r\n", b.getAttachPin());
        //         if (b.getAttachPin() == 0)
        //         {
        //             modePref.setMode(ModePref::MODE_CONFIG);
        //             ESP.deepSleep(1);
        //         }
        //     });
        // }

        // void attachCb(void) {

        // }

        // btnscanT.attach_ms(20, this->attachCb);
        // create ticker callback function like above



        // while (modePref.currentMode() == ModePref::MODE_CONFIG)
        // {
        //     modePref.setMode(ModePref::MODE_RUN);
        //     bootPref.clear();
        //     lcd->setup();
        //     NTP ntpModule;
        //     Serial.println("in config mode..");
        //     WiFi.begin("CMMC_3BB_2.4G", "zxc12345");
        //     while (WiFi.status() != WL_CONNECTED)
        //     {
        //         Serial.println("Connecting WiFi...");
        //         delay(200);
        //     }
        //     Serial.println("WiFi Connected..");
        //     ntpModule.setup();
        //     while (true)
        //     {
        //         ntpModule.loop();
        //         if (ntpModule.isSynced)
        //         {
        //             RtcDateTime dt = RtcDateTime(ntpModule.timeinfo.tm_year, ntpModule.timeinfo.tm_mon + 1, ntpModule.timeinfo.tm_mday, ntpModule.timeinfo.tm_hour, ntpModule.timeinfo.tm_min, ntpModule.timeinfo.tm_sec);
        //             rtcModule->Rtc->SetDateTime(dt);
        //             WiFiClient client;

        //             // The line below is optional. It can be used to blink the LED on the board during flashing
        //             // The LED will be on during download of one buffer of data from the network. The LED will
        //             // be off during writing that buffer to flash
        //             // On a good connection the LED should flash regularly. On a bad connection the LED will be
        //             // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
        //             // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
        //             // httpUpdate.setLedPin(LED_BUILTIN, LOW);

        //             t_httpUpdate_return ret = httpUpdate.update(client, "http://128.199.104.122/offline.bin");
        //             // Or:
        //             //t_httpUpdate_return ret = httpUpdate.update(client, "server", 80, "file.bin");
        //             switch (ret)
        //             {
        //             case HTTP_UPDATE_FAILED:
        //                 Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
        //                 // ESP.deepSleep(10e5);
        //                 break;

        //             case HTTP_UPDATE_NO_UPDATES:
        //                 Serial.println("HTTP_UPDATE_NO_UPDATES");
        //                 // ESP.deepSleep(10e5);
        //                 break;

        //             case HTTP_UPDATE_OK:
        //                 Serial.println("HTTP_UPDATE_OK");
        //                 Serial.println("HTTP_UPDATE_OK");
        //                 Serial.println("HTTP_UPDATE_OK");
        //                 // ESP.deepSleep(10e5);
        //                 break;
        //             default:
        //                 Serial.println("Fallback error.");
        //                 Serial.println(ret);
        //                 break;
        //             }
        //             ESP.deepSleep(1);
        //         }
        //     }
        // }
        // rtcModule->setup();
    }

    // void loop_0()
    // {
    //     bootPref.loop();
    //     rtcModule->loop();
    // };

    // void loop_1()
    // {
    //     RtcDateTime *now = rtcModule->getCurrentRtcDateTime();
    //     lcd->setTime(now->Hour(), now->Minute(), now->Second());
    //     if (digitalRead(0) == LOW)
    //     {
    //         lcd->setTime(APP_MAJOR, APP_MINOR, 0);
    //     }
    //     else
    //     {
    //         lcd->setTime(now->Hour(), now->Minute(), now->Second());
    //     }
    //     // lcd->setDustValue(dustModule->_pm2_5, dustModule->_pm10);
    //     // lcd->setTime(now->Hour(), now->Minute(), now->Second());
    // }

    void loop_2()
    {
        lcd->loop();
    }

    void loop()
    {
        // loop_0();
        // loop_1();
        loop_2();
    }
};