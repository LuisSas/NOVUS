//Programa de Arduino IDE para la obtencion de la dirección MAC del esp32

#include "WiFi.h" //Librería para uso de WIFI
 
void setup(){
  Serial.begin(115200); //Puerto Serial a 115200 boudios
  WiFi.mode(WIFI_MODE_STA);  //ESP en modo de estación wifi
  Serial.println(WiFi.macAddress()); //Imprimir dirección MAC
}
 
void loop(){

}
