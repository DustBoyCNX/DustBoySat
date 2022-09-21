#include <Arduino.h>
#include <wasm3.h>
#include <Arduino.h>
#include <PMS.h>
#include "./app/App.hpp"
#include "IoTeX-blockchain-client.h"
#include "secrets.h"

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <sstream>

#define BRIDGE_WASM_VM 1

PMS pms(Serial1);
PMS::DATA _pmsData;
App app;

int scanTime = 5; // In seconds
BLEScan *pBLEScan;
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        //  Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());

        if (advertisedDevice.haveName() && advertisedDevice.haveServiceData() && advertisedDevice.getName().find("ATC") != std::string::npos)
        {
            Serial.printf("Advertised Name: %s\n", advertisedDevice.getName().c_str());
            // Serial.printf("Advertised Name: %s\n", advertisedDevice.getName().c_str());
            std::string strServiceData = advertisedDevice.getServiceData();
            uint8_t cServiceData[100];
            char charServiceData[100];

            strServiceData.copy((char *)cServiceData, strServiceData.length(), 0);

            /*
              A4 C1 38 BE CA FD # 0 - 5
                                01 2C # temp 6 - 7
                                      47 # humid 8
                                         50 # batt % 9
                                            0B 72 # batt 10 - 11
                                                   66 # counter
            */

            if (true || cServiceData[6] > 0x00)
            {
                float temp;
                int humi;
                float battV;
                int battP;
                int counter;

                temp = (cServiceData[6] << 8 | (cServiceData[7])) * 0.1; // little endian
                humi = cServiceData[8];
                battV = (cServiceData[10] << 8 | (cServiceData[11])) * 0.001;
                battP = cServiceData[9];
                counter = cServiceData[12];

                Serial.print(" > ");
                for (int i = 0; i < strServiceData.length(); i++)
                {
                    sprintf(&charServiceData[i * 2], "%02X", cServiceData[i]);
                    Serial.printf("%02X ", cServiceData[i]);
                }

                Serial.println();
                Serial.printf(" > temp=%.2f °C, humid=%d%%, batt=%d%%, batt=%.2fV, counter=%d\r\n", temp, humi, battP, battV, counter);
                Serial.println("-------------------------------------------------");
            }
            else
            {
                Serial.println("No data");
            }

            std::stringstream ss;
            //        ss << "fe95" << charServiceData;
            ss << charServiceData;
        }
    }
};

// DustModule dustModule(&Serial1);

#include <m3_env.h>

/*
 * Configuration
 */

// Redefine the default LED pin here, if needed
#ifndef LED_PIN
#define LED_PIN LED_BUILTIN
#endif

#define WASM_STACK_SLOTS 2048
#define NATIVE_STACK_SIZE (32 * 1024)

// For (most) devices that cannot allocate a 64KiB wasm page
//#define WASM_MEMORY_LIMIT   4096

/*
 * WebAssembly app
 */

#include "../wasm_apps/rust/app.wasm.h"
//#include "../wasm_apps/rust/app.wasm.h"
//#include "../wasm_apps/tinygo/app.wasm.h"
//#include "../wasm_apps/cpp/app.wasm.h"

/*
 * API bindings
 *
 * Note: each RawFunction should complete with one of these calls:
 *   m3ApiReturn(val)   - Returns a value
 *   m3ApiSuccess()     - Returns void (and no traps)
 *   m3ApiTrap(trap)    - Returns a trap
 */

m3ApiRawFunction(m3_arduino_millis)
{
    m3ApiReturnType(uint32_t)

        m3ApiReturn(millis());
}

m3ApiRawFunction(m3_arduino_delay)
{
    m3ApiGetArg(uint32_t, ms)

        // You can also trace API calls
        // Serial.print("api: delay "); Serial.println(ms);

        delay(ms);

    m3ApiSuccess();
}

// This maps pin modes from arduino_wasm_api.h
// to actual platform-specific values
uint8_t mapPinMode(uint8_t mode)
{
    switch (mode)
    {
    case 0:
        return INPUT;
    case 1:
        return OUTPUT;
    case 2:
        return INPUT_PULLUP;
    }
    return INPUT;
}

