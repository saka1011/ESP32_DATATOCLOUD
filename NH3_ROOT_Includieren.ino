//----------------------google_WiFi_includes-----------------------------------------------------//
#include "WiFiClientSecure.h"                 //Bilbiotheken für Internet verbindung               
#include "WiFi.h"                             // --""--
#include <Wire.h>
#include <SPI.h>
#include <HTTPClient.h>                       //für HTTP anfragen an google

//----------------------EIGENE_DATEN_EINTRAGEN->WLAN und Google Script----------------------------// 
const char* ssid     = "NH3WiFi";          //Netzwerk mit dem Verbunden werden soll  
const char* password = "HITlabor2019";      //Netzwerk Passwort
const char*  server = "script.google.com";    // Server URL
// google script key
const char* key = "AKfycby0i4LrdutdWgu6eQNQJQzLonMj0_tuSdStSNYN1cNAW35nrRiO"; //Identität des Google Scripts
//-----------------------------------------------------------------------------------------------//

#define debug true  //für debug ausgaben
int zeitindex=0;

void setup() 
{
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

}

void loop() 
{ 
  if(zeitindex>15) 
  {
  cloudfkt();       //nach bestimmterzeit in cloud schreiben (fkt)
  zeitindex=0;
  }
  delay(delayTime);
  zeitindex++;
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
 
    //SendeZumGoogle(URL);
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
  Serial.println("keine Verbindung mehr. Bitte neustarten");
  ESP.restart(); 
  }  
}
