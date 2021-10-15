/*Programa para prueba de ESPNOW, este programa es para el nodo que enviará información al maestro*/
#include <esp_now.h> //librería ESP NOW
#include <WiFi.h> //Librería uso WiFi

uint8_t broadcastAddress[] = {0x84, 0xCC, 0xA8, 0x47, 0x4D, 0xF8}; //Registro del la dirreción MAC del receptor


//Estructura de datos donde se almacenará los datos a enviar. 
typedef struct test_struct {
  int x;
  int y;
} test_struct;

test_struct test; //nombrando la estructura "test"


// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",mac_addr[0]); 
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}



void setup() {
  
  Serial.begin(115200);
 
  WiFi.mode(WIFI_STA);//esp en modo estación
 
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent); //Registrando el envío de datos
  
   // registrando el canal
  esp_now_peer_info_t peerInfo;
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // registrando el par 
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  
}

void loop() {
  //Generando datos random en X e Y de la estructura "test"
  test.x = random(0,20); 
  test.y = random(0,20);
  esp_err_t result = esp_now_send(0, (uint8_t *) &test, sizeof(test_struct)); //Enviando los datos de "test" al maestro
  delay(2000);
}
