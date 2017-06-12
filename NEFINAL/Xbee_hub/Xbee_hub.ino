/*
  Class: Network Everthing
  Project: Missing stuff finder(Final)
  @author: Xincheng Huang(Peter), and Yaming Xu(Diana)
  This part of the code is for the Xbee hub.
  Each Xbee has a uique id(set to 0 in this one), used for encode
  and decode, thus building the one to many communication, the 
  ids are recorded in this piece of code.
  The code also implements functions I2C communication with the
  Ethernet shield.
  The code is based on state-machine algorithm, which Diana shows
  a lot of hatred, LOL
*/

#include <Wire.h>

const int buttonA = 8;
const int buttonB = 10;

const int peer_codeA = 0;
const int peer_codeB = 1;
const int peer_codeC = 2;

const int M_SILENCE = 0;
const int M_ALARMING = 1;

int PEER_STATEA = M_SILENCE;
int PEER_STATEB = M_SILENCE;
int PEER_STATEC = M_SILENCE;

char incoming_command = '0';
char outgoing_command;
int master_command = 4;


void setup() {
  // put your setup code here, to run once:
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600); // start serial
  pinMode(13, OUTPUT);
}

void loop() {
  serial_control();
  peerA();
  peerB();
  peerC();
  halt();
}

void serial_control() {
  if (Serial.available() > 0) {
    incoming_command = Serial.read();
  } else {
    incoming_command = '0';
  }
}

void peerA() {
  //controls Xbee on UNO 32
  if (PEER_STATEA == M_SILENCE) {
    if (master_command == 0) {
      outgoing_command = peer_codeA + 'H';
      int counter = 100;
      while (counter > 0) {
        Serial.println(outgoing_command);
        delay(10);
        counter--;
      }
      PEER_STATEA = M_ALARMING;
      master_command = 4;
      digitalWrite(13, LOW);
    }
  }
  else if (PEER_STATEA == M_ALARMING) {
    if ((char) (incoming_command - peer_codeA) == 'L') {
      PEER_STATEA = M_SILENCE;
      //restore the web page accordingly
    }
  }
}

void peerB() {
  //controls Xbee on UNO 58
  if (PEER_STATEB == M_SILENCE) {
    if (master_command == 1) {
      outgoing_command = peer_codeB + 'H';
      int counter = 100;
      while (counter > 0) {
        Serial.println(outgoing_command);
        delay(10);
        counter--;
      }
      PEER_STATEB = M_ALARMING;
      master_command = 4;
      digitalWrite(13, LOW);
    }
  }
  else if (PEER_STATEB == M_ALARMING) {
    if (Serial.available() > 0) {
      incoming_command = Serial.read() - peer_codeB;
    }
    if ((char) (incoming_command - peer_codeB) == 'L') {
      PEER_STATEB = M_SILENCE;
      //restore the web page accordingly
    }
  }
}

void peerC() {
  if (PEER_STATEC == M_SILENCE) {
    if (master_command == 2) {
      outgoing_command = peer_codeC + 'H';
      int counter = 100;
      while (counter > 0) {
        Serial.println(outgoing_command);
        delay(10);
        counter--;
      }
      PEER_STATEC = M_ALARMING;
      master_command = 4;
      digitalWrite(13, LOW);
    }
  }
  else if (PEER_STATEC == M_ALARMING) {
    if (Serial.available() > 0) {
      incoming_command = Serial.read() - peer_codeC;
    }
    if ((char) (incoming_command - peer_codeC) == 'L') {
      PEER_STATEC = M_SILENCE;
      //restore the web page accordingly
    }
  }
}

void halt(){
  if (master_command == 3){
      outgoing_command = 'Z';
      int counter = 100;
      while(counter > 0){
        Serial.println(outgoing_command);
        delay(10);
        counter--;
      }
      PEER_STATEA = M_SILENCE;
      PEER_STATEB = M_SILENCE;
      PEER_STATEC = M_SILENCE;
      master_command == 4;
  }
}

void receiveEvent(int howMany){
  while (Wire.available() > 0){
    master_command = Wire.read();
    digitalWrite(13, HIGH);
  }
}
