/*Network Everything midterm
  Three servos controlled by three piezo disk vibration sensors.
  Each time when one of the sensors gets hit by the ball, it would
  activate the servo it corresponds to, and opens a gate to let the
  bal move forward.
  @author: Xincheng Huang (Peter)
  Date: Mar 18th, 2017
  Credit to Piezo sample code from DFRduino, and arduino sample code
  from Servo library
*/

#include <Servo.h>
#include <Wire.h>
#include "pitches.h"

//switch to true on the previous group's signal
boolean trigger = false;
boolean play1 = true;
boolean play2 = true;
boolean play3 = true;


//The incoming Byte. No data: -1; Standing by: other data; Triggered: 1.
int start = -1;

//The Byte to send
String send_msg = "0";

//servo ports
int servoPort1 = 9;  //waits for the command from the previous group
int servoPort2 = 10; //piezo1
int servoPort3 = 11; //piezo2

//piezo ports
int piezoPort1 = 0; //servo2
int piezoPort2 = 1; //servo3
int piezoPort3 = 2; //send command to the subsequent group

//piezo values
int piezoValue1 = 0;
int piezoValue2 = 0;
int piezoValue3 = 0;

//gate state control
boolean gate1 = false;
boolean gate2 = false;
boolean gate3 = false;

//Servo value
int servoValue1 = 90;
int servoValue2 = 90;
int servoValue3 = 0;

//Music notes
int melody1[] = {
  NOTE_D4, NOTE_D4, NOTE_D4, NOTE_C7
};

int noteDurations1[] = {
  4, 4, 4, 2
};

int melody2[] = {
  NOTE_B2, NOTE_C3, NOTE_C5,
  NOTE_G6, NOTE_C7, NOTE_F4,
};

int noteDurations2[] = {
  8, 4, 8, 4, 4
};

int melody3[] = {
  NOTE_B2, NOTE_C3, NOTE_C5,
  NOTE_G6, NOTE_C7, NOTE_F7,
};

int noteDurations3[] = {
  8, 4, 8, 4, 4
};

Servo servo1;
Servo servo2;
Servo servo3;

void setup() {
  pinMode(12, OUTPUT);
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  servo1.attach(servoPort1);
  servo2.attach(servoPort2);
  servo3.attach(servoPort3);
  servoInitiate();
  delay(2000);
}

void loop() {
//  Serial.println(start);
  //  if (trigger == false) {
  //Serial.println("standing by...");
  //  } else if (trigger == true) {
  //Serial.println("start");
  piezoValue1 = analogRead(piezoPort1);
  piezoValue2 = analogRead(piezoPort2);
  piezoValue3 = analogRead(piezoPort3);
  gatesStates();
  gatesOpen();
  sender();
  //  Serial.print(piezoValue1, DEC);
  //  Serial.print(" ");
  //  Serial.print(piezoValue2, DEC);
  //  Serial.print(" ");
  //  Serial.println(piezoValue3, DEC);
  //    Serial.println(servoValue1, DEC);
}

void servoInitiate() {
  servo1.write(90);
  servo2.write(90);
  servo3.write(0);
  //  servo4.write(0);
}

void gatesStates() {
  //sensor value pending revision
  if (trigger == true) {
    if (play1 == true) {
      music(1);
      play1 = false;
    }
    gate1 = true;
  }
  if (piezoValue1 > 50 && gate1 == true) {
    if (play2 == true) {
      music(2);
      play2 = false;
    }
    gate2 = true;
  }
  if (piezoValue2 > 50 && gate2 == true) {
    if (play3 == true) {
      music(3);
      play3 = false;
    }
    gate3 = true;
  }
}

void gatesOpen() {
  if (gate1 == true) {
    if (servoValue1 > 0) {
      servoValue1--;
      servo1.write(servoValue1);
    }
  }
  if (gate2 == true) {
    if (servoValue2 > 0) {
      servoValue2--;
      servo2.write(servoValue2);
    }
  }
  if (gate3 == true) {
    if (servoValue3 < 90) {
      servoValue3++;
      servo3.write(servoValue3);
    }
  }
}

void receiveEvent(int howMany) {
  if (Wire.available() > 0) { // loop through all but the last
    start = Wire.read();
  }
  if (start == 1) {
    trigger = true;
  }
}

void sender() {
  if (piezoValue3 > 50) {
    send_msg = "1";
  }
  Serial.println(send_msg);
  if (send_msg == "1") {
    digitalWrite(12, HIGH);
  }
}

void music(int i) {
  if (i == 1) {
    for (int thisNote = 0; thisNote < 4; thisNote++) {

      // to calculate the note duration, take one seco
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations1[thisNote];
      tone(8, melody1[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(8);
    }
  }
  if (i == 2) {
    for (int thisNote = 0; thisNote < 6; thisNote++) {

      // to calculate the note duration, take one seco
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations2[thisNote];
      tone(8, melody2[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(8);
    }
  }
  if (i == 3) {
    for (int thisNote = 0; thisNote < 6; thisNote++) {

      // to calculate the note duration, take one seco
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations3[thisNote];
      tone(8, melody3[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(8);
    }
  }
}
