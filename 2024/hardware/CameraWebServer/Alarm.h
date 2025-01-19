class Alarm {
  private:
    int led;
  public:
    Alarm(int led) {
      this->led = led;
    }
    void begin() {
      pinMode(led, OUTPUT);
    }
    void on() {
      digitalWrite(led, 1);
    }
    void off() {
      digitalWrite(led, 0);
    }
    void tick() {
      on();
      delay(500);
      off();
    }
};