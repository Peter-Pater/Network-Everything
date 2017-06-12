#include <SPI.h>
#include <SD.h>

File dataFile;

boolean readData = true;
int data;
float t;
float pt = 0;
const int CS_SD = 4;

void setup() {
  Serial.begin(9600);
  if (!SD.begin(CS_SD)) {
    Serial.println("initialization failed!");
    return;
  }
  //setting time out for the possble endding
  stream.setTimeout(10);
  // re-open the file for reading:
  dataFile = SD.open("heart.txt");
  establishContact();
}

void loop() {
  int command = 2;
  if (dataFile.available() > 0) {
    if (readData == true) {
      data = dataFile.parseInt();
      readData = false;
    }
    if (millis() - pt > data) {
      command = 1;
      pt = millis();
      readData = true;
    }
  } else {
    dataFile.close();
  }
  if (Serial.available() > 0) {
    Serial.print(command);
    Serial.print(",");
    Serial.println(data);
  }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("0");   // send an initial string
    delay(300);
  }
    pt = millis();  
}
