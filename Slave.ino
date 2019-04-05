// Отправитель (стоит) большая Италия
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <dht11.h> // Добавляем библиотеку DHT11
dht11 DHT; // Объявление переменной класса dht11
#define DHT11_PIN 4 // Датчик DHT11 подключен к цифровому пину номер 4
#include <OneWire.h>

OneWire  ds(6);  // подключен к 10 пину (резистор на 4.7к обязателен)

const int trigPin = 3;
const int echoPin = 5;

// defines variables
long duration;
int distance;


int counter = 0;

void setup() {

  Serial.begin(9600); // Скорость работы порта
Serial.println("DHT TEST PROGRAM "); // Выводим текст
Serial.print("LIBRARY VERSION: "); // Выводим текст
Serial.println(DHT11LIB_VERSION);
Serial.println(); // Пустая строка

pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
//Serial.begin(9600); // Starts the serial communication


 // Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {

// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;

// Prints the distance on the Serial Monitor
//Serial.print("Distance: ");
//Serial.println(distance);


byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;

  if ( !ds.search(addr)) {
 //   Serial.println("No more addresses.");
 //   Serial.println();
   ds.reset_search();
    delay(250);
    return;
  }

 // Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
   // Serial.write(' ');
  //  Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
  //   Serial.println("CRC is not valid!");
     return;
  }
 // Serial.println();

  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      //Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
   //   Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
     // Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
     // Serial.println("Device is not a DS18x20 family device.");
      return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // начало коммуникации

  //delay(1000);

  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);         // читаем значение

 // Serial.print("  Data = ");
 // Serial.print(present, HEX);
 // Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // смотрим 9 байтов
    data[i] = ds.read();
 //   Serial.print(data[i], HEX);
 //   Serial.print(" ");
  }
 // Serial.print(" CRC=");
 // Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // Преобразуем получненный данные в температуру
  // Используем int16_t тип, т.к. он равен 16 битам
  // даже при компиляции под 32-х битный процессор
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3;
    if (data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7;
    else if (cfg == 0x20) raw = raw & ~3;
    else if (cfg == 0x40) raw = raw & ~1;
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  //Serial.print(fahrenheit);
  //Serial.println(" Fahrenheit");

int chk;
;
// Мониторинг ошибок
chk = DHT.read(DHT11_PIN); // Чтение данных
switch (chk){
case DHTLIB_OK:
break;
case DHTLIB_ERROR_CHECKSUM:
Serial.println("Checksum error, \t");
break;
case DHTLIB_ERROR_TIMEOUT:
Serial.println("Time out error, \t");
break;
default:
Serial.println("Unknown error, \t");
break;
}
// Выводим показания влажности и температуры
Serial.print("Humidity = ");
Serial.print(DHT.humidity, 1);
Serial.print(", Temp = ");
Serial.println(DHT.temperature,1);
Serial.print("Distance: ");
Serial.println(distance);


//delay(1000);

  Serial.print("Sending packet: ");

  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print(counter);
  LoRa.print(" ");
  LoRa.print(celsius);
  LoRa.print("C ");
  LoRa.print(DHT.humidity, 1);
  LoRa.print("% ");
  LoRa.print(DHT.temperature,1);
   LoRa.print("C ");
  LoRa.print(distance);
   LoRa.print("mm ");
  LoRa.endPacket();

  counter++;

 // delay(1000);
}
