// Отправитель (стоит) большая Китай

#include <SPI.h>
#include <LoRa.h>

int counter = 0;
int cel = 40;
void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(cel);

  // send packet
  LoRa.beginPacket();
  LoRa.write(cel,1);
    LoRa.endPacket();

  counter++;

  delay(1000);
}
