// Приемник (идет) маленький Китай

#include <SPI.h>
#include <LoRa.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(2,3,4,5,6,7);

void setup() {
  
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

    lcd.begin(16, 2);
  // Print a message to the LCD.
// lcd.print("Paket:");
  
}


void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  int rssi = LoRa.packetRssi();
   
    
    
 int k=0; 
 int m=0;
 int n=0;
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
 
    // read packet
    while (LoRa.available()) {
      char number = ((char)LoRa.read());
     Serial.print(number);

      lcd.setCursor(k,m);
      lcd.write(number);
      k++;
      lcd.setCursor(14,0);
      if (k>13) {
        
        m=1; k=0;
      lcd.setCursor(k,m);
      //n++;
      
      }
      //char number = (LoRa.read());
      //lcd.setCursor(1, 2);
      //int number = LoRa.read();
              }
          
          

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());


    lcd.setCursor(1,2);
   // lcd.write(number);
      
    /*lcd.setCursor(0,1);
    lcd.print("RSSI:");
    lcd.print(rssi);
    lcd.setCursor(9,1);
    lcd.print("Bytes:");
    lcd.print(packetSize);
   */ 
  }

    // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 1);
  // print the number of seconds since reset:
 // lcd.print((char)LoRa.read());
  
}
