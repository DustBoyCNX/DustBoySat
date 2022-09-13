#include <CMMC_Interval.h>
#include <Preferences.h>
#include <rom/rtc.h>
#include "../modules/ModePref.hpp"

#ifndef BootPref_HPP
#define BootPref_HPP

class BootPref
{
public:
    void setup()
    {

        if (rtc_get_reset_reason(0) == 1)
        {
            // Serial.println(count());
            if (count() >= 5)
            {
                modePref.setMode(ModePref::MODE_CONFIG);
                clear();
            }
        }
        else if (rtc_get_reset_reason(0) == 5)
        {
        }
        else
        {
            clear();
            modePref.setMode(ModePref::MODE_RUN);
        }

        // Serial.println(rtc_get_reset_reason(0));
    }

    void loop()
    {
        if (!disabled && (millis() > 5 * 1000L))
        {
            modePref.setMode(ModePref::MODE_RUN);
            clear();
            disabled = true;
            Serial.println("RUN MODE");
        }
    }

    uint8_t count()
    {
        bool is_read_only = false;
        preferences.begin("boot_x", is_read_only);
        unsigned int boot_count = preferences.getUInt("boot_count", 0);
        boot_count++;
        Serial.printf("boot count = %u\r\n", boot_count);
        preferences.putUInt("boot_count", boot_count);
        preferences.end();
        return boot_count;
    }

    void clear()
    {
        preferences.begin("boot_x", false);
        preferences.putUInt("boot_count", 0);
        preferences.end();
    }

private:
    bool disabled = false;
    Preferences preferences;
};
BootPref bootPref;
#endif