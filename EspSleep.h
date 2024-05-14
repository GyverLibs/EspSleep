#pragma once
#include <Arduino.h>
#include <rtc_utils.h>

#ifdef ESP8266

class EspSleep {
   public:
    // rtc_offset - смещение для хранения остатка времени, сон занимает 3 ячейки (12 байт). По умолчанию стоит самая последняя возможная ячейка. instant - режим deepSleepInstant
    EspSleep(uint8_t rtc_offset = 125, bool instant = 0, WakeMode mode = RF_DEFAULT) : _rtc_offset(rtc_offset), _instant(instant), _mode(mode) {}

    // спать (миллисекунды, секунды, минуты, часы, дни)
    void sleep(uint64_t ms, uint64_t sec = 0, uint32_t min = 0, uint16_t hour = 0, uint16_t day = 0) {
        if (sec) ms += sec * 1000ull;
        if (min) ms += min * 60 * 1000ull;
        if (hour) ms += hour * 60 * 60 * 1000ull;
        if (day) ms += day * 24 * 60 * 60 * 1000ull;
        sleep_us(ms * 1000ull);
    }

    // спать, микросекунды
    void sleep_us(uint64_t us) {
        uint64_t max = ESP.deepSleepMax();
        if (us > max) {
            us -= max;
            rtc_write(&us, _rtc_offset);
            us = max;
        } else {
            max = 0;
            rtc_write(&max, _rtc_offset);
        }
        _instant ? ESP.deepSleepInstant(us, _mode) : ESP.deepSleep(us, _mode);
    }

    // тикер блоков сна. Вызывать в начале программы. Вернёт false, если это первый запуск мк и сон не запущен (true, если сон в процессе)
    bool tick() {
        uint64_t left;
        uint8_t res = rtc_read(&left, _rtc_offset);
        if (left) sleep_us(left);
        return (res == RTCU_RESET) ? false : true;
    }

    // отменить следующий сон в tick
    void stop() {
        uint64_t left = 0;
        rtc_write(&left, _rtc_offset);
    }

    // размер смещения (в 4-Б блоках, не в байтах!)
    size_t rtc_size() {
        return 3;
    }

   private:
    uint8_t _rtc_offset;
    bool _instant;
    WakeMode _mode;
};
#endif
