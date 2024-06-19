#include <WiFi.h>
#include <WiFiAP.h>

class WifiController
{
private:
    const char*ssid;
    const char*password;

    const char*wifiSsid;
    const char*wifiPass;
public:
    WifiController(const char*ssid, const char*password) : ssid(ssid), password(password) {}
    ~WifiController() {}
    void begin() {
        WiFi.mode(WIFI_AP_STA);
        if (!WiFi.softAP(this->ssid, this->password)) {
          log_e("Soft AP creation failed.");
          while (1);
        }
    }
    void connect(const char*ssid, const char*password, CScreenLCD lcd) {
        this->wifiSsid = ssid;
        this->password = password;
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
