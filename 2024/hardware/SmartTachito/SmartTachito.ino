#include <WiFi.h>
// #include <WiFiAP.h>

#include <HTTPClient.h> 
#include <base64.h>
#include <Wire.h>
// #include <HardwareSerial.h>
// #include <TinyGPSPlus.h>

#include "Ultrasonic.h"
#include "Alarm.h"
#include "CScreenLCD.h"
#include "WifiController.h"
#include "ServoController.h"

// #define RXD2 16
// #define TXD2 17

const char *ssid = "LT";
const char *password = "shawarma";

/* const char *myssid = "SmartTachito";
const char *mypassword = "1234"; */

// WifiController wifi("SmartTachito", "FISI - B22");
WifiController wifiC;
CScreenLCD lcd(0x27, 16, 2);  
UltraSonic ultrasonic(27, 26);
Alarm alarmC(12);
ServoController servos(23, 2, 4);
const int light = 32;

HTTPClient http;
int httpCode;
String response;

String mostConfidentLabel;
double confidence;

const char* serverUrl = "http://192.168.216.135/:8000/api/sendImg/";
const char* contentType = "application/json";
const char* cameraServer = "http://192.168.216.193/capture"; 

// HardwareSerial NEO6M(1);
// TinyGPSPlus gps;
int isConnected = 0;

void setup(){
  lcd.begin();
  lcd.caratula();
  delay(3000);
  Serial.begin(115200);

  wifiC.begin();
  
  ultrasonic.begin();
  alarmC.begin();
  servos.begin();
  pinMode(light, OUTPUT);

  wifiC.connect(ssid, password, lcd);

  lcd.conexion();
  delay(2000);
  lcd.caratula();

  // NEO6M.begin(9600, SERIAL_8N1, RXD2, TXD2);

}

void loop(){
  int distance = ultrasonic.getDistance();

  if (distance<=39 && distance>=0) {
    digitalWrite(light, 1);
    delay(1000);
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(cameraServer);

      httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
          String image_data = http.getString();
          http.end();
          http.begin(serverUrl);
          http.addHeader("Content-Type", contentType);
          String payload = "{\"frame\":\"" + base64::encode((uint8_t*)image_data.c_str(), image_data.length()) + "\"";
          // if (NEO6M.available() > 0 && gps.encode(NEO6M.read())) {
          //   if (gps.location.isValid()) {
          //     payload += ", \"latitude\":\"" + String(gps.location.lat(), 6) + "\", \"longitude\":\"" + String(gps.location.lng(), 6) + "\"";
          //   }
          // }
          payload += "}";
          httpCode = http.POST(payload);
          if (httpCode > 0) {

            response = http.getString();

            Serial.print("Response: ");
            Serial.println(response);
            
            WifiController::parseJsonString(response, mostConfidentLabel, confidence);
            http.end();
            alarmC.tick();  
            
            if (mostConfidentLabel!="Error") {
              if (confidence<60) {
                servos.openGeneral();
                mostConfidentLabel = "GENERALES";
              } else {
                if (mostConfidentLabel == "PLASTICO" || mostConfidentLabel == "VIDRIO") {
                  servos.openPlastic();
                } else if (mostConfidentLabel == "CARTON" || mostConfidentLabel == "PAPEL") {
                  servos.openPaper();
                } else {
                  servos.openGeneral();
                }
              }
              lcd.printDetection(mostConfidentLabel, confidence);
            }
          } else {
            lcd.printError("servidor");
          }
          
        } else {
          lcd.printError("camara");
        }
      } else {
        lcd.printError("camara");
      }
    }
  }
  delay(1000);
  digitalWrite(light, 0);
}
