#include <CMMC_Interval.h>
#include <Preferences.h>
#include <rom/rtc.h>

#ifndef ModePref_HPP
#define ModePref_HPP
class ModePref
{
public:
    static const uint8_t MODE_CONFIG = 0;
    static const uint8_t MODE_RUN = 1;
    void setup()
    {
        // preferences.begin("mode_x", false);
    }

    void loop()
    {
    }

    uint8_t currentMode()
    {
        preferences.begin("mode_x", true);
        uint8_t mode = preferences.getUInt("mode", 1);
        preferences.end();
        return mode;
    }

    void setMode(uint8_t mode)
    {
        Serial.printf("set mode = %u\r\n", mode);
        preferences.begin("mode_x", false);
        preferences.putUInt("mode", mode);
        preferences.end();
    }

private:
    Preferences preferences;
};

static ModePref modePref;
#endif