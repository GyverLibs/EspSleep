#pragma once
#include <Arduino.h>
#include <rtc_utils.h>

#ifdef ESP8266

class EspSleep {
   public:
    // rtc_offset - смещение для хранения остатка времени, сон занимает 3 ячейки (12 байт)
    EspSleep(uint8_t rtc_offset = 0, bool instant = 0, WakeMode mode = RF_DEFAULT) : _rtc_offset(rtc_offset), _instant(instant), _mode(mode) {}

    // спать, сутки
    void sleep_day(uint16_t day) {
        sleep_us(day * 24 * 60 * 60 * 1000000ull);
    }

    // спать, часы
    void sleep_hour(uint32_t hour) {
        sleep_us(hour * 60 * 60 * 1000000ull);
    }

    // спать, минуты
    void sleep_min(uint32_t min) {
        sleep_us(min * 60 * 1000000ull);
    }

    // спать, секунды
    void sleep_sec(uint32_t sec) {
        sleep_us(sec * 1000000ull);
    }

    // спать, миллиссекунды
    void sleep_ms(uint64_t ms) {
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