#include <EspSleep.h>
EspSleep sleep;

void setup() {
    // тут просыпаемся
    // можно отменить оставшийся сон через sleep.stop()

    // ...

    // засыпаем, если осталось время сна
    // при первой подаче питания на мк ничего не происходит
    sleep.tick();


    // сюда попадаем, когда время сна полностью вышло!
    // А также при подаче питания на мк
    Serial.begin(115200);
    Serial.println();
    Serial.println("awake!");

    // засыпаем например тут
    sleep.sleep_sec(10);
}

void loop() {
    // уйти в сон можно где угодно
    // if (foo) sleep.sleep_sec(10);
}