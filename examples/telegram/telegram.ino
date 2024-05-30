#include <Arduino.h>

#define WIFI_SSID ""
#define WIFI_PASS ""
#define BOT_TOKEN ""
#define CHAT_ID ""

#include <ESP8266WiFi.h>

// #define RTC_CALI_BLOCK 0
#include <EspSleep.h>
EspSleep sleep;

#include <FastBot2.h>
FastBot2 bot;

void setup() {
    sleep.tick();

    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("\nConnected");
    bot.setToken(F(BOT_TOKEN));

    if (sleep.firstStart()) {
        bot.sendMessage(fb::Message("first start", CHAT_ID));
    } else {
        bot.sendMessage(fb::Message("awake", CHAT_ID));
    }
    sleep.sleep(0, 0, 0, 6);
}

void loop() {
}