#include <esp_now.h> //libreria espNOW
#include <WiFi.h>  //librería wifi
#include <SPI.h>
#include <mySD.h>
#include <Ewma.h>
#include <FIRFilter.h>

// REMPLAZAR CON LA DIRECCION MAC del RECEPTOR
uint8_t broadcastAddress[] = {0x08, 0x3A, 0xF2, 0xAC, 0x57, 0xDC};

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
uint8_t S=0;
unsigned int pico=0;
unsigned int pico_1=0;
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
Ewma adcFilter2(0.007);  // More smoothing - less prone to noise, but slower to detect changes
//float filtered1=0;
int filtered=0;


unsigned int datos;

const double b_coefficients[] = {0.00121, 0.00135, 0.00159, 0.00193, 0.00241, 0.00301, 0.00376, 0.00467, 0.00572, 0.00693,
0.00829,  0.00978,  0.01141,  0.01315,  0.01498,  0.01689,  0.01886,  0.02085,  0.02284,  0.02481,
0.02673,  0.02856,  0.03028,  0.03188,  0.03331,  0.03456,  0.03562,  0.03645,  0.03706,  0.03743,
0.03755,  0.03743,  0.03706,  0.03645,  0.03562,  0.03456,  0.03331,  0.03188,  0.03028,  0.02856,
0.02672,  0.02481,  0.02284,  0.02085,  0.01886,  0.01689,  0.01498,  0.01315,  0.01141,  0.00978,
0.00828,  0.00693,  0.00572,  0.00466,  0.00376,  0.00301,  0.00240,  0.00193,  0.00159,  0.00135,
0.00121


};
FIRFilter fir(b_coefficients);
int filtered2=0;





unsigned long promedio=0;
unsigned long promedio_1=0;

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
  //SD.remove("test.csv");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  //myFile = SD.open("test.csv", FILE_WRITE);
  
  // if the file opened okay, write to it:
  /*if (myFile) {
    Serial.print("Writing to test.csv...");
    myFile.println("testing 1, 2, 3.");
  // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.csv");
  }*/
  
  // re-open the file for reading:
  /*myFile = SD.open("test.csv");
  if (myFile) {
    Serial.println("test.csv:");
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.csv");
  }*/
}



void loop()
{
  digitalWrite(15,LOW);
  
  
     if(modo==1){
        adquisicion();
      }


  if(modo==3){
    SD.remove("test_nom.csv");
    x_0=0;
    x_1=0;
    unsigned int i=1;
    int c=0;
    digitalWrite(15,HIGH);
    myFile = SD.open("test.csv");
    
    pico=0;
    //minimum=5000;
    x_0=0;
    // read from the file until there's nothing else in it:
    
    delay(1);
    x_0=0;
    c=0;
     myFile = SD.open("test.csv");
     while (myFile.available()) {
      
        b=(myFile.read());
        l=(char)b;
        if(i==5){
          c++; 
          Serial.println(l);
          x_1=l.toInt();
          if(x_1==1)
          {
            l=c;
            esp_now_send(broadcastAddress, (uint8_t *) &c, sizeof(c));
            //Serial.println(l);  
            delay(10);   
          }
        }    
        if(i==7){
          i=0;
                 
        }
        i++;
     }
    myFile.close();
    modo=0;
    x_0=0;
  }
      
  
}


void adquisicion(){
      float d_0=0;
      pico=5000;
      pico_1=5000;
      promedio=0;
      promedio_1=0;
      SD.remove("test.csv");
      unsigned int t=0;
      unsigned int t_1=0;
      unsigned int c=0;
      unsigned int c_1=0;
      unsigned int d_1=0;
      unsigned int S_1=0;
      
      unsigned int GSR=0; //variable de lectura GSR
      
      myFile = SD.open("test.csv", FILE_WRITE);   
        for(unsigned int i=0; i<=50000; i++){
          
          digitalWrite(15,!digitalRead(15));
          analogRead(33);
          datos=analogRead(36); 
          //filtered1 = adcFilter1.filter(datos);
          //filtered2 = adcFilter2.filter(datos);
          filtered2=fir.filter(datos);
          //Serial.println(datos);
          if(i>600){
            d_0=((filtered2-d_0)*50)+200;
            d_1=d_0;
            
            
            //myFile.print(filtered2);
            
            //Serial.print(filtered2);
            //myFile.print(",");
            //Serial.print(",");
            
            if(t_1<=500){
              promedio_1+=filtered2;
              
            }
            else{
              pico_1=(promedio_1/500)+5;
                promedio_1=0;
                t_1=0;
              }
            t_1++;
            if(S_1==1 && filtered2<x_1 && filtered2>pico_1 && c_1>280){
                //myFile.print("1");
               // Serial.print("1");
                c_1=0;

            }
            else {
               //myFile.print("0");
               //Serial.print("0");
            }
            
            if(filtered2>x_1) {
              S_1=1;
            }
            if(filtered2<x_1) {
              S_1=0;
            }
            
           c_1++;

            //myFile.print(",");
            //Serial.print(",");
            
            myFile.print(filtered2);            
            Serial.print(filtered2);
            myFile.print(",");
            Serial.print(",");
            
            if(t<=500){
              promedio+=d_1;
              
            }
            else{
              pico=(promedio/500)+10;
                promedio=0;
                t=0;
              }
            t++;
            
            if(S==1 && d_1<x_0 && d_1>pico && c>280){
                myFile.print("1");
                Serial.print("1");
                c=0;

            }
            else {
               myFile.print("0");
               Serial.print("0");
            }
            
            if(d_1>x_0) {
              S=1;
            }
            if(d_1<x_0) {
              S=0;
            }
            
           c++;
            
            
             myFile.print(",");
             Serial.print(",");
             GSR=analogRead(34);
             myFile.print(GSR);
             Serial.print(GSR);
             myFile.print(",");
             Serial.print(",");
             filtered=adcFilter2.filter(GSR);
             //filtered=fir.filter(filtered2);
             myFile.println(filtered);
             Serial.println(filtered); 
            
          }
          d_0=filtered2;
          analogRead(39);
          
          analogRead(35);
          analogRead(32);
          
          //delayMicroseconds(850);
          delayMicroseconds(1600);
          //delay(1);
          x_0=d_1;
          x_1=filtered2;
          
        }
        Serial.print(promedio);
        myFile.close();
        modo=0;
}
