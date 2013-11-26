#include <TinyGPS.h>

/*
6-12-12
Aaron Weiss
SparkFun Electronics, Beerware

Example GPS Parser based off of arduiniana.org TinyGPS examples.

Parses NMEA sentences from an EM406 running at 4800bps into readable
values for latitude, longitude, elevation, date, time, course, and
speed. Use 115200 baud for your serial port baud rate

For the SparkFun GPS Shield. Make sure the switch is set to DLINE.

Once you get your longitude and latitude you can paste your
coordinates from the terminal window into Google Maps. Here is the
link for SparkFun's location.
http://maps.google.com/maps?q=40.06477,+-105.20997

Uses the NewSoftSerial library for serial communication with your GPS,
so connect your GPS TX and RX pin to any digital pin on the Arduino,
just be sure to define which pins you are using on the Arduino to
communicate with the GPS module.

REVISIONS: 1-17-11
changed values to RXPIN = 2 and TXPIN = to correspond with
hardware v14+. Hardware v13 used RXPIN = 3 and TXPIN = 2.

*/

const int chipSelect = 4;

// In order for this sketch to work, you will need to download
// TinyGPS and NewSoftSerial library from arduiniana.org and put them
// into the libraries folder in your ardiuno directory.
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <SD.h>


// Define which pins you will use on the Arduino to communicate with your
// GPS. In this case, the GPS module's TX pin will connect to the
// Arduino's RXPIN which is pin 3.
#define RXPIN 2
#define TXPIN 3

// This is the serial rate for your terminal program. It must be this
// fast because we need to print everything before a new sentence
// comes in. If you slow it down, the messages might not be valid and
// you will likely get checksum errors.
// Set this value equal to the baud rate of your terminal program
#define TERMBAUD 14400

// Set this value equal to the baud rate of your GPS
#define GPSBAUD 4800

// Create an instance of the TinyGPS object
TinyGPS gps;
// Initialize the NewSoftSer6ial library to the pins you defined above
SoftwareSerial uart_gps(RXPIN, TXPIN);

// This is where you declare prototypes for the functions that will be
// using the TinyGPS library.
String getgps(TinyGPS &gps);

// In the setup function, you need to initialize two serial ports; the
// standard hardware serial port (Serial()) to communicate with your
// terminal program an another serial port (NewSoftSerial()) for your
// GPS.
void setup()
{
  
 // Open serial communications and wait for port to open:
  Serial.begin(14400);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(8, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

// Sets baud rate of your terminal program
Serial.begin(TERMBAUD);
// Sets baud rate of your GPS
uart_gps.begin(GPSBAUD);

Serial.println("");
Serial.println("GPS Shield QuickStart Example Sketch v12");
Serial.println(" ...waiting for lock... ");
Serial.println("");
Serial.print("{");

}
int ctr = 0;
String dataString = "";
// This is the main loop of the code. All it does is check for data on
// the RX pin of the ardiuno, makes sure the data is valid NMEA sentences,
// then jumps to the getgps() function.
void loop()
{

while(uart_gps.available()) // While there is data on the RX pin...
  {
    
  int c = uart_gps.read(); // load the data into a variable...
  if(gps.encode(c)) // if there is a new valid sentence...
    {
      Serial.print("\"");
      Serial.print(ctr);
      Serial.print("\":");
      dataString += getgps(gps); // then grab the data.
    }
    
//     String dataString = "";

  // read three sensors and append to the string:
//  for (int analogPin = 0; analogPin < 3; analogPin++) {
//    int sensor = analogRead(analogPin);
//    dataString += String(sensor);
//    if (analogPin < 2) {
//      dataString += ","; 
//    }
//  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  } 
  
  }
}


// The getgps function will get and print the values we want.
String getgps(TinyGPS &gps)
{
// To get all of the data into varialbes that you can use in your code,
// all you need to do is define variables and query the object for the
// data. To see the complete list of functions see keywords.txt file in
// the TinyGPS and NewSoftSerial libs.
     ctr++;
// Define the variables that will be used
float latitude, longitude;
// Then call this function
gps.f_get_position(&latitude, &longitude);
// You can now print variables latitude and longitude
//Serial.print("Lat/Long: ");
Serial.print("{");
//Serial.print(latitude,5);
Serial.print("\"lat\": ");
Serial.print(latitude,5);
Serial.print(", \"long\": ");
Serial.print(longitude,5);

// Same goes for date and time
int year;
byte month, day, hour, minute, second, hundredths;
gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
// Print data and time
Serial.print(", \"date\": \""); Serial.print(year); Serial.print("/");
Serial.print(month, DEC); Serial.print("/"); Serial.print(day, DEC);
Serial.print("\", \"time\": \""); Serial.print(hour, DEC); Serial.print(":");
Serial.print(minute, DEC); Serial.print(":"); Serial.print(second, DEC);
Serial.print("."); Serial.print(hundredths, DEC); 
//Since month, day, hour, minute, second, and hundr
Serial.print("\", \"alt\": "); Serial.print(gps.f_altitude());  
Serial.print(", \"speed\": "); Serial.print(gps.f_speed_kmph()); 


Serial.println("},");


// Here you can print the altitude and course values directly since
// there is only one value for the function

// Here you can print statistics on the sentences.
unsigned long chars;
unsigned short sentences, failed_checksum;
gps.stats(&chars, &sentences, &failed_checksum);
//Serial.print("Failed Checksums: ");Serial.print(failed_checksum);
//Serial.println(); Serial.println();

// Here you can print the number of satellites in view
//Serial.print("Satellites: ");
//Serial.println(gps.satellites());

return "test";
}

