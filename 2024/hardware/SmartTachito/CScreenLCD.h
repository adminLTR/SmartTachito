#include <LiquidCrystal_I2C.h> 

class CScreenLCD
{
private:
    LiquidCrystal_I2C*lcd;
public:
    CScreenLCD(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows) {
        this->lcd = new LiquidCrystal_I2C(lcd_Addr, lcd_cols, lcd_rows);
    }
    ~CScreenLCD() {}
    void begin() {
        this->lcd->init();                    
        this->lcd->backlight();
        this->lcd->clear();
    }
    void caratula() {
        this->lcd->clear();
        this->lcd->setCursor(0, 0);
        this->lcd->print("JorgeKtch LuisLT");
        this->lcd->setCursor(1, 1);
        this->lcd->print("Smart  Tachito");
    }
    void crearAP() {
        this->lcd->clear();
        this->lcd->setCursor(0, 0);
        this->lcd->print("Creando");
        this->lcd->setCursor(1, 1);
        this->lcd->print("Access point");
    }
    void conectando() {
        this->lcd->setCursor(0, 0);
        this->lcd->print("Connecting");
        this->lcd->setCursor(0, 1);
        this->lcd->print("to red...");
        delay(1000);
        this->lcd->clear();
    }
    void conexion(){
        this->lcd->clear();
        this->lcd->setCursor(0, 0);
        this->lcd->print("Successfully");
        this->lcd->setCursor(0, 1);
        this->lcd->print("connected");
    }

    void printDetection(String label, double confidence) {
        this->lcd->clear();
        this->lcd->setCursor(0, 0);
        this->lcd->print(label);
        this->lcd->setCursor(0, 1);
        this->lcd->print("%");
        this->lcd->setCursor(1, 1);
        this->lcd->print(confidence);
    }

    void printCoors(double lng, double lat) {
        this->lcd->clear();
        this->lcd->setCursor(0, 0);
        this->lcd->print("LNG: ");
        this->lcd->setCursor(4, 0);
        this->lcd->print(lng);
        this->lcd->setCursor(0, 1);
        this->lcd->print("LAT: ");
        this->lcd->setCursor(4, 1);
        this->lcd->print(lat);
    }
    void printError(String error) {
        this->lcd->clear();
        this->lcd->setCursor(0, 0);
        this->lcd->print("Error al conectar");
        this->lcd->setCursor(0, 1);
        this->lcd->print("con " + error);
    }
};
