
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>                                //OLED Grafikausgabe
#include <Adafruit_SSD1306.h>                            //OLED Ansteuerung des Displaytreibers
#include "MutichannelGasSensor.h"                        //NH3_Sensor 

Adafruit_BME280 bme; // I2C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//----------------------google_WiFi_includes-----------------------------------------------------//
#include "WiFiClientSecure.h"                 //Bilbiotheken für Internet verbindung               
#include "WiFi.h"                             // --""--
#include <Wire.h>
#include <SPI.h>
#include <HTTPClient.h>                       //für HTTP anfragen an google
#define SEALEVELPRESSURE_HPA (1013.25)

//----------------------EIGENE_DATEN_EINTRAGEN->WLAN und Google Script----------------------------// 
const char* ssid     = "NH3WiFi";          //Netzwerk mit dem Verbunden werden soll  
const char* password = "HITlabor2019";      //Netzwerk Passwort
const char*  server = "script.google.com";    // Server URL
// google script key
const char* key = "AKfycby0i4LrdutdWgu6eQNQJQzLonMj0_tuSdStSNYN1cNAW35nrRiO"; //Identität des Google Scripts
//-----------------------------------------------------------------------------------------------//

#define debug true  //für debug ausgaben
int zeitindex=0;


unsigned long delayTime;
  

void setup() {
  Serial.begin(115200);
  //--------------------------OLED Display Einstellungen---------------------------------
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);           //OLED I2C initialisieren
  gas.begin(0x04);
  gas.powerOn();
  display.display();                                   //sorgt für Darstellung auf dem Display
  delay(2000);                                          //warte 2s
  display.clearDisplay();                              //Display leeren
  display.setTextSize(1);                              //Textgröße einstellen
  display.setTextColor(WHITE);   

//-------------------------Setup_WiFi_verbindung----------------------------------------------------------//
 Serial.begin(115200);                                 //Baudrate von WLAN
  Serial.println("Warte auf Verbindung");
  WiFi.begin(ssid, password);                           //Verbindung beginnen

  int abbruch=0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    abbruch++;
    if (abbruch==20)ESP.restart();                      //nach belibigerzeit ohne Verbindung neustarten
  }
  Serial.println("");
  Serial.print("IP Addresse: ");
  Serial.println(WiFi.localIP());


//-------------------------Setup_BME---------------------------------------------------------//  
  bool status;
  status = bme.begin(0x76);  
  if (!status) 
  {
  Serial.println("Error!");
  while (1);
  }
  delayTime = 2000;
}

 
void loop() 
{ 
  printValues();
  if(zeitindex==0) 
  {
  cloudfkt();
  zeitindex=15;
  }
  delay(delayTime);
  zeitindex--;
}

void printValues() 
{
  Serial.print("Temperatur = ");
  Serial.print(bme.readTemperature());
  Serial.println(" °C");
  Serial.print("Feuchtigkeit = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");
  Serial.print("NH3 = ");
  if(gas.measure_NH3()>=0) Serial.print(gas.measure_NH3());
  else Serial.print("invalid");
  Serial.println(" ppm");
  Serial.println();
  
  display.display(); 
  display.clearDisplay();  
  display.setCursor(0,5);                  //Textposition (1. Reihe)
  display.print("Temp. : ");                //OlED Ausgabe
  display.print(bme.readTemperature());
  display.println(" C");
  display.setCursor(0,25);                 //Textposition (2. Reihe)
  display.print("Feucht. : ");
  display.print(bme.readHumidity());
  display.println("%");   
  display.setCursor(0,45);                 //Textposition (3. Reihe)
  display.print("NH3 : ");
  display.print(gas.measure_NH3());
  display.println("ppm");              
}

void cloudfkt()
{
  String URL="https://script.google.com/macros/s/";         //skript server
  URL += key;                                               //skript key(Identität)
  URL += "/exec?";
  URL += "1_Spalte=";                                       //1.Spalte
  URL += bme.readTemperature();                             //sensordaten in Link eintragen
  URL += "&2_Spalte=";                                      //2.Spalte
  URL += bme.readHumidity();                                //sensordaten in Link eintragen
  URL += "&3_Spalte=";
  URL += gas.measure_NH3();
 
 if ((WiFi.status() == WL_CONNECTED)) { //Noch verbunden?
 
    HTTPClient http;
 
    http.begin(URL); //Specify the URL                                //verbindung zum Skript aufbauen
    int httpCode = http.GET();                                        //sende den Link an das Skript
 
    /*if (httpCode > 0) { //Check for the returning code              //Antwort von Website
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }*/
 
    http.end(); //Free the resources
  if(debug)Serial.println("Gesendet."); 
  }
  else
  {
  Serial.println("keine Verbindung mehr. Esp startet neu");
  ESP.restart(); 
  }  
}
