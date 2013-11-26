#include <TinyGPS.h>
#include <SoftwareSerial.h>

#define RXPIN 3
#define TXPIN 2
#define GPSBAUD 4800
#define TERMBAUD 14400


SoftwareSerial uart_gps(RXPIN, TXPIN);
TinyGPS gps;

void setup(){
    Serial.begin(TERMBAUD);
    uart_gps.begin(GPSBAUD);
}

void loop()
{
  while (uart_gps.available())
  {
    int c = uart_gps.read();
    if (gps.encode(c))
    {
      Serial.print("Hello World");
    }
  }
}
