#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define motor1in 2
#define motor1pwm 5
#define motor2in 4
#define motor2pwm 6
#define fpvYpin 3

RF24 radio(9, 10);
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
byte values[4] = {0, 0, 0, 0};
Servo fpvY;


void setup() {
  fpvY.attach(fpvYpin);
  Serial.begin(9600);
  Serial.setTimeout(10);
  motorSetup();
  rxSetup();
  delay(100);
}


void loop()
{
  if (radio.available())
  {
    while (radio.available())
    {
      radio.read(&values, sizeof(values));

      setMotors(map(values[0], 0, 255, -255, 255), map(values[1], 0, 255, -255, 255));
      fpvY.write(map(values[3], 0, 255, 45, 135));
    }
  }
}


void setMotors(int inx, int iny) // -255 to 255
{
  setMotor(iny + inx, motor1pwm, motor1in);
  setMotor(iny - inx, motor2pwm, motor2in);
}
void setMotor(int mspeed, int pinPwm, int pinIn)
{
  if (mspeed > 0) // forward
  {
    analogWrite(pinPwm, mspeed);
    digitalWrite(pinIn, 0);
  }
  else if (mspeed < 0) // back
  {
    analogWrite(pinPwm, 255 + mspeed);
    digitalWrite(pinIn, 1);
  }
  else // brake
  {
    digitalWrite(pinPwm, 0);
    digitalWrite(pinIn, 0);
  }
}


void motorSetup()
{
  pinMode(motor1in, OUTPUT);
  pinMode(motor2in, OUTPUT);

  pinMode(motor1pwm, OUTPUT);
  pinMode(motor2pwm, OUTPUT);
}


void rxSetup()
{
  radio.begin();              // активировать модуль
  //radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  //radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  //radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(4);   // размер пакета, в байтах // 32

  radio.openReadingPipe(1, address[5]);   // хотим слушать трубу 0
  radio.setChannel(0x79);     // выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_LOW);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp();        // начать работу
  radio.startListening(); // начинаем слушать эфир, мы приёмный модуль
}
