[![latest](https://img.shields.io/github/v/release/GyverLibs/EspSleep.svg?color=brightgreen)](https://github.com/GyverLibs/EspSleep/releases/latest/download/EspSleep.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/EspSleep.svg)](https://registry.platformio.org/libraries/gyverlibs/EspSleep)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/EspSleep?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# EspSleep
Библиотека позволяет esp8266 уходить в сон на любой период (до ~585 миллиардов лет)

### Совместимость
esp8266

### Зависимости
- [rtc_utils](https://github.com/GyverLibs/rtc_utils)

## Содержание
- [Использование](#usage)
- [Версии](#versions)
- [Установка](#install)
- [Баги и обратная связь](#feedback)

<a id="usage"></a>

## Использование
### Инициализация
```cpp
EspSleep(uint8_t rtc_offset = 0, bool instant = 0, WakeMode mode = RF_DEFAULT);
```

### Описание класса
```cpp
// спать (миллисекунды, секунды, минуты, часы, дни)
void sleep(uint64_t ms, uint64_t sec = 0, uint32_t min = 0, uint16_t hour = 0, uint16_t day = 0);

// спать, микросекунды
void sleep_us(uint64_t us);

// тикер блоков сна. Вызывать в начале программы. 
// Вернёт false, если это первый запуск мк и сон не запущен (true, если сон в процессе)
bool tick();

// отменить следующий сон в tick
void stop();

// размер смещения (в 4-Б блоках, не в байтах!)
size_t rtc_size();
```

### Как это работает
> Для пробуждения по таймеру GPIO16 должен быть подключен к RST!

> Остальная информация по сну [в документации](https://arduino-esp8266.readthedocs.io/en/latest/libraries.html#esp-specific-apis)

esp8266 аппаратно не может спать дольше, чем `ESP.deepSleepMax()` микросекунд (~3.2 часа). Поэтому сон дробится на периоды, равные максимальному времени сна, и оставшееся время сна записывается в RTC память (оперативная память, не очищающаяся при перезагрузке). Для сна в таком режиме с этой библиотекой нужно использовать конструкцию вида:

```cpp
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
    sleep.sleep(5000);
}

void loop() {
    // уйти в сон можно где угодно
    // if (foo) sleep.sleep(5000);
}
```

<a id="versions"></a>

## Версии
- v1.0

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **EspSleep** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/EspSleep/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!

<a id="feedback"></a>

## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код