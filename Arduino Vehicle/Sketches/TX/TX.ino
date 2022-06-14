#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(9, 10);
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
byte values[4] = {0,0,0,0};


void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(10);
  txSetup();
  Serial.println("Arduino is alive!!");
  delay(100);
}


void loop()
{
  while (Serial.available())
  {
    String input = Serial.readStringUntil(";");
    SetArray(input);
    radio.write(&values, sizeof(values));
  }
}


void SetArray(String input)
{
  input += ".";
  if (!isDigit(input[0])) return;
  
  int intIndex = 0;
  String buf = "";

  for(int i = 0; i < input.length(); i++)
  {
    if (isDigit(input[i]))
    {
      buf += input[i];
    }
    else
    {
      values[intIndex] = buf.toInt();
      buf = "";
      intIndex++;
    }
  }
}


void txSetup()
{
  radio.begin();              // активировать модуль
  //radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  //radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  //radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(4);   // размер пакета, в байтах // 32

  radio.openWritingPipe(address[5]);  // мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x79);             // выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_LOW);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp();        // начать работу
  radio.stopListening();  // не слушаем радиоэфир, мы передатчик
}
