// #include <WiFi.h>
// #include <ESPAsyncWebSrv.h>
#include <HTTPClient.h>
// #include <LiquidCrystal_I2C.h>  
#include <base64.h>
// #include <HardwareSerial.h>
// #include <TinyGPSPlus.h>
// #include "Ultrasonic.h"
#include "Alarm.h"
#include "CScreenLCD.h"
#include "WifiController.h"
#include "ServoController.h"

const char *ssid = "LT";
const char *password = "zavaletayprudencio";

WifiController wifi("SmartTachito", "1234");
CScreenLCD lcd(0x27, 16, 2);  
// UltraSonic ultrasonic(27, 26);
Alarm alarmC(12);
ServoController servos(23, 2, 4);
const int light = 32;
const int pir = 27;

HTTPClient http;
int httpCode;
String response;

String mostConfidentLabel;
double confidence;

const char* serverUrl = "http://192.168.234.193:8000/api/sendImg/";
const char* contentType = "application/json";
const char* cameraServer = "http://192.168.234.129/capture";

void setup(){
  wifi.begin();
  lcd.begin();
  // ultrasonic.begin();
  alarmC.begin();
  servos.begin();
  pinMode(light, OUTPUT);
  pinMode(pir, INPUT);
  Serial.begin(115200);

  wifi.connect(ssid, password, lcd);

  lcd.conexion();
  delay(2000);
  lcd.caratula();

}

void loop(){
  // int distance = ultrasonic.getDistance();

  // if (distance<=30 && distance>=0) {
  if (digitalRead(pir) == 1) {
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
          String payload = "{\"frame\":\"" + base64::encode((uint8_t*)image_data.c_str(), image_data.length()) + "\"}";
          httpCode = http.POST(payload);
          if (httpCode > 0) {

            response = http.getString();

            Serial.print("Response: ");
            Serial.println(response);
            
            WifiController::parseJsonString(response, mostConfidentLabel, confidence);
            http.end();
            
            if (mostConfidentLabel!="Error") {
              lcd.printDetection(mostConfidentLabel, confidence);
              delay(1500);
              alarmC.tick();   

              if (confidence<60) {
                servos.openGeneral();
              } else {
                if (mostConfidentLabel == "PLASTICO" || mostConfidentLabel == "VIDRIO") {
                  servos.openPlastic();
                } else if (mostConfidentLabel == "CARTON" || mostConfidentLabel == "PAPEL") {
                  servos.openPaper();
                } else {
                  servos.openGeneral();
                }
              }
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
