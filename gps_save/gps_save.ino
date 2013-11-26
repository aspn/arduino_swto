#include <SD.h>

/*
-----------------------------
Written by Jonnyboy323
Version 1.0
GPS Data Logger
10/17/11
-----------------------------
*/

//Add the Mikal Hart Libraries
#include <TinyGPS.h>

//Add the SdFat Libraries
#include <SdFat.h>
#include <SdFatUtil.h> 
#include <ctype.h>
#include <SoftwareSerial.h>

#define RXPIN 2
#define TXPIN 3

#define LEDPIN 13

#define TERMBAUD 14400

#define GPSBAUD 4800

TinyGPS gps;
SoftwareSerial uart_gps(RXPIN, TXPIN);

//Create the variables to be used by SdFat Library
Sd2Card card;
SdVolume volume;
SdFile root;
SdFile file;

#define BUFFSIZE 65
char buffer[BUFFSIZE]; //This will be a data buffer for writing contents to the file.
char in_char=0;
char filename[]="trip.txt";

//Intialize TinyGPS and NewSoftSerial

//Intialize GPS variables
long lat, lon;
float flat, flon, fspeed, falt, fcourse;
unsigned long age, date, time, chars;
int year;
byte month, day, hour, minute, second, hundredths;
unsigned short sentences, failed;

bool newdata = false;
bool feedgps(); 

void setup()
{
  
  pinMode(LEDPIN, OUTPUT);
 
  pinMode(8, OUTPUT); 
  
  digitalWrite(LEDPIN, HIGH);
  
  card.init();            //Initialize the SD card and configure the I/O pins.
  volume.init(card);      //Initialize a volume on the SD card.
  root.openRoot(volume);  //Open the root directory in the volume. 

  Serial.begin(TERMBAUD);
  Serial.print("Initializing SD card...");
  
  //Create file with defined filename
  file.open(root, filename, O_CREAT | O_APPEND | O_WRITE);    //Open or create the file 'name' in 'root' for writing to the end of the file
//  file.print("Y/M/D, H:M:S, latitude, longitude, altitude(cm), speed(kmh)\n");    //Write the header array to the end of the file.
  file.close(); //Close the file.
  
  uart_gps.begin(GPSBAUD);
  Serial.println("");
  Serial.println("GPS Shield v12");
  Serial.println(" ...waiting for lock... ");
  Serial.println("");
  Serial.print("{");
  
  // flash LED 5 times when setup is finished
  /*
  for (int i=0; i<4; i++){    
    digitalWrite(13, HIGH);   // set the LED on
    delay(1000);              // wait for a second
    digitalWrite(13, LOW);    // set the LED off 
  }
*/  
}

void loop()
{
  bool newdata = false; // check if data is coming in
  while(uart_gps.available())     // While there is data on the RX pin...
  {
      int c = uart_gps.read();    // load the data into a variable...
      if(gps.encode(c))      // if there is a new valid sentence...
      {
        newdata = true;         // then grab the data.
      }
  }
    
  if (newdata)
  {     
    //Pull gps data
    gps.f_get_position(&flat, &flon, &age);
    feedgps(); //used to keep gps "awake"
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
    feedgps();
    falt = gps.f_altitude();
    fspeed = gps.f_speed_kmph();
    feedgps();
    
    //Break up float values into whole numbers(var) and decimals(var1) to be added to data
    int flat1 = (flat - (int)flat) * 10000;
    int flon1 = (flon - (int)flon) * 100000;
    int alt1 = 0;
    if (falt >= 10000000.00){ //when gps cant get altitude
      falt = 0; //set to 0         
    }else{
      alt1 = (falt - (int)falt) * 10;
    } 
    int fspeed1 = (fspeed - (int)fspeed) * 10;

    //Create then write the char array "buffer" to the end of the file
    file.open(root, filename, O_WRITE | O_APPEND);  //Open the file in write mode and append the data to the end of the file.
    sprintf(buffer, "\"03\":{\"date\": \"%d/%d/%d\", \"time\": \"%d:%d:%d\", \"lat\": %0d.%d, \"long\": %0d.%d, \"alt\": %0d.%d, \"speed\": %0d.%d}, \n", year,month,day,hour,minute,second,(int)flat,abs(flat1),(int)flon,abs(flon1),(int)falt,abs(alt1),(int)fspeed,abs(fspeed1));
    file.print(buffer); //Write the 'contents' array to the end of the file.
    Serial.println("writing");

delay(1000);
digitalWrite(9, HIGH);
delay(1000);
digitalWrite(9, LOW);
delay(1000);

  float latitude, longitude;
  // Then call this function
  gps.f_get_position(&latitude, &longitude);
  // You can now print variables latitude and longitude
  Serial.print("Lat/Long: "); 
  Serial.print(latitude,5); 
  Serial.print(", "); 
  Serial.println(longitude,5);
  
  // Same goes for date and time
  int year;
  byte month, day, hour, minute, second, hundredths;
  gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
  // Print data and time
  Serial.print("Date: "); Serial.print(month, DEC); Serial.print("/"); 
  Serial.print(day, DEC); Serial.print("/"); Serial.print(year);
  Serial.print("  Time: "); Serial.print(hour, DEC); Serial.print(":"); 
  Serial.print(minute, DEC); Serial.print(":"); Serial.print(second, DEC); 
  Serial.print("."); Serial.println(hundredths, DEC);
  //Since month, day, hour, minute, second, and hundr
  
  // Here you can print the altitude and course values directly since 
  // there is only one value for the function
  Serial.print("Altitude (meters): "); Serial.println(gps.f_altitude());  
  // Same goes for course
  Serial.print("Course (degrees): "); Serial.println(gps.f_course()); 
  // And same goes for speed
  Serial.print("Speed(kmph): "); Serial.println(gps.f_speed_kmph());
    
    file.close();  //Close the file
  }
}

bool feedgps()
{
    return true;
}

