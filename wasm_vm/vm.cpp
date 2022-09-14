/*
 * Wasm3 - high performance WebAssembly interpreter written in C.
 * Copyright © 2020 Volodymyr Shymanskyy, Steven Massey.
 * All rights reserved.
 */

#include <Arduino.h>
#include <wasm3.h>
#include <Arduino.h>
#include <PMS.h>
#include "./app/App.hpp"
#include "IoTeX-blockchain-client.h"
#include "secrets.h"

PMS pms(Serial1);
PMS::DATA data;
App app;
#include <m3_env.h>

/*
 * Configuration
 */

// Redefine the default LED pin here, if needed
#ifndef LED_PIN
#define LED_PIN             LED_BUILTIN
#endif

#define WASM_STACK_SLOTS    2048
#define NATIVE_STACK_SIZE   (32*1024)

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
    m3ApiReturnType (uint32_t)

    m3ApiReturn(millis());
}

m3ApiRawFunction(m3_arduino_delay)
{
    m3ApiGetArg     (uint32_t, ms)

    // You can also trace API calls
    //Serial.print("api: delay "); Serial.println(ms);

    delay(ms);

    m3ApiSuccess();
}

// This maps pin modes from arduino_wasm_api.h
// to actual platform-specific values
uint8_t mapPinMode(uint8_t mode)
{
    switch(mode) {
    case 0: return INPUT;
    case 1: return OUTPUT;
    case 2: return INPUT_PULLUP;
    }
    return INPUT;
}

m3ApiRawFunction(m3_arduino_pinMode)
{
    m3ApiGetArg     (uint32_t, pin)
    m3ApiGetArg     (uint32_t, mode)

#if !defined(PARTICLE)
    typedef uint8_t PinMode;
#endif
    pinMode(pin, (PinMode)mapPinMode(mode));

    m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_digitalWrite)
{
    m3ApiGetArg     (uint32_t, pin)
    m3ApiGetArg     (uint32_t, value)

    digitalWrite(pin, value);

    m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_getPinLED)
{
    m3ApiReturnType (uint32_t)

    m3ApiReturn(LED_PIN);
}

// m3ApiRawFunction(m3_arduino_getSensorValues)
// {
//     m3ApiReturnType (uint32_t)

//     m3ApiReturn(A0);
// }

m3ApiRawFunction(m3_arduino_getPm2_5)
{
    m3ApiReturnType (uint32_t)
    // Serial.printf("host: get pm2.5 value: %d\r\n", data.PM_AE_UG_2_5);
    m3ApiReturn(data.PM_AE_UG_2_5);
}

m3ApiRawFunction(m3_arduino_print)
{
    m3ApiGetArgMem  (const uint8_t *, buf)
    m3ApiGetArg     (uint32_t,        len)

    //printf("api: print %p %d\n", buf, len);
    Serial.write(buf, len);

    m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_printInt)
{
    m3ApiGetArg(int32_t, out);
    Serial.print(out);

    m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_print_int)
{
  m3ApiGetArg(int32_t, out);

  Serial.print(out);

  m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_getGreeting)
{
    m3ApiGetArgMem  (uint8_t *,    out)
    m3ApiGetArg     (uint32_t,     out_len)

    const char buff[] = "Hello WASM world! 😊";
    memcpy(out, buff, min(sizeof(buff), out_len));

    m3ApiSuccess();
}

m3ApiRawFunction (m3_arduino_rawSerialOut) {
    m3ApiGetArg     (uint32_t,     value)

    Serial.println(value);

    m3ApiSuccess();
}


