
#include <SD.h>
#include <SdFat.h>
#include <SdFatUtil.h> 

Sd2Card card;
SdVolume volume;
SdFile root;
SdFile file;

#define BUFFSIZE 65
char buffer[BUFFSIZE]; //This will be a data buffer for writing contents to the file.
char in_char=0;
char filename[]="test.txt";

void setup()
{
  Serial.begin(9600);
  card.init();            //Initialize the SD card and configure the I/O pins.
  volume.init(card);      //Initialize a volume on the SD card.
  root.openRoot(volume);  //Open the root directory in the volume.

  file.open(root, filename, O_CREAT | O_APPEND | O_WRITE);  
  file.close();
  
  Serial.println("kdone");
} 

void loop()
{
  
  file.open(root, filename, O_WRITE | O_APPEND);
  sprintf(buffer, "%d", "hello world");
  file.print(buffer);
  
  Serial.println("k");
  file.close();
}
