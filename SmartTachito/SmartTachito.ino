#include <WiFi.h>
// #include <ESPAsyncWebSrv.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>  
#include <base64.h>
#include <ESP32Servo.h>
#include <HardwareSerial.h>
#include <TinyGPSPlus.h>
#include "Ultrasonic.h"
#include "Alarm.h"
#include "Lcd.h"

/* const char *ssid = "LTR";
const char *password = "2J8LQV5L"; */

/* const char *ssid = "LT";
const char *password = "prudencio"; */

/* const char *ssid = "yisus";
const char *password = "jorgitoprecioso"; */

const char *ssid = "esp32xd";
const char *password = "SmartTachito";

const int lcdColumns = 16;
const int lcdRows = 2;
/* #define RXD2 16
#define TXD2 17 */

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
UltraSonic ultrasonic(27, 26);
Alarm alarmC(12);
Servo servoPlastik;
Servo servoPaper;
Servo servoGeneral;
/* HardwareSerial NEO6M(0); 
TinyGPSPlus gps; */

const int light = 32;

const char* serverUrl = " http://192.168.4.2:8000/api/sendImg/";
const char* contentType = "application/json";
const char* cameraServer = "http://192.168.4.3/capture";

// AsyncWebServer server(80); 

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

/* void getCoors(double& lng, double& lat) {
  while (NEO6M.available() > 0) {
    if (gps.encode(NEO6M.read())) {
      Serial.print("Satellites: ");
      Serial.println(gps.satellites.value());
      
      if (gps.location.isValid()) {
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
        
        lat = gps.location.lat();
        lng = gps.location.lng();
        return;
      } else {
        Serial.println("Invalid GPS data");
      }
    }
  }
} */

void setup(){
  lcd.init();                    
  lcd.backlight();
  LCD::caratulaLCD(lcd);
  delay(1500);
  lcd.clear();

  ultrasonic.begin();
  alarmC.begin();

  servoPlastik.attach(23);
  servoPlastik.write(0);

  servoPaper.attach(2);
  servoPaper.write(0);
  
  servoGeneral.attach(4);
  servoGeneral.write(0);

  Serial.begin(115200);

  // CONECTION LCD
  /* WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    LCD::conectandoLCD(lcd);
  } */
  WiFi.softAP(ssid, password);

  Serial.println("Punto de acceso iniciado");
  Serial.print("Dirección IP del servidor: ");
  Serial.println(WiFi.softAPIP());

  pinMode(light, OUTPUT);

  /* server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String response = "WiFi Credentials: " + String(ssid) + ", " + String(password);
    request->send(200, "text/plain", response);
  });

  server.begin(); */

  LCD::conexionLCD(lcd);
  delay(2000);
  LCD::caratulaLCD(lcd);

  // NEO6M.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop(){
  /* double longitude = 0;
  double latitude = 0; */
  int distance = ultrasonic.getDistance();
  // getCoors(longitude, latitude);
  /* Serial.println(longitude);
  Serial.println(latitude); */
  Serial.println(distance);
  if (distance<=30 && distance>=0) {
    digitalWrite(light, 1);
    //if (WiFi.status() == WL_CONNECTED) {
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
            LCD::printDetectionLCD(lcd, mostConfidentLabel, confidence);
            delay(1500);
            //printCoorsLCD(lng, lat);
            alarmC.tick();   

            if (mostConfidentLabel == "PLASIICO" || mostConfidentLabel == "VIDRIO") {
              servoPlastik.write(180);
              servoPaper.write(45);
              servoGeneral.write(45);
              delay(3000);
              servoPlastik.write(0);
              servoPaper.write(0);
              servoGeneral.write(0);
            } else if (mostConfidentLabel == "CARTON" || mostConfidentLabel == "PAPEL") {
              servoPaper.write(180);
              servoPlastik.write(45);
              servoGeneral.write(45);
              delay(3000);
              servoPlastik.write(0);
              servoPaper.write(0);
              servoGeneral.write(0);
            } else {
              servoGeneral.write(180);
              servoPaper.write(45);
              servoPlastik.write(45);
              delay(3000);
              servoPlastik.write(0);
              servoPaper.write(0);
              servoGeneral.write(0);
            }
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
    //}
  }
  delay(1000);
  digitalWrite(light, 0);

}
