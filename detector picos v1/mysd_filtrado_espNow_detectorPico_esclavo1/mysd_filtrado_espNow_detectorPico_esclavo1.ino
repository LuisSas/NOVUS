#include <esp_now.h> //libreria espNOW
#include <WiFi.h>  //librería wifi
#include <SPI.h>
#include <mySD.h>
#include <Ewma.h>

// REMPLAZAR CON LA DIRECCION MAC del RECEPTOR
uint8_t broadcastAddress[] = {0x08, 0x3A, 0xF2, 0xAC, 0x77, 0x90};

// Definir variables que almacenaran los datos a enviar 
int modo; // 1(recopilación datos), 3(enviando datos), 
// Variable to store if sending data was successful
String success;


unsigned int lectura=0; //variable donde estará los valores al leer el archivo 

String l;
uint8_t b;
unsigned int x_0=0;
unsigned int x_1=0;
unsigned int maximum=0;
unsigned int minimum=5000; 
boolean S=false;
unsigned int pico=0;
float norm=0;

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



 


//Ewma adcFilter1(0.1);   // Less smoothing - faster to detect changes, but more prone to noise
Ewma adcFilter2(0.008);  // More smoothing - less prone to noise, but slower to detect changes
//float filtered1=0;
int filtered2=0;


unsigned int datos;



File myFile;
File myFile2;
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
        for(unsigned int i=0; i<=15000; i++){
          digitalWrite(15,!digitalRead(15));
          analogRead(33);
          datos=analogRead(36); 
          //filtered1 = adcFilter1.filter(datos);
          filtered2 = adcFilter2.filter(datos);
          //Serial.println(datos);      
          myFile.println(filtered2);
          Serial.println(filtered2);
          analogRead(39);
          analogRead(34);
          analogRead(35);
          analogRead(32);
          
          delayMicroseconds(300);
          
          //myFile.flush();
          
        }
        myFile.close();
        modo=0;
      }


  if(modo==3){
    SD.remove("test_nom.txt");
    unsigned int i=1;
    digitalWrite(15,HIGH);
    myFile = SD.open("test.txt");
    // read from the file until there's nothing else in it:
    pico=0;
    //minimum=5000;
    while (myFile.available()) {
      
      b=(myFile.read());
      //if(i<5) 
      l += (char)b;
      //Serial.println(l);
      //esp_now_send(broadcastAddress, (uint8_t *) &lectura, sizeof(lectura));//enviando comandos
      
      if(i==6){
        x_1=l.toInt();
        l="";
        i=0;   
        if(x_1>maximum) maximum=x_1;
        if(x_1<minimum) minimum=x_1;     
      }      
      i++;

      
      
      x_0=x_1;
      
      //delay(200);
      //delayMicroseconds(1200);
    }
    // close the file:
    myFile.close();
    Serial.println(maximum);
    Serial.println(minimum);


    delay(1);
     myFile2 = SD.open("test_nom.txt", FILE_WRITE);
     myFile = SD.open("test.txt");
     while (myFile.available()) {
        b=(myFile.read());
        l += (char)b;
        
        if(i==6){
          x_1=l.toInt();
          l="";
          i=0;
          norm=((x_1-float(minimum))/(float(maximum)-float(minimum))*10.00);  
          //Serial.print(x_1);
          Serial.println(norm);

          myFile2.println(norm);          
        }      
        i++;
      
     }
    myFile.close();
    myFile2.close(); 
    modo=0;
    x_0=0;
  }
      
  
}
