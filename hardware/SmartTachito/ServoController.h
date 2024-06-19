#include <ESP32Servo.h>

class ServoController
{
private:
    Servo*servoPlastic = new Servo();
    Servo*servoPaper = new Servo();
    Servo*servoGeneral = new Servo();
    int plastic;
    int paper;
    int general;
    void openGate(Servo*open, Servo*s1, Servo*s2) {
        open->write(180);
        s1->write(45);
        s2->write(45);
        delay(3000);
        open->write(0);
        s1->write(0);
        s2->write(0);
    }
public:
    ServoController(int plastic, int paper, int general) : plastic(plastic), paper(paper), general(general) { }
    ~ServoController() {}
    void begin() {
        this->servoPaper->attach(paper);
        this->servoPlastic->attach(plastic);
        this->servoGeneral->attach(general);
        this->servoPlastic->write(0);
        this->servoPaper->write(0);
        this->servoGeneral->write(0);
    }
    void openGeneral() {
        this->openGate(this->servoGeneral, this->servoPaper, this->servoPlastic);
    }
    void openPaper() {
        this->openGate(this->servoPaper, this->servoGeneral, this->servoPlastic);
    }
    void openPlastic() {
        this->openGate(this->servoPlastic, this->servoPaper, this->servoGeneral);
    }
};