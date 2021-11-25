/*
  Programa inicial del prototipo de la plataforma 1-Maestro 1-Esclavo
  ESCLAVO


  Acciones:
  Empezar la adquisición de datos
  Envío de datos almacenados al maestro


 Elementos del programa:
 Comunicación ESPNOW 
 Lectura y dataloggin de ADC (mySD)
 
*/

#include <esp_now.h> //libreria espNOW
#include <WiFi.h>  //librería wifi
#include <SPI.h>  //Librería SPI
#include <mySD.h>

// REMPLAZAR CON LA DIRECCION MAC del RECEPTOR
uint8_t broadcastAddress[] = {0x08, 0x3A, 0xF2, 0xAC, 0x77, 0x90};

// Definir variables que almacenaran los datos a enviar 
int modo; // 1(recopilación datos), 3(enviando datos), 
// Variable to store if sending data was successful
String success;


unsigned int lectura=0;


// Callback when data is sent ESPNOW
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    //success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}


// Callback when data is received ESPNOW
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&modo, incomingData, sizeof(incomingData));
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  //modo = incomingReadings.m;
}


unsigned int datos;



File myFile;

// change this to match your SD shield or module;
//     Arduino Ethernet shield: pin 4
//     Adafruit SD shields and modules: pin 10
//     Sparkfun SD shield: pin 8
const int chipSelect = 5;

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(115200);
  pinMode(36, INPUT);
  pinMode(15,OUTPUT);

  // Seleccionando ESP32 como Wi-Fi Station
  WiFi.mode(WIFI_STA);


  // Iniciando ESP NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
   // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Registrando el par
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Añadiendo el par        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Registro de llamada cuando información es recivida
  esp_now_register_recv_cb(OnDataRecv);


  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(SS, OUTPUT);
   
  if (!SD.begin(5, 23, 19, 18)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  SD.remove("test.txt");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
  // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  
  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}



void loop()
{
  digitalWrite(15,LOW);
  
  
     if(modo==1){
      SD.remove("test.txt");
      myFile = SD.open("test.txt", FILE_WRITE);   
        for(unsigned int i=0; i<=125000; i++){
          digitalWrite(15,!digitalRead(15));
          datos=analogRead(36); 
          //Serial.println(datos);      
          myFile.println(datos);
          Serial.println(datos);
          analogRead(39);
          analogRead(34);
          analogRead(35);
          analogRead(32);
          analogRead(33);
          //delayMicroseconds(100);
          
          //myFile.flush();
          
        }
        myFile.close();
        modo=0;
      }


  if(modo==3){
    digitalWrite(15,HIGH);
    myFile = SD.open("test.txt");
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      lectura=(myFile.read());
      esp_now_send(broadcastAddress, (uint8_t *) &lectura, sizeof(lectura));//enviando comandos
      //delay(1);
      delayMicroseconds(1200);
    }
    // close the file:
    myFile.close();
    modo=0;
  }
      
  
}
