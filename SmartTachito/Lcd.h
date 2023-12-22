namespace LCD
{
    void caratulaLCD(LiquidCrystal_I2C&lcd) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("JorgeKtch LuisLT");
        lcd.setCursor(1, 1);
        lcd.print("Smart  Tachito");
    }
    void conectandoLCD(LiquidCrystal_I2C&lcd) {
    lcd.setCursor(0, 0);
    lcd.print("Connecting");
    lcd.setCursor(0, 1);
    lcd.print("to red...");
    delay(1000);
    lcd.clear();
    }
    void conexionLCD(LiquidCrystal_I2C&lcd){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Successfully");
    lcd.setCursor(0, 1);
    lcd.print("connected");
    }

    void printDetectionLCD(LiquidCrystal_I2C&lcd, String label, double confidence) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(label);
    lcd.setCursor(0, 1);
    lcd.print("%");
    lcd.setCursor(1, 1);
    lcd.print(confidence);
    }

    void printCoorsLCD(LiquidCrystal_I2C&lcd, double lng, double lat) {
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

} // namespace lcd

