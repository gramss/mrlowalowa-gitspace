/*

First Git-code ... more to come

Status:

- OneWire DS18B20 Temp-Sensor
- converting Temp int to String
- Sending Temp String via 433 MHz (itoa)

Copyright (c): mrlowalowa 27.01.2014 :P

*/


#include <OneWire.h>
#include <DallasTemperature.h>
#include <VirtualWire.h>

#define ONE_WIRE_BUS 12
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
int temp, sens_fail = 0;
char msg[10];

void setup(){
  Serial.begin(9600); //Debug
  Serial.println("Setup Start");
  sensors.begin(); //One-Wire
  
  // 433 Mhz Virtual Wire
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);	 // Bits per sec
  vw_set_rx_pin(4);
  vw_set_tx_pin(3);
  vw_rx_start();
  
  Serial.println("Setup End");
}

void loop(){
  Serial.print("Temp-Abfrage...");
  sensors.requestTemperatures();
  Serial.print("beendet!");
  Serial.println("");
  if (sensors.getTempCByIndex(0) == (-127.00 || 85.00)){
    sensors.begin(); // Bei Fehlinitialisierung oder Fehlerquellen bei der Temperaturübertragung --> Neustart der Sensoren
    Serial.println("Sensoren neu initialisiert");
    sens_fail++;
    Serial.println("Fehler bis jetzt: ");
    Serial.print(sens_fail);
    delay(10);
  }
  temp = sensors.getTempCByIndex(0);
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println("");
  // TEMP ABFRAGE BEENDET
  
  uint8_t buf[VW_MAX_MESSAGE_LEN]; // Längen der msg für Virtual-Wire
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  itoa(temp,msg,10);  // int to string conversion
  
  Serial.print("Char msg: ");
  Serial.print(msg);
  Serial.println(" ");
  
  digitalWrite(13, true); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  Serial.println("Sent");
  digitalWrite(13, false);
  
  /*********************
  More coming up:
  
  - recive com from Server (this is a client.)
  - converting string back to int (fkt.: atoi(...) )
  - RGB Strip control
  - RGB Strip Power Supply Power On / Off Relay
  
  Far Future:
  - talk to Arduino in Garden (Ground Water %, temp, battery..)
  
  
  **********************/
  }
  
