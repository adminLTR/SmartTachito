#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>  
#include <base64.h>
#include <ESP32Servo.h>
#include <HardwareSerial.h>
// #include <TinyGPSPlus.h>
#include "Ultrasonic.h"
#include "Alarm.h"

/* const char *ssid = "LTR";
const char *password = "2J8LQV5L"; */

const char *ssid = "LT";
const char *password = "prudencio";

const int lcdColumns = 16;
const int lcdRows = 2;
#define RXD2 16
#define TXD2 17

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
UltraSonic ultrasonic(27, 26);
Alarm alarmC(12);
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
// HardwareSerial NEO6M(0); 
// TinyGPSPl us gps;

double lng = 0;
double lat = 0;

const char* serverUrl = "http://192.168.51.193:8000/api/sendImg/";
const char* contentType = "application/json";
const char* cameraServer = "http://192.168.51.129/capture";

void parseJsonString(String jsonString, String& mostConfidentLabel, double& confidence) {
  if (jsonString.indexOf("error")>=0) {
    mostConfidentLabel = "Error";
    confidence = 0;
  } else {
    int index = jsonString.indexOf(',');
    mostConfidentLabel = jsonString.substring(26, index-1);
    confidence = jsonString.substring(index+16, jsonString.length()-1).toDouble();
  }
}

/* void getCoors(double&lng, double&lat) {
  while (NEO6M.available() > 0) {
    if (gps.encode(NEO6M.read())) {
      if (gps.location.isUpdated()) {
        Serial.println(gps.location.lat(), 6);
        Serial.println(gps.location.lng(), 6);
      }
    }
  }
} */

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

void printDetectionLCD(String label, double confidence) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(label);
  lcd.setCursor(0, 1);
  lcd.print("%");
  lcd.setCursor(1, 1);
  lcd.print(confidence*100);
}

void printCoorsLCD(double lng, double lat) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LNG: ");
  lcd.setCursor(4, 0);
  lcd.print(lng);
  lcd.setCursor(0, 1);
  lcd.print("LAT: ");
  lcd.setCursor(4, 1);
  lcd.print(lat);
}

void setup(){
  lcd.init();                    
  lcd.backlight();
  caratulaLCD();
  delay(1500);
  lcd.clear();

  ultrasonic.begin();
  alarmC.begin();

  servo1.attach(23);
  servo1.write(0);

  servo2.attach(2);
  servo2.write(0);
  
  servo3.attach(4);
  servo3.write(0);
  
  servo4.attach(13);
  servo4.write(0);

  Serial.begin(115200);

  // CONECTION LCD
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    conectandoLCD();
  }
  conexionLCD();
  delay(2000);
  caratulaLCD();

  // NEO6M.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop(){
  int distance = ultrasonic.getDistance();
  // getCoors(lng, lat);
  /* Serial.println(lng);
  Serial.println(lat); */
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
          /* Serial.print("HTTP Response Code: ");
          Serial.println(httpResponseCode);
          Serial.print("Response: ");
          Serial.println(response); */
          Serial.print("Response: ");
          Serial.println(response);
          String mostConfidentLabel;
          double confidence;
          parseJsonString(response, mostConfidentLabel, confidence);
          http.end();
          
          if (mostConfidentLabel!="Error") {
            printDetectionLCD(mostConfidentLabel, confidence);
            delay(1500);
            //printCoorsLCD(lng, lat);
            alarmC.tick();   
            
            servo1.write(90);
            servo2.write(90);
            servo3.write(90);
            servo4.write(90);
            delay(1500);
            servo1.write(0);
            servo2.write(0);
            servo3.write(0);
            servo4.write(0);
          }
          
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
  delay(1000);
}
