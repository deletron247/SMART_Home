//==== Подключаем библиотеки ===============================================================================
#include <Wire.h>                     // Подключаем библиотеку для работы с шиной I2C
#include <LiquidCrystal_I2C.h>        // Подключаем библиотеку для работы с LCD дисплеем по шине I2C
#include <OneWire.h>                  // Подключаем библтотеку для работы с датчиком температуры
#include <DallasTemperature.h>        // Подключаем библиотеку для работы с датчиком температуры
#include <SPI.h>                      // Подключаем библиотеку для работы с ISP интерфейсом
#include <nRF24L01.h>                 // Подключаем библиотеку для работы с радио модулем
#include <RF24.h>                     // Подключаем библиотеку для управления радио модулем
//==========================================================================================================
//
//==== Настраиваем работу с датчиками ======================================================================
#define ONE_WIRE_BUS 2 // Пин подключения сигналиного провода датчика 
OneWire oneWire(ONE_WIRE_BUS); // Настраиваем интерфейс для работы OneWire
DallasTemperature sensors(&oneWire); // Говорим OneWire что он будет работать с DalasTemperature
//==========================================================================================================
//
//==== Настраиваем радио модуль ============================================================================
RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
byte transmited_data[10]; // массив, хранящий передаваемые данные
byte recieved_data[2];// массив принятых данных
//==========================================================================================================
//
//==== Адреса датчиков температуры =========================================================================
DeviceAddress sensor1 = { 0x28, 0xFF, 0x13, 0xFA, 0x0, 0x17, 0x5, 0xAD }; // Адрес первого сенсора
DeviceAddress sensor2 = { 0x28, 0x2, 0x0, 0x7, 0x46, 0x5A, 0x1, 0xD8 }; // Адрес второго сенсора
DeviceAddress sensor3 = { 0x28, 0x2, 0x0, 0x7, 0xB3, 0x1F, 0x1, 0x4 }; // Адрес третего сенсора
DeviceAddress sensor4 = { 0x28, 0x7, 0x0, 0x7, 0x68, 0xC2, 0x1, 0xE6 }; // Адрес четвертого сенсора
DeviceAddress sensor5 = { 0x28, 0xFF, 0xD6, 0xB, 0x1, 0x17, 0x5, 0x8 }; // Адрес пятого сенсора
DeviceAddress sensor6 = { 0x28, 0xFF, 0x59, 0xE7, 0x0, 0x17, 0x5, 0x5C }; // Адрес шестого сенсора
//==========================================================================================================
//
//==== Глобальные переменные ===============================================================================
int Tm = 95; // Температура бойлера при которой включаются все насосы для охлаждения
int Tp = 15; // На сколько должна быть больше температура конденсатора или котла чтобы включился один из насосов
int N1 = 3; // Пин подключения реле первого насоса 
int N2 = 4; // Пин подключения реле второго насоса
int Ni1 = 0;
int Ni2 = 0; 
//==========================================================================================================

//---# Настройки для дисплея #-------------------------------------------------------------------------------
LiquidCrystal_I2C lcd(0x3F,16,2);     //  Объявляем  объект библиотеки, указывая параметры дисплея (адрес I2C = 0x3F, количество столбцов = 16, количество строк = 2)
                                     
uint8_t symbol[6][8] = {              //  Объявляем массив из собственных символов, каждый символ состоит из 8 байт
        { 0,8,11,4,26,2,0,0 },
        { 0,2,20,14,5,8,0,0 }
        }; 
