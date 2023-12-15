#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <base64.h>
#include "Ultrasonic.h"
#include "Alarm.h"

const char *ssid = "LTR";
const char *password = "2J8LQV5L";

/* const char *ssid = "LT";
const char *password = "prudencio"; */


const int lcdColumns = 16;
const int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
UltraSonic ultrasonic(27, 26);
Alarm alarmC(12);

const char* serverUrl = "http://192.168.1.17:8000/api/sendImg/";
const char* contentType = "application/json";
const char* cameraServer = "http://192.168.1.11/capture";


void parseJsonString(String jsonString, String& mostConfidentLabel, double& confidence) {
  int index = jsonString.indexOf(',');
  mostConfidentLabel = jsonString.substring(26, index-1);
  confidence = jsonString.substring(index+16, jsonString.length()-1).toDouble();
}

void caratulaLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("JorgeKtch LuisLT");
  lcd.setCursor(1, 1);
  lcd.print("Smart  Tachito");
}
void conectandoLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Connecting");
  lcd.setCursor(0, 1);
  lcd.print("to red...");
  delay(1000);
  lcd.clear();
}
void conexionLCD(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Successfully");
  lcd.setCursor(0, 1);
  lcd.print("connected");
}

void setup(){
  lcd.init();                    
  lcd.backlight();
  caratulaLCD();
  delay(1500);
  lcd.clear();

  ultrasonic.begin();
  alarmC.begin();

  Serial.begin(9600);

  // CONECTION LCD
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    conectandoLCD();
  }
  conexionLCD();
  delay(2000);
  caratulaLCD();
}

void loop(){
  int distance = ultrasonic.getDistance();
  if (distance<=8 && distance>=0) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(cameraServer);

      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
          String image_data = http.getString();
          http.end();
          http.begin(serverUrl);
          http.addHeader("Content-Type", contentType);
          String payload = "{\"frame\":\"" + base64::encode((uint8_t*)image_data.c_str(), image_data.length()) + "\"}";
          int httpResponseCode = http.POST(payload);
          String response = http.getString();
          Serial.print("HTTP Response Code: ");
          Serial.println(httpResponseCode);
          Serial.print("Response: ");
          Serial.println(response);
          String mostConfidentLabel;
          double confidence;
          parseJsonString(response, mostConfidentLabel, confidence);
          Serial.println(mostConfidentLabel);
          Serial.println(confidence);
          http.end();

          alarmC.tick();
          
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Error al conectar");
          lcd.setCursor(0, 1);
          lcd.print("con la camara");
        }
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("No hay conexion");
        lcd.setCursor(0, 1);
        lcd.print("con la camara");
      }
    }
  }
  delay(2000);
}
