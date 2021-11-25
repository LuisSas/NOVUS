/*Programa utilizado para realizar prueba del filtro media Movil de la librería EWMA y guardar datos en la microSD*/

#include <WiFi.h>  //librería wifi
#include <SPI.h>  //Librería SPI
#include <mySD.h> //Librería para microSD
#include <Ewma.h> //Librería filtro EWMA


unsigned int lectura=0;


//Ewma adcFilter1(0.1);   // Less smoothing - faster to detect changes, but more prone to noise
Ewma adcFilter2(0.01);  // More smoothing - less prone to noise, but slower to detect changes
//float filtered1=0;
int filtered2=0;


unsigned int datos;



File myFile;
const int chipSelect = 5;

void setup()
{
 
  Serial.begin(115200);
  pinMode(36, INPUT);
  pinMode(15,OUTPUT);


  Serial.print("Initializing SD card...");
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
  
  if(Serial.available())
  {
     if(Serial.read()=='1'){
      SD.remove("test.txt");
      myFile = SD.open("test.txt", FILE_WRITE);   
        for(unsigned int i=0; i<=12500; i++){
          digitalWrite(15,!digitalRead(15));
          datos=analogRead(36); 
          //filtered1 = adcFilter1.filter(datos);
          filtered2 = adcFilter2.filter(datos); //filtering the analog data
          //Serial.println(datos);      
          myFile.println(filtered2); //Saving data in the SD
          Serial.println(filtered2);
          analogRead(39);
          analogRead(34);
          analogRead(35);
          analogRead(32);
          analogRead(33);
          //delayMicroseconds(100);
          
          //myFile.flush();
          
        }
        myFile.close();
      }
      else delay(100);

  }
  
}
