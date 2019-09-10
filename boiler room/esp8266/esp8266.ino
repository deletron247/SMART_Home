//********БИБЛИОТЕКИ**************
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
//********************************

const char* ssid="Название WIFI";
const char* password="пароль от WIFI";

String Website,Td1,Td2,Td3,Td4,Td5,Td6,XML,Javascript;

int T1 = 0;
int T2 = 0;
int T3 = 0;
int T4 = 0;
int T5 = 0;
int T6 = 0;

//---# Настройки для NRF #-----------

RF24 radio(4, 15); // "создать" модуль на пинах 9 и 10 Для Уно

byte recieved_data[6]; // массив принятых данных

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

ESP8266WebServer server(80); // 80 is the port number

void javascriptContent(){
    Javascript ="<SCRIPT>\n";
    Javascript+="var xmlHttp=createXmlHttpObject();\n";
    Javascript+="function createXmlHttpObject(){\n";
    Javascript+="if(window.XMLHttpRequest){\n";
    Javascript+="xmlHttp=new XMLHttpRequest();\n";
    Javascript+="}else{\n";
    Javascript+="xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
    Javascript+="}\n";
    Javascript+="return xmlHttp;\n";
    Javascript+="}\n";
    Javascript+="\n";
    Javascript+="function response(){\n";
    Javascript+="xmlResponse=xmlHttp.responseXML;\n";
    Javascript+="xmldoc = xmlResponse.getElementsByTagName('data');\n";
    Javascript+="message = xmldoc[0].firstChild.nodeValue;\n";
    Javascript+="document.getElementById('div1').innerHTML=message;\n";
    Javascript+="}\n";

    Javascript+="function process(){\n";
    Javascript+="xmlHttp.open('PUT','xml',true);\n";
    Javascript+="xmlHttp.onreadystatechange=response;\n";
    Javascript+="xmlHttp.send(null);\n";
    Javascript+="setTimeout('process()',200);\n";
    Javascript+="}\n";
    
    Javascript+="</SCRIPT>\n";
    
  
  }



void WebsiteContent(){
    javascriptContent();
    
    Website="<!DOCTYPE html><html>\n";
    Website+="<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" charset=\"utf-8\">\n";
    Website+="<title>Smart Home</title>\n";
    Website+="<link rel=\"shortcut icon\" href=\"https://i.ibb.co/4WQtWLr/favicon.png\" type=\"image/x-icon\">\n";
    Website+="<link rel=\"stylesheet\" type=\"text/css\" href=\"https://cdnjs.cloudflare.com/ajax/libs/semantic-ui/2.4.1/semantic.min.css\">\n";
    Website+="<style>body { background-color: #1a202b; }; i.icon {font-size: 1.3em;padding-right: 1.5em;}\n";
    Website+=".item {font-size: 1.05em;}</style>\n";
    Website+="<body onload='process()'> <div class=\"ui fixed inverted menu borderless\"> <div class=\"ui container\"> <a class=\"item\" href=\"https://github.com/deletron247/SMART_Home\" target=\"_blank\">GitHub</a></div></div>";
    Website+="<div class=\"ui main text container raised segment\" style=\"margin-top: 5em; margin-bottom: 5em\"> <div class=\"ui items\" style=\"padding: 2em; margin-top: 0\">";
    Website+="<h2 class=\"ui icon header center aligned\"> <i class=\"settings icon\"></i> <div class=\"content\">Панель управления<div class=\"sub header\"></div> </div> </h2> <div class=\"ui divider\"></div>";
    Website+="<div class=\"item\"> <div class=\"middle aligned content\"> <div class=\"header\">Бойлер</div> <div class=\"description\">";
    
    Website+="<div id='div1'>"+Td1+"</div></body></html>";
    Website+=Javascript;
    server.send(200,"text/html",Website);
  }

void XMLcontent(){

  XML ="<?xml version='1.0'?>";
  XML+="<data>";
  XML+=Td1;
  XML+="</data>";

  server.send(200,"text/xml",XML);
  
  }  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)delay(500);
  WiFi.mode(WIFI_STA);
  Serial.print(WiFi.localIP());
  server.on("/",WebsiteContent);
  server.on("/xml",XMLcontent);
  server.begin();
  IPAddress ip(192,168,0,200);   
  IPAddress gateway(192,168,0,254);   
  IPAddress subnet(255,255,255,0);   
  WiFi.config(ip, gateway, subnet);

//---# Инициализируем NRF #---------------
radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openReadingPipe(1, address[0]);     //хотим слушать трубу 0
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.startListening();  //начинаем слушать эфир, мы приёмный модуль
//------------------------------------------

}

void loop() {
//---# Начинаем слушать эфир NRF и присваиваем переменные #---------
  byte pipeNo;
  while ( radio.available(&pipeNo)) {  // слушаем эфир со всех труб
    radio.read( &recieved_data, sizeof(recieved_data) );         // чиатем входящий сигнал
  }
  int T1 = recieved_data[0];
  int T2 = recieved_data[1];
  int T3 = recieved_data[2];
  int T4 = recieved_data[3];
  int T5 = recieved_data[4];
  int T6 = recieved_data[5];
//------------------------------------------------------------------
    
  // put your main code here, to run repeatedly:
  Td1=(String)"Верх "+T1+" °C";
  Td2=(String)"Верх "+T2+" °C";
  Td3=(String)"Верх "+T3+" °C";

  Td4=(String)"Низ "+T4+" °C";
  Td5=(String)"Низ "+T5+" °C";
  Td6=(String)"Низ "+T6+" °C";
  server.handleClient();


}
    
