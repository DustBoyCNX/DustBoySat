#include <esp_sleep.h>
#include <esp32-hal.h>
#include <Arduino.h>
#include <wasm3.h>
#include <WiFi.h>
#include <Arduino.h>
#include <PMS.h>
#include "./app/App.hpp"
#include "IoTeX-blockchain-client.h"
#include "secrets.h"

#include <Arduino.h>
#include <sstream>

#define BRIDGE_WASM_VM 1

PMS pms(Serial1);
PMS::DATA _pmsData;
App app;

/*
 * WebAssembly app
 */

#include "vm_binding.hpp"

void setupPms()
{
    Serial1.begin(9600, SERIAL_8N1, 32, 33);
    pms.passiveMode(); // Switch to passive mode
    pms.wakeUp();
    // detect boot from deepsleep
    if (rtc_get_reset_reason(0) == DEEPSLEEP_RESET)
    {
        // Serial.println("Woke from deep sleep");
    }
    else
    {
        // Serial.println("Power on or reset");
    }
    // Serial.println("PMS setup done");
}

void setup()
{
    Serial.begin(9600, SERIAL_8N2);
    WiFi.softAPdisconnect(true);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    setupPms();

    app.setup();
    app.lcd->setTitle("RustBoy!");
    app.lcd->setLabel("PM 2.5");
    // app.lcd->setDisplayValue

    // Wait for serial port to connect
    // Needed for native USB port onl
    // while(!Serial) {}

    // Serial.println("\nWasm3 v" M3_VERSION " (" M3_ARCH "), build " __DATE__ " " __TIME__);

#ifdef ESP32
    // On ESP32, we can launch in a separate thread
    xTaskCreate(&wasm_task, "wasm3", NATIVE_STACK_SIZE, NULL, 5, NULL);
#else
    wasm_task(NULL);
#endif
}

typedef struct __attribute((__packed__))
{
    uint8_t header = 0x68;
    uint8_t counter;
    uint16_t pm10;
    uint16_t pm2_5;
    uint16_t pm1;
    uint8_t tail = 0x66;
} PACKET_T;

PACKET_T packet;
uint8_t counter = 0;
;

// #define OUTPUT_FORMAT_ASCII

enum OUTPUT_MODE
{
    OUTPUT_MODE_ASCII = 0,
    OUTPUT_MODE_BINARY,
    OUTPUT_MODE_MOCKUP
};

uint8_t mode = OUTPUT_MODE_ASCII;
bool isMockup = false;

void loop()
{
    app.loop();
    pms.requestRead();
    if (pms.readUntil(_pmsData))
    {
        app.lcd->setDustValue(_pmsData.PM_AE_UG_2_5, _pmsData.PM_AE_UG_10_0);
        app.lcd->setDisplayValue(_pmsData.PM_AE_UG_10_0);

        packet.header = 0x68;
        packet.counter = counter;
        packet.pm10 = _pmsData.PM_AE_UG_10_0;
        packet.pm2_5 = _pmsData.PM_AE_UG_2_5;
        packet.pm1 = _pmsData.PM_AE_UG_1_0;
        packet.tail = 0x66;
    }

    if (isMockup)
    {
        packet.counter = 1;
        packet.pm10 = 19;
        packet.pm2_5 = 7;
        packet.pm1 = 19;
    }

    if (mode == OUTPUT_MODE_ASCII)
    {
        Serial.printf("h,%d,%d,%d,%d", packet.counter, packet.pm10, packet.pm2_5, packet.pm1);
        Serial.write(0x0a);
    }
    else if (mode == OUTPUT_MODE_BINARY)
    {
        Serial.write((uint8_t *)&packet, sizeof(packet));
    }

    counter++;

    // ` (0x60) = cmd header
    // buffer[0] = (s=sleep) cmd
    // buffer[1] = uint8 byte = sleep time
    // buffer[2] = ! (0x21) = tail

    // Count Number: 1 PM10: 19 PM2.5: 7 PM1: 19

    char buffer[4] = {0};
    if (Serial.read() == '`' /* 0x60 */)
    {
        Serial.readBytes(buffer, 3);
        if (buffer[2] == '!')
        {
            if (buffer[0] == 's') // `s1!
            {
                app.lcd->wipe();
                delay(10);
                if (buffer[1] == '!')
                { // `s!! means sleep 10 minutes
                    Serial.printf("10");
                    esp_sleep_enable_timer_wakeup(1 * 60 * 1000000);
                }
                else
                {
                    Serial.printf("%d", (int)buffer[1]);
                    esp_sleep_enable_timer_wakeup(buffer[1] * 1000000); // sleep in seconds
                }
                esp_deep_sleep_start();
            }
            else if (buffer[0] == 'm')
            {
                if (buffer[1] == 'A') // `mA!
                {
                    Serial.println("mode ASCII");
                    mode = OUTPUT_MODE_ASCII;
                }
                else if (buffer[1] == 'B') // `mB!
                {
                    mode = OUTPUT_MODE_BINARY;
                }
                else if (buffer[1] == 'M') // `mM!
                {
                    isMockup = true;
                }
                else if (buffer[1] == 'N') // `mN!
                {
                    isMockup = false;
                }
            }
        }
        strcpy(buffer, "");
    }
    delay(10 * 1000);
}
