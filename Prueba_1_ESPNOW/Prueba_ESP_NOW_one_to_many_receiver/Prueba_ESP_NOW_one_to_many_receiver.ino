/*Programa para prueba de ESPNOW, este programa es para el nodo que recibirá información enviada por un esclavo*/
#include <esp_now.h> //Librería ESPNOW
#include <WiFi.h>

//Estructura de datos donde se almacenará los datos recibidos. 
typedef struct test_struct {
  int x;
  int y;
} test_struct;

test_struct myData; //nombrando la estructura "myData"

/*NOTA: no es necesario utilizar la estructura myData, se puede usar una única variable, hay que modificar la variable en el "memcpy" de "OnDataRecv"
en lugar de usar la estructura myData, usar la variable deseada*/
//callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));  //Guardando los daros recibidos en la estructura "myData"
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("x: ");
  Serial.println(myData.x);//imprimiendo valores de X de la estructura
  Serial.print("y: ");
  Serial.println(myData.y);//imprimiendo valores de Y de la estructura
  Serial.println();
}
 



void setup() {
  Serial.begin(115200); 
  WiFi.mode(WIFI_STA);//ESP en modo estación wifi
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv); //Registro para abilitar la recepción de datos
}

void loop() {
  // put your main code here, to run repeatedly:

}
