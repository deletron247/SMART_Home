# SMART_Home

- Программа для слежения за температурой в бойлере, котле и солнечном конденсаторе.
Если температура источника тепла выше температуры бойлера то мы включаем насос для нагрева бойлера.
При температуре выше Tm мы включаем все насосы для охлаждения теплоносителя  

## Installation
1) Скачиваем [Arduino IDE](https://www.arduino.cc/en/main/software) с оф. сайта.

2) Устанавливаем и после установки идем в C:\Program Files\Arduino\libraries и распаковываем архивы взяты от сюда [ТЫК](https://github.com/deletron247/SMART_Home/tree/master/libraries).

3) Cкачиваем [драйвер](https://iarduino.ru/lib/ch340.zip) для програматора установленого на Arduino и Wemos D1 R1 

4) Подключаем все датчики (к D2) и загружаем через arduino ide прошивку [Search_ROM_Sensor.ino](https://github.com/deletron247/SMART_Home/blob/master/boiler%20room/arduino/Search_ROM_Sensor.ino) на  arduino nano выбрав ее в инструментах

5) Комбинацией клавиш Ctrl+Shift+m открываем консоль и видим:

![](https://github.com/deletron247/SMART_Home/blob/master/image/1.PNG)

Это наши датчики температуры бойлера, солнечного колектора и котла.

6)Открываем код [arduino.ino](https://github.com/deletron247/SMART_Home/blob/master/boiler%20room/arduino/arduino.ino) и прописываем адреса полученые в пункте 2 в:
```C++
//---# Адреса датчиков температуры #-------------------------------------------------------------------------
DeviceAddress sensor1 = { 0x28, 0xFF, 0x13, 0xFA, 0x0, 0x17, 0x5, 0xAD }; // Адрес первого сенсора
DeviceAddress sensor2 = { 0x28, 0x2, 0x0, 0x7, 0x46, 0x5A, 0x1, 0xD8 }; // Адрес второго сенсора
DeviceAddress sensor3 = { 0x28, 0x2, 0x0, 0x7, 0xB3, 0x1F, 0x1, 0x4 }; // Адрес третего сенсора
DeviceAddress sensor4 = { 0x28, 0x7, 0x0, 0x7, 0x68, 0xC2, 0x1, 0xE6 }; // Адрес четвертого сенсора
DeviceAddress sensor5 = { 0x28, 0xFF, 0xD6, 0xB, 0x1, 0x17, 0x5, 0x8 }; // Адрес пятого сенсора
DeviceAddress sensor6 = { 0x28, 0xFF, 0x59, 0xE7, 0x0, 0x17, 0x5, 0x5C }; // Адрес шестого сенсора
//-----------------------------------------------------------------------------------------------------------
```
7)Настройки переменных которые используются по умолчанию, температуру можно изменить через web панель
```C++
//---# Глобальные переменные #-------------------------------------------------------------------------------
int Tm = 95; // Температура бойлера при которой включаются все насосы для охлаждения
int N1 = 3;  // Пин подключения реле первого насоса 
int N2 = 4;  // Пин подключения реле второго насоса
int Tp = 5;  // На сколько должна быть больше температура конденсатора или котла чтобы включился один из насосов
//-----------------------------------------------------------------------------------------------------------
```
## Scheme

Arduino

![](https://github.com/deletron247/SMART_Home/blob/master/image/scheme1.png)

ESP8266

![](https://github.com/deletron247/SMART_Home/blob/master/image/scheme2.png)

## WEB panel

![](https://github.com/deletron247/SMART_Home/blob/master/image/webpanel.png)