m3ApiRawFunction(m3_arduino_pinMode)
{
    m3ApiGetArg(uint32_t, pin)
        m3ApiGetArg(uint32_t, mode)

#if !defined(PARTICLE)
            typedef uint8_t PinMode;
#endif
    pinMode(pin, (PinMode)mapPinMode(mode));

    m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_digitalWrite)
{
    m3ApiGetArg(uint32_t, pin)
        m3ApiGetArg(uint32_t, value)

            digitalWrite(pin, value);

    m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_getPinLED)
{
    m3ApiReturnType(uint32_t)

        m3ApiReturn(LED_PIN);
}

// m3ApiRawFunction(m3_arduino_getSensorValues)
// {
//     m3ApiReturnType (uint32_t)

//     m3ApiReturn(A0);
// }

m3ApiRawFunction(m3_arduino_getPm10)
{
    m3ApiReturnType(uint32_t)
        m3ApiReturn(_pmsData.PM_AE_UG_10_0);
}

m3ApiRawFunction(m3_arduino_getPm2_5)
{
    m3ApiReturnType(uint32_t)
        m3ApiReturn(_pmsData.PM_AE_UG_2_5);
}

m3ApiRawFunction(m3_arduino_getPm1)
{
    m3ApiReturnType(uint32_t)
        m3ApiReturn(_pmsData.PM_AE_UG_1_0);
}

m3ApiRawFunction(m3_arduino_print)
{
    m3ApiGetArgMem(const uint8_t *, buf)
        m3ApiGetArg(uint32_t, len)

        // printf("api: print %p %d\n", buf, len);
        Serial.write(buf, len);
    m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_printInt)
{
    m3ApiGetArg(int32_t, out);
    Serial.print(out);
    m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_getGreeting)
{
    m3ApiGetArgMem(uint8_t *, out)
        m3ApiGetArg(uint32_t, out_len)

            const char buff[] = "Hello WASM world! 😊";
    memcpy(out, buff, min(sizeof(buff), out_len));

    m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_rawSerialOut)
{
    m3ApiGetArg(uint32_t, value)

        Serial.println(value);

    m3ApiSuccess();
}

m3ApiRawFunction(m3_display_setTitle)
{
    m3ApiGetArgMem(const uint8_t *, buf)
        m3ApiGetArg(uint32_t, len)
        // for (int i = 0; i < len; i++) {
        //     Serial.print((char)buf[i]);
        // }

        // char title[32];
        // // memcpy(title, buf, min(sizeof(title), len));
        // strncpy(title, (char*)buf, min(sizeof(title), len));

        char title[len + 1];
    memcpy(title, buf, len);
    title[len] = '\0';

    app.lcd->setTitle(title);

    m3ApiSuccess();
}

m3ApiRawFunction(m3_display_setDisplayValue)
{
    m3ApiGetArg(uint16_t, value);
    app.lcd->setDisplayValue(value);
    m3ApiSuccess();
}

// define macro for this
// m3_LinkRawFunction (module, "wiring", "printInt",            "v(i)",  &m3_arduino_printInt);

#define LINK(module, namespace, name, signature, rawFunction) \
    m3_LinkRawFunction(module, namespace, name, signature, rawFunction)

M3Result LinkArduino(IM3Runtime runtime)
{
    IM3Module module = runtime->modules;
    LINK(module, "wiring", "millis", "i()", &m3_arduino_millis);
    LINK(module, "wiring", "delay", "v(i)", &m3_arduino_delay);
    LINK(module, "wiring", "pinMode", "v(ii)", &m3_arduino_pinMode);
    LINK(module, "wiring", "digitalWrite", "v(ii)", &m3_arduino_digitalWrite);

    LINK(module, "wiring", "getPinLED", "i()", &m3_arduino_getPinLED);
    LINK(module, "wiring", "getPm10", "i()", &m3_arduino_getPm10);
    LINK(module, "wiring", "getPm2_5", "i()", &m3_arduino_getPm2_5);
    LINK(module, "wiring", "getPm1", "i()", &m3_arduino_getPm1);
    LINK(module, "wiring", "getGreeting", "v(*i)", &m3_arduino_getGreeting);
    LINK(module, "wiring", "rawSerialOut", "v(i)", &m3_arduino_rawSerialOut);
    LINK(module, "wiring", "print", "v(*i)", &m3_arduino_print);
    LINK(module, "wiring", "printInt", "v(i)", &m3_arduino_printInt);

    m3_LinkRawFunction(module, "serial", "printInt", "v(i)", &m3_arduino_printInt);

    LINK(module, "display", "setTitle", "v(*i)", &m3_display_setTitle);
    LINK(module, "display", "setDisplayValue", "v(i)", &m3_display_setDisplayValue);

    return m3Err_none;
}

