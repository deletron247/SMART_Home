#include "OneWire.h"
 
OneWire ds(2);  // Пин подключения датчиков температуры
 
void setup(void) {
  Serial.begin(9600);
}
 
void loop(void) {
  byte i;
  byte addr[8];
 
  if (!ds.search(addr)) {
    Serial.println(" No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  Serial.print(" ROM =");
  for (i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }
}
