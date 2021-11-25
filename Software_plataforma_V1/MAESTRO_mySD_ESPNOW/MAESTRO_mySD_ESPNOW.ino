/*
  Programa inicial del prototipo de la plataforma 1-Maestro 1-Esclavo
  MAESTRO


  Acciones:
  Enviar comandos a esclavos por Serial
        1-> Empezar la adquisición de datos
        2-> Cerrar archivo **IMPORTANTE** Siempre enviar el comando una vez que se terminaron de recibir los datos por parte del esclavo ó no se guardará el archivo
        3-> Solicitar los datos del esclavo


 Elementos del programa:
 Comunicación ESPNOW 
 Lectura y dataloggin de ADC (mySD)
 
*/

#include <esp_now.h> //libreria espNOW
#include <WiFi.h>  //librería wifi
#include <SPI.h> //Librería protocolo SPI
#include <mySD.h> //Librería uso de SD

String dataMessage;
File myFile;

// REMPLAZAR CON LA DIRECCION MAC del RECEPTOR
uint8_t broadcastAddress1[] = {0x08, 0x3A, 0xF2, 0xAC, 0x57, 0xDC}; ///1
//uint8_t broadcastAddress2[] = {0x84, 0xCC, 0xA8, 0x47, 0x4D, 0xF8}; ///2
//uint8_t broadcastAddress3[] = {0x3C, 0x61, 0x05, 0x11, 0xE4, 0x90}; ///3


uint8_t xx;//datos de sen
String l;
int t;


// Definir variables que almacenaran los datos a enviar a los esclavos
int modo; // 1(recopilación datos), 3(enviando datos), 
unsigned int m=0;

// Variable to store if sending data was successful
String success;




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
  //uint8_t xx;
  memcpy(&xx,incomingData,sizeof(incomingData));
  //Serial.write(xx);
  l=(char)xx;
  digitalWrite(15,HIGH);
  myFile.print(l);//Guardando en el archivo la información recibida
  
  //appendFile(SD, "/data1.txt", dataMessage.c_str()); 
  //if(m=4) appendFile(SD, "/data2.txt", dataMessage.c_str());
  //if(m=5) appendFile(SD, "/data3.txt", dataMessage.c_str());
  /*memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  //t++;
  dataMessage = String(incomingReadings.x)+ "\r\n";
  appendFile(SD, "/data.txt", dataMessage.c_str());
    Serial.println(incomingReadings.x);*/
}






void setup() {
  // Setear comunicación serial a 115200 baudios
  Serial.begin(115200);
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
  
  // Registrando el par 1
  esp_now_peer_info_t peerInfo;
  
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Añadiendo el par  1   
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);    
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 1");
    return;
  }


  


 /* // Registrando el par 3
  //esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress3, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;*/
  
 /* // Añadiendo el par  3  
  memcpy(peerInfo.peer_addr, broadcastAddress3, 6);     
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer 3");
    return;
  }*/


  
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

void loop() {
  digitalWrite(15,LOW);
  //modo=0;
  getReadings();//revisar serial para enviar comandos
  
  
  //delay(10);
  /*esp_now_send(broadcastAddress3, (uint8_t *) &modo, sizeof(modo));//enviando comandos a 3
  delay(10);*/
  //if(modo=='2') muestreo();
}



void getReadings(){
  if(Serial.available()> 0){
  modo=Serial.read();
  }
  if(modo=='1') { //Comando a esclavo para iniciar el dataloggin
    modo=1;
    esp_now_send(broadcastAddress1, (uint8_t *) &modo, sizeof(modo));//enviando comandos a esclavo
    modo=0;
    m=0;
  }
  else{
    if(modo=='2') { //Cerrar el archivo del Maestro ***IMPORTANTE siempre enviar el comando para guardar el archivo
    modo=2;
    myFile.close();
    esp_now_send(broadcastAddress1, (uint8_t *) &modo, sizeof(modo));//enviando comandos a esclavo
    modo=0;
    m=0;
    
    }
    else{
      
      if(modo=='3'){
        modo=3;
        SD.remove("test.txt");
        myFile = SD.open("test.txt", FILE_WRITE);
        esp_now_send(broadcastAddress1, (uint8_t *) &modo, sizeof(modo));//enviando comandos a 1
        modo=0;
        m=0;
        
      }
          
        
      }
    }
  }