m3ApiRawFunction (m3_display_setTitle) {
    m3ApiGetArgMem  (const uint8_t *, buf)
    m3ApiGetArg     (uint32_t,        len)
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

m3ApiRawFunction (m3_display_setDisplayValue) {
    m3ApiGetArg (uint16_t, value);
    app.lcd->setDisplayValue(value);
    m3ApiSuccess();
}

    // define macro for this
    // m3_LinkRawFunction (module, "wiring", "printInt",            "v(i)",  &m3_arduino_printInt);

#define LINK(module, namespace, name, signature, rawFunction) \
    m3_LinkRawFunction(module, namespace, name, signature, rawFunction)

    


M3Result  LinkArduino  (IM3Runtime runtime)
{
    IM3Module module = runtime->modules;
    // const char* arduino = "arduino";

    LINK(module, "wiring", "millis",           "i()",    &m3_arduino_millis);
    LINK(module, "wiring", "delay",            "v(i)",   &m3_arduino_delay);
    LINK(module, "wiring", "pinMode",          "v(ii)",  &m3_arduino_pinMode);
    LINK(module, "wiring", "digitalWrite",     "v(ii)",  &m3_arduino_digitalWrite);

    // Test functions
    LINK(module, "wiring", "getPinLED",        "i()",    &m3_arduino_getPinLED);
    LINK(module, "wiring", "getPm2_5",        "i()",    &m3_arduino_getPm2_5);
    LINK(module, "wiring", "getPm10",        "i()",    &m3_arduino_getPinLED);
    LINK(module, "wiring", "getPm1",        "i()",    &m3_arduino_getPinLED);
    LINK(module, "wiring", "getGreeting",      "v(*i)",  &m3_arduino_getGreeting);
    LINK(module, "wiring", "rawSerialOut", "v(i)", &m3_arduino_rawSerialOut);
    LINK(module, "wiring", "print",            "v(*i)",  &m3_arduino_print);
    LINK(module, "wiring", "printInt", "v(i)", &m3_arduino_print_int);

    m3_LinkRawFunction(module, "serial", "printInt", "v(i)", &m3_arduino_print_int);

    LINK(module, "display", "setTitle", "v(*i)", &m3_display_setTitle);
    LINK(module, "display", "setDisplayValue", "v(i)", &m3_display_setDisplayValue);



    return m3Err_none;
}

/*
 * Engine start, liftoff!
 */

#define FATAL(func, msg) { Serial.print("Fatal: " func " "); Serial.println(msg); return; }

void wasm_task(void*)
{
    M3Result result = m3Err_none;

    IM3Environment env = m3_NewEnvironment ();
    if (!env) FATAL("NewEnvironment", "failed");

    IM3Runtime runtime = m3_NewRuntime (env, WASM_STACK_SLOTS, NULL);
    if (!runtime) FATAL("NewRuntime", "failed");

#ifdef WASM_MEMORY_LIMIT
    runtime->memoryLimit = WASM_MEMORY_LIMIT;
#endif

    IM3Module module;
    result = m3_ParseModule (env, &module, app_wasm, app_wasm_len);
    if (result) FATAL("ParseModule", result);

    result = m3_LoadModule (runtime, module);
    if (result) FATAL("LoadModule", result);

    result = LinkArduino (runtime);
    if (result) FATAL("LinkArduino", result);


    IM3Function ff;
    result = m3_FindFunction (&ff, runtime, "_callback");
    if (result) FATAL("FindFunction", result);
    Serial.println("Running callback...");

    result = m3_CallV (ff);
    if (result) {
        M3ErrorInfo info;
        m3_GetErrorInfo (runtime, &info);
        Serial.print("Error: ");
        Serial.print(result);
        Serial.print(" (");
        Serial.print(info.message);
        Serial.println(")");
        if (info.file && strlen(info.file) && info.line) {
            Serial.print("At ");
            Serial.print(info.file);
            Serial.print(":");
            Serial.println(info.line);
        }
    }
    


    IM3Function f;
    result = m3_FindFunction (&f, runtime, "_start");
    if (result) FATAL("FindFunction", result);

    Serial.println("Running WebAssembly...");

    result = m3_CallV (f);

    // Should not arrive here

    if (result) {
        M3ErrorInfo info;
        m3_GetErrorInfo (runtime, &info);
        Serial.print("Error: ");
        Serial.print(result);
        Serial.print(" (");
        Serial.print(info.message);
        Serial.println(")");
        if (info.file && strlen(info.file) && info.line) {
            Serial.print("At ");
            Serial.print(info.file);
            Serial.print(":");
            Serial.println(info.line);
        }
    }



}



// // void setup()
// // {
// //   Serial.begin(115200);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
// //   // Serial1.begin(9600);  // GPIO2 (D4 pin on ESP-12E Development Board)
// //   Serial1.begin(9600, SERIAL_8N1, 32, 33);
// //   pms.passiveMode();    // Switch to passive mode
// //   pms.wakeUp();
// //   delay(1000);
// // }

// // void loop()
// // {
// //   pms.requestRead();
// //   if (pms.readUntil(data))
// //   {
// //     Serial.print("PM 1.0 (ug/m3): ");
// //     Serial.println(data.PM_AE_UG_1_0);

// //     Serial.print("PM 2.5 (ug/m3): ");
// //     Serial.println(data.PM_AE_UG_2_5);

// //     Serial.print("PM 10.0 (ug/m3): ");
// //     Serial.println(data.PM_AE_UG_10_0);
// //   }
// //   else
// //   {
// //     Serial.println("No data.");
// //   }

// //   Serial.println("Going to sleep for 60 seconds.");
// //   pms.sleep();
// //   delay(100);
// // }

void setupPms() {
    Serial1.begin(9600, SERIAL_8N1, 32, 33);
    pms.passiveMode();    // Switch to passive mode
    pms.wakeUp();
    delay(1000);
    // Serial.println("PMS setup done");
    
}


void setup()
{
    Serial.begin(115200);
    setupPms();

    app.setup();
    app.lcd->setTitle("DustBoy");
    app.lcd->setLabel("PM 10");
    // app.lcd->setDisplayValue

    // Wait for serial port to connect
    // Needed for native USB port only
    while(!Serial) {}

    Serial.println("\nWasm3 v" M3_VERSION " (" M3_ARCH "), build " __DATE__ " " __TIME__);

#ifdef ESP32
    // On ESP32, we can launch in a separate thread
    xTaskCreate(&wasm_task, "wasm3", NATIVE_STACK_SIZE, NULL, 5, NULL);
#else
    wasm_task(NULL);
#endif
}

void loop()
{
  app.loop();
  pms.requestRead();
  if (pms.readUntil(data))
  {
    app.lcd->setDustValue(data.PM_AE_UG_2_5, data.PM_AE_UG_10_0);
    app.lcd->setDisplayValue(data.PM_AE_UG_10_0);
    // Serial.println("read data done");
    // Serial.print("PM 1.0 (ug/m3): ");
    // Serial.println(data.PM_AE_UG_1_0);

    // Serial.print("PM 2.5 (ug/m3): ");
    // Serial.println(data.PM_AE_UG_2_5);

    // Serial.print("PM 10.0 (ug/m3): ");
    // Serial.println(data.PM_AE_UG_10_0);
  }
  else
  {
    Serial.println("No data.");
  }

//   Serial.println("Going to sleep for 60 seconds.");
  pms.sleep();
  delay(100);
}
