// Function: This program can be used to measure heart rate, the lowest pulse in the program be set to 30.
//         Use an external interrupt to measure it.
// Hardware: Grove - Ear-clip Heart Rate Sensor, Grove - Base Shield, Grove - LED
// Arduino IDE: Arduino-1.0
// Author: FrankieChu
// Date: Jan 22, 2013
// Version: v1.0
// by www.seeedstudio.com
#include <SPI.h>
#include <SD.h>

#define LED 4//indicator, Grove - LED is connected with D4 of Arduino
boolean led_state = LOW;//state of LED, each time an external interrupt
//will change the state of LED
unsigned char counter;
unsigned long temp[21];
unsigned long sub;
bool data_effect = true;
unsigned int heart_rate;//the measurement result of heart rate

const int CS_SD = 4;
const int max_heartpluse_duty = 2000;//you can change it follow your system's request.
//2000 meams 2 seconds. System return error
//if the duty overtrip 2 second.
File dataFile;

void setup()
{
  // Heart rate code
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Serial.println("Please ready your chest belt.");
  delay(5000);
  arrayInit();
  Serial.println("Heart rate test begin.");
  attachInterrupt(0, interrupt, RISING);//set interrupt 0,digital port 2
  if (!SD.begin(CS_SD)) {
    Serial.println("card failure");
    return;
  }
  Serial.println("card initialized!");
  SD_initialize();
}
void loop()
{
  digitalWrite(LED, led_state);//Update the state of the indicator
}
/*Function: calculate the heart rate*/
void sum()
{
  if (data_effect)
  {
    heart_rate = 1200000 / (temp[20] - temp[0]); //60*20*1000/20_total_time
    Serial.print("Heart_rate_is:\t");
    Serial.println(heart_rate);
  }
  data_effect = 1; //sign bit
}
/*Function: Interrupt service routine.Get the sigal from the external interrupt*/
void interrupt()
{
  temp[counter] = millis();
  Serial.println(counter, DEC);
  Serial.println(temp[counter]);
  switch (counter)
  {
    case 0:
      sub = temp[counter] - temp[20];
      if (sub < max_heartpluse_duty && sub > 0) {
        dataFile.println(sub);
        Serial.println(sub);
      }
      break;
    default:
      sub = temp[counter] - temp[counter - 1];
      if (sub < max_heartpluse_duty && sub > 0) {
        dataFile.println(sub);
        Serial.println(sub);
      }
      break;
  }
  if (sub > max_heartpluse_duty) //set 2 seconds as max heart pluse duty
  {
    data_effect = 0; //sign bit
    counter = 0;
    Serial.println("Heart rate measure error,test will restart!" );
    dataFile.close();
    arrayInit();
    SD_initialize();
  }
  if (counter == 20 && data_effect)
  {
    counter = 0;
    sum();
  }
  else if (counter != 20 && data_effect)
    counter++;
  else
  {
    counter = 0;
    data_effect = 1;
  }

}
/*Function: Initialization for the array(temp)*/
void arrayInit()
{
  for (unsigned char i = 0; i < 20; i ++)
  {
    temp[i] = 0;
  }
  temp[20] = millis();
}

void SD_initialize() {
  //Serial.println("card initialized");
  dataFile = SD.open("heart.txt", FILE_WRITE);
  //dataFile.println("start!");
}
