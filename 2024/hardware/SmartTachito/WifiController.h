#include <WiFi.h>
#include <WiFiAP.h>

class WifiController
{
private:

public:
    WifiController() {}
    ~WifiController() {}
    void begin() {
        WiFi.mode(WIFI_STA);
    }
    void connect(const char*ssid, const char*password, CScreenLCD lcd) {
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            lcd.conectando();
        }
    }
    static void parseJsonString(String jsonString, String& mostConfidentLabel, double& confidence) {
        if (jsonString.indexOf("error")>=0) {
            mostConfidentLabel = "Error";
            confidence = 0;
        } else {
            int index = jsonString.indexOf(',');
            mostConfidentLabel = jsonString.substring(26, index-1);
            confidence = jsonString.substring(index+16, jsonString.length()-1).toDouble();
        }
    }
};
