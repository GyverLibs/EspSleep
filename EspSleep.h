#pragma once
#include <Arduino.h>
#include <rtc_utils.h>

#ifdef ESP8266

// время калибровки, мкс
#ifndef RTC_CALI_BLOCK
#define RTC_CALI_BLOCK 50000
#endif

// макс. блок сна, мкс
#ifndef MAX_SLEEP_BLOCK
#define MAX_SLEEP_BLOCK 2ull * 60 * 60 * 1000 * 1000
#endif

// время сна для включения wifi, мкс
#define ENABLE_RF_SLEEP_PRD 10

class EspSleep {
    struct sleep_data {
        uint64_t left = 0;
        bool enableWiFi = 0;
    };

   public:
    // rtc_offset - смещение для хранения остатка времени, сон занимает 5 ячеек. По умолчанию стоит самая последняя возможная ячейка. instant - режим deepSleepInstant
    EspSleep(uint8_t rtc_offset = 123, bool instant = 0, WakeMode mode = RF_DEFAULT) : _rtc_offset(rtc_offset), _instant(instant), _mode(mode) {}

    // спать (миллисекунды, секунды, минуты, часы, дни)
    void sleep(uint64_t ms, uint32_t sec = 0, uint32_t min = 0, uint16_t hour = 0, uint16_t day = 0) {
        if (sec) ms += sec * 1000ull;
        if (min) ms += min * 60 * 1000ull;
        if (hour) ms += hour * 60 * 60 * 1000ull;
        if (day) ms += day * 24 * 60 * 60 * 1000ull;
        sleep_us(ms * 1000ull);
    }

    // спать, микросекунды
    void sleep_us(uint64_t us) {
        sleep_data data;
        if (us > MAX_SLEEP_BLOCK) {
            data.left = us - MAX_SLEEP_BLOCK;
            us = MAX_SLEEP_BLOCK;
        }
        if (_mode != RF_DISABLED) data.enableWiFi = 1;
        rtc_write(&data, _rtc_offset);

        if (RTC_CALI_BLOCK > 0) {
            uint32_t rtc = get_rtc();
            delayMicroseconds(RTC_CALI_BLOCK);
            rtc = get_rtc() - rtc;
            us = us * rtc / (RTC_CALI_BLOCK + 1);   // warn
        }
        _instant ? ESP.deepSleepInstant(us, RF_DISABLED) : ESP.deepSleep(us, RF_DISABLED);
    }

    // тикер блоков сна. Вызывать в начале программы. Вернёт false, если это первый запуск мк и сон не запущен (true, если сон в процессе)
    bool tick() {
        sleep_data data;
        uint8_t res = rtc_read(&data, _rtc_offset);
        if (data.left) sleep_us(data.left);
        else if (data.enableWiFi) {  // проснулись и нужно включить RF
            stop();
            _instant ? ESP.deepSleepInstant(ENABLE_RF_SLEEP_PRD, _mode) : ESP.deepSleep(ENABLE_RF_SLEEP_PRD, _mode);
        }
        _first = (res == RTCU_RESET);
        return !_first;
    }

    // true - первый запуск после сброса питания. ВЫЗЫВАТЬ ПОСЛЕ tick()!
    bool firstStart() {
        return _first;
    }

    // отменить следующий сон в tick
    void stop() {
        sleep_data data;
        rtc_write(&data, _rtc_offset);
    }

   private:
    uint8_t _rtc_offset;
    bool _instant;
    WakeMode _mode;
    bool _first = 0;

    uint32_t get_rtc() {
        return (uint64_t)system_get_rtc_time() * system_rtc_clock_cali_proc() >> 12;
    }
};
#endif