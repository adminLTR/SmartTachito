class UltraSonic {
  private:
    int echo;
    int trigger;
    void throwSignal() {
      digitalWrite(trigger, HIGH);
      delay(1);
      digitalWrite(trigger, LOW);
    }
  public:
    UltraSonic(int echo, int trigger) {
      this->echo = echo;
      this->trigger = trigger;
    }
    void begin() {
      pinMode(echo, INPUT);
      pinMode(trigger, OUTPUT);
    }
    int getDistance() {
      throwSignal();
      int duration = pulseIn(echo, HIGH);
      int distance = (duration ) / 58.2;
      return distance;
    }
};