//==========================================================================================================
//
//==== Функция для отображения врашения первого насоса =====================================================
void N1on(){
  lcd.setCursor(15, 0); 
  lcd.print("\1");
  delay(1000);
  lcd.setCursor(15, 0); 
  lcd.print("\2");
  delay(500);
}
//==========================================================================================================
//
//==== Функция для отображения врашения Второго насоса =====================================================
void N2on(){
  lcd.setCursor(15, 1); 
  lcd.print("\1");
  delay(1000);
  lcd.setCursor(15, 1); 
  lcd.print("\2");
  delay(500);
}
//==========================================================================================================
//
//==== Выполнить при запуске ===============================================================================                      
void setup(){                         
    lcd.init();                       //  Инициируем работу с LCD дисплеем
    lcd.backlight();                  //  Включаем подсветку LCD дисплея
    lcd.createChar(1, symbol[0]);     //  Загружаем 1 символ в ОЗУ дисплея
    lcd.createChar(2, symbol[1]);     //  Загружаем 2 символ в ОЗУ дисплея
    
    sensors.begin();                  // Инициализируем работу термо датчиков

    pinMode(N1, OUTPUT);
    pinMode(N2, OUTPUT);

    radio.begin();                    // Инициализируем работу радио модуля
    radio.setAutoAck(1);              //режим подтверждения приёма, 1 вкл 0 выкл
    radio.setRetries(0, 15);          //(время между попыткой достучаться, число попыток)
    radio.enableAckPayload();         //разрешить отсылку данных в ответ на входящий сигнал
    radio.setPayloadSize(32);         //размер пакета, в байтах

    radio.openWritingPipe(address[0]);//мы - труба 0, открываем канал для передачи данных
    radio.setChannel(0x60);           //выбираем канал (в котором нет шумов!)

    radio.setPALevel (RF24_PA_MAX);   //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
    radio.setDataRate (RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
    //должна быть одинакова на приёмнике и передатчике!
    //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

    radio.powerUp();                  //начать работу
    radio.stopListening();            //не слушаем радиоэфир, мы передатчик
}                                    
//==========================================================================================================
//
//==== Цикл ================================================================================================
void loop(){
  sensors.requestTemperatures();      // Спрашиваем у датчиков температуру 
//==== Внутрение переменные ================================================================================ 
  int T1 = sensors.getTempC(sensor1); // Переменная для вывода температуры 1 датчика 
  int T2 = sensors.getTempC(sensor2); // Переменная для вывода температуры 2 датчика 
  int T3 = sensors.getTempC(sensor3); // Переменная для вывода температуры 3 датчика 
  int T4 = sensors.getTempC(sensor4); // Переменная для вывода температуры 4 датчика
  int T5 = sensors.getTempC(sensor5); // Переменная для вывода температуры 5 датчика
  int T6 = sensors.getTempC(sensor6); // Переменная для вывода температуры 6 датчик
//==========================================================================================================
//
//==== Выводим показания на дисплей ========================================================================
//==== Первая строка =======================================================================================
  lcd.clear();
  lcd.setCursor(0, 0);                // Устанавливаем курсор на 1 символ 1 строки и начинаем выводить данные
  lcd.print("B=");
  lcd.print(T1);
  lcd.print(" K=");
  lcd.print(T2);
  lcd.print(" C=");
  lcd.print(T3 );
//==== Вторая строка ======================================================================================
  lcd.setCursor(0, 1);               // Устанавливаем курсор на 1 символ 2 строки и начинаем выводить данные
  lcd.print("B=");
  lcd.print(T4);
  lcd.print(" K=");
  lcd.print(T5);
  lcd.print(" C=");
  lcd.print(T6 );
//=========================================================================================================
//
//==== Логика включения насосов ===========================================================================
  if (T1>Tm){
    digitalWrite(N1, LOW); 
    digitalWrite(N2, LOW);
    N1on(); // Выводим функцию для значка вращения первого насоса
    N2on(); // Выводим функцию для значка вращения второго насоса
    Ni1 = 1;
    Ni2 = 1;
  }
  else{
    if (T2>T4+Tp){
      digitalWrite(N1, LOW);
      N1on(); // Выводим функцию для значка вращения первого насоса
      Ni1 = 1;
    }
    else{
      digitalWrite(N1, HIGH);
      lcd.setCursor(15, 0); 
      lcd.print("\1");
      Ni1 = 0;
    }
    if (T3>T4+Tp){
      digitalWrite(N2, LOW);
      N2on(); // Выводим функцию для значка вращения второго насоса
      Ni2 = 1;
    }
    else{
      digitalWrite(N2, HIGH);
      lcd.setCursor(15, 1); 
      lcd.print("\1");
      Ni2 = 0;
    }
   }
  // Формируем пакет для передачи на ESP
  transmit_data[0] = T1;
  transmit_data[1] = T2;
  transmit_data[2] = T3;
  transmit_data[3] = T4;
  transmit_data[4] = T5;
  transmit_data[5] = T6;
  transmit_data[6] = Tm;
  transmit_data[7] = Tp;
  transmit_data[8] = Ni1;
  transmit_data[9] = Ni2;

  if (radio.write(&transmited_data, sizeof(transmited_data))) {    // отправка пакета transmited_data
     if (!radio.available()) {                                  // если получаем пустой ответ
      } else {
       while (radio.available() ) {                    // если в ответе что-то есть
         radio.read(&recieved_data, sizeof(recieved_data));    // читаем
       }
     }
  }

  if (recieved_data[0] > 0){
    Tm = recieved_data[0];
  }

  if (recieved_data[1] > 0){
    Tp = recieved_data[1];
  }
}
//=========================================================================================================