/*
 * Engine start, liftoff!
 */

#define FATAL(func, msg)                  \
    {                                     \
        Serial.print("Fatal: " func " "); \
        Serial.println(msg);              \
        return;                           \
    }

void wasm_task(void *)
{
    M3Result result = m3Err_none;

    IM3Environment env = m3_NewEnvironment();
    if (!env)
        FATAL("NewEnvironment", "failed");

    IM3Runtime runtime = m3_NewRuntime(env, WASM_STACK_SLOTS, NULL);
    if (!runtime)
        FATAL("NewRuntime", "failed");

#ifdef WASM_MEMORY_LIMIT
    runtime->memoryLimit = WASM_MEMORY_LIMIT;
#endif

    IM3Module module;
    result = m3_ParseModule(env, &module, app_wasm, app_wasm_len);
    if (result)
        FATAL("ParseModule", result);

    result = m3_LoadModule(runtime, module);
    if (result)
        FATAL("LoadModule", result);

    result = LinkArduino(runtime);
    if (result)
        FATAL("LinkArduino", result);

    IM3Function ff;
    result = m3_FindFunction(&ff, runtime, "_callback");
    if (result)
        FATAL("FindFunction", result);
    Serial.println("Running callback...");

    result = m3_CallV(ff);
    if (result)
    {
        M3ErrorInfo info;
        m3_GetErrorInfo(runtime, &info);
        Serial.print("Error: ");
        Serial.print(result);
        Serial.print(" (");
        Serial.print(info.message);
        Serial.println(")");
        if (info.file && strlen(info.file) && info.line)
        {
            Serial.print("At ");
            Serial.print(info.file);
            Serial.print(":");
            Serial.println(info.line);
        }
    }

    IM3Function f;
    result = m3_FindFunction(&f, runtime, "_start");
    if (result)
        FATAL("FindFunction", result);

    Serial.println("Running WebAssembly...");

    result = m3_CallV(f);

    // Should not arrive here

    if (result)
    {
        M3ErrorInfo info;
        m3_GetErrorInfo(runtime, &info);
        Serial.print("Error: ");
        Serial.print(result);
        Serial.print(" (");
        Serial.print(info.message);
        Serial.println(")");
        if (info.file && strlen(info.file) && info.line)
        {
            Serial.print("At ");
            Serial.print(info.file);
            Serial.print(":");
            Serial.println(info.line);
        }
    }
}

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

void setupBle()
{
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); // create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); // active scan uses more power, but get results faster

    //  pBLEScan->setInterval(0x50);
    //  pBLEScan->setWindow(0x30);

    pBLEScan->setInterval(1000);
    pBLEScan->setWindow(1000); // less or equal setInterval value
}

void setup()
{
    Serial.begin(9600, SERIAL_8N1);
    setupPms();
    // setupBle();

    app.setup();
    app.lcd->setTitle("RustBoy!");
    app.lcd->setLabel("PM 2.5");
    // app.lcd->setDisplayValue

    // Wait for serial port to connect
    // Needed for native USB port onl
    // while(!Serial) {}

    Serial.println("\nWasm3 v" M3_VERSION " (" M3_ARCH "), build " __DATE__ " " __TIME__);

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
            if (buffer[0] == 's')
            {
                Serial.printf("sleep time = %d seconds\r\n", buffer[1]);
                app.lcd->wipe();
                delay(10);
                esp_sleep_enable_timer_wakeup(buffer[1] * 1000000);
                esp_deep_sleep_start();
            }
            else if (buffer[0] == 'm')
            {
                if (buffer[1] == 'A')
                {
                    Serial.println("mode ASCII");
                    mode = OUTPUT_MODE_ASCII;
                }
                else if (buffer[1] == 'B')
                {
                    mode = OUTPUT_MODE_BINARY;
                }
                else if (buffer[1] == 'M')
                {
                    isMockup = true;
                }
                else if (buffer[1] == 'N')
                {
                    isMockup = false;
                }
            }
        }
        strcpy(buffer, "");
    }

    //   Serial.println("Going to sleep for 60 seconds.");
    //   pms.sleep();
    delay(1000);
    //   BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
    //  Serial.print("Devices found: ");
    //  Serial.println(foundDevices.getCount());
    //  Serial.println("Scan done!");

    //   delay(1000);
    //   pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
}
