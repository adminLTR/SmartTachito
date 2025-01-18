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
    int closedAngle = 10;
    int openedAngle = 90;
    int mediumAngle = 25;
    void openGate(Servo*open, Servo*s1, Servo*s2) {
        open->write(this->openedAngle);
        s1->write(this->mediumAngle);
        s2->write(this->mediumAngle);
        delay(3000);
        open->write(this->closedAngle);
        s1->write(this->closedAngle);
        s2->write(this->closedAngle);
    }
public:
    ServoController(int plastic, int paper, int general) : plastic(plastic), paper(paper), general(general) { }
    ~ServoController() {}
    void begin() {
        this->servoPaper->attach(paper);
        this->servoPlastic->attach(plastic);
        this->servoGeneral->attach(general);
        this->servoPlastic->write(this->closedAngle);
        this->servoPaper->write(this->closedAngle);
        this->servoGeneral->write(this->closedAngle);
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