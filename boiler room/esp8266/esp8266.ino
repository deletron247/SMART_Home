//==== Подключаем библиотеки ===============================================
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
//==========================================================================
//
//==== Настраиваем подключение к Wifi ======================================
const char* ssid="1975_Network";
const char* password="1122334455";
//==========================================================================
String Website,Td1,Td2,Td3,Td4,Td5,Td6,Tmd,Tpd,XML,Javascript;

int T1,T2,T3,T4,T5,T6,N1,N2,Tmn,Tpn = 0;

//---# Настройки для NRF #-----------

RF24 radio(4, 15); // "создать" модуль на пинах 9 и 10 Для Уно

byte recieved_data[10]; // массив принятых данных

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
    
    Javascript+="xmldoc = xmlResponse.getElementsByTagName('T1');\n";
    Javascript+="message = xmldoc[0].firstChild.nodeValue;\n";
    Javascript+="document.getElementById('T1').innerHTML=message;\n";
    
    Javascript+="xmldoc = xmlResponse.getElementsByTagName('T4');\n";
    Javascript+="message = xmldoc[0].firstChild.nodeValue;\n";
    Javascript+="document.getElementById('T2').innerHTML=message;\n";

    Javascript+="xmldoc = xmlResponse.getElementsByTagName('T2');\n";
    Javascript+="message = xmldoc[0].firstChild.nodeValue;\n";
    Javascript+="document.getElementById('T3').innerHTML=message;\n";
    
    Javascript+="xmldoc = xmlResponse.getElementsByTagName('T5');\n";
    Javascript+="message = xmldoc[0].firstChild.nodeValue;\n";
    Javascript+="document.getElementById('T4').innerHTML=message;\n";

    Javascript+="xmldoc = xmlResponse.getElementsByTagName('T3');\n";
    Javascript+="message = xmldoc[0].firstChild.nodeValue;\n";
    Javascript+="document.getElementById('T5').innerHTML=message;\n";
    
    Javascript+="xmldoc = xmlResponse.getElementsByTagName('T6');\n";
    Javascript+="message = xmldoc[0].firstChild.nodeValue;\n";
    Javascript+="document.getElementById('T6').innerHTML=message;\n";
   
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
    Website+="<h2 class=\"ui icon header center aligned\"> <i class=\"settings icon\"></i> <div class=\"content\">Панель управления<div class=\"sub header\"></div> </div> </h2>";
   
    Website+="<div class=\"ui divider\"></div><div class=\"item\"> <div class=\"middle aligned content\"> <div class=\"header\">Бойлер</div> <div class=\"description\">";
    Website+="<div id='T1'>"+Td1+"</div>";
    Website+="<div id='T2'>"+Td4+"</div></div>";

    Website+="<div class=\"ui divider\"></div><div class=\"header\">Котёл</div> <div class=\"description\">";
    Website+="<div id='T3'>"+Td2+"</div>";
    Website+="<div id='T4'>"+Td5+"</div></div>";

    Website+="<div class=\"ui divider\"></div><div class=\"header\">Солнечный колектор</div> <div class=\"description\">";
    Website+="<div id='T5'>"+Td3+"</div>";
    Website+="<div id='T6'>"+Td6+"</div></div>";

    Website+="<div class=\"ui divider\"></div><div class=\"header\">Насосы</div> <div class=\"description\">";
    if (N1 == 0) {Website+="<h3>Насос №1<img height=\"30px\" style=\"margin-left:10px; vertical-align:middle\" src=\"https://raw.githubusercontent.com/deletron247/SMART_Home/master/website/2.png\" alt=\"image\"><h3>"; }else{ Website+="<h3>Насос №1<img height=\"30px\" style=\"margin-left:10px; vertical-align:middle\" src=\"https://raw.githubusercontent.com/deletron247/SMART_Home/master/website/1.png\" alt=\"image\"><h3>";}
    if (N2 == 0) {Website+="<h3>Насос №2<img height=\"30px\" style=\"margin-left:10px; vertical-align:middle\" src=\"https://raw.githubusercontent.com/deletron247/SMART_Home/master/website/2.png\" alt=\"image\"><h3></div>"; }else{ Website+="<h3>Насос №2<img  height=\"30px\" style=\"margin-left:10px; vertical-align:middle\" src=\"https://raw.githubusercontent.com/deletron247/SMART_Home/master/website/1.png\" alt=\"image\"><h3></div>";}

    Website+="<div class=\"ui divider\"></div><div class=\"header\">Настройка</div> <div class=\"description\">";
    Website+="<form method=\"GET\" action=\"con\">";
    Website+="<p><h4>Макс.t бойлера<input type=\"text\" name=\"Tm\" style=\"margin-left:10px\" value="+Tmd+" size=\"2\">°C</h4></p>";
    Website+="<p><h4>Термо порог<input type=\"text\" name=\"Tp\" style=\"margin-left:10px\" value="+Tpd+" size=\"2\">°C</h4></p>";
    Website+="<p><button type=\"submit\">Отправить</button></p>";
    Website+="</form>";
    Website+="</div>";
    
    Website+="</body></html>";
    Website+=Javascript;
    server.send(200,"text/html",Website);
  }

void XMLcontent(){

  XML ="<?xml version='1.0'?>";
  XML+="<data>";
  XML+="<T1>"; //Температура датчика 1
  XML+=Td1;
  XML+="</T1>";
  XML+="<T2>"; //Температура датчика 2
  XML+=Td2;
  XML+="</T2>";
  XML+="<T3>"; //Температура датчика 3
  XML+=Td3;
  XML+="</T3>";
  XML+="<T4>"; //Температура датчика 4
  XML+=Td4;
  XML+="</T4>";
  XML+="<T5>"; //Температура датчика 5
  XML+=Td5;
  XML+="</T5>";
  XML+="<T6>"; //Температура датчика 6
  XML+=Td6;
  XML+="</T6>";
  XML+="</data>";

  server.send(200,"text/xml",XML);
  
  }  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)delay(500);
  WiFi.mode(WIFI_STA);
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
  server.on("/con", []() {   
  Tmn = server.arg("Tm").toInt();
  Tpn = server.arg("Tp").toInt();
  server.send(200, "text/html", Website);
  });
}

void loop() {
//---# Начинаем слушать эфир NRF и присваиваем переменные #---------
  byte pipeNo;
  if ( radio.available(&pipeNo)) {  // слушаем эфир со всех труб
    radio.read( &recieved_data, sizeof(recieved_data) );         // читем входящий сигнал
  }
  int T1 = recieved_data[0];
  int T2 = recieved_data[1];
  int T3 = recieved_data[2];
  int T4 = recieved_data[3];
  int T5 = recieved_data[4];
  int T6 = recieved_data[5];
  int Tm = recieved_data[6];
  int Tp = recieved_data[7];
  N1 = recieved_data[8];
  N2 = recieved_data[9];
//------------------------------------------------------------------
    
  //Преобразуем переменные для вывода на сайт
  //Показания температуры
  Td1=(String)"Верх "+T1+" °C";
  Td2=(String)"Верх "+T2+" °C";
  Td3=(String)"Верх "+T3+" °C";

  Td4=(String)"Низ "+T4+" °C";
  Td5=(String)"Низ "+T5+" °C";
  Td6=(String)"Низ "+T6+" °C";
  //Показания макс темп. и темп порога
  Tmd=(String)Tm; 
  Tpd=(String)Tp;
  
  server.handleClient();
  if ((Tmn!=Tm)&&(Tmn>0)){
    Tm=Tmn;
  }
}
    
