/*
  Class: Network Everthing
  Project: Missing stuff finder(Final)
  @author: Xincheng Huang(Peter), and Yaming Xu(Diana)
  This part of the code is for the stuff tracker running on Xbees.
  Each Xbee has a uique id(set to 0 in this one), used for encode
  and decode, thus building the one to many communication.
  The code also implements functions for music.
  The code is based on state-machine algorithm, which Diana shows
  a lot of hatred, LOL
*/

const int BuzzerPin = 9;
const int buttonPin = 8;

//UNO   CODE
// 32    0
// 58    1
const int self_code = 0;
//The self_code : 0, 1, 2
char incoming_command = '0';
char outgoing_command;

const int M_SILENCE = 0;
const int M_ALARMING = 1;

int STATE = M_SILENCE;

char notes[] = "CCGGAAG FFEEDDC GGFFEED GGFFEED CCGGAAG FFEEDDC ";
int beats[] = {8, 8, 8, 8, 8, 8, 16, 1, 8, 8, 8, 8, 8, 8, 16, 1,
               8, 8, 8, 8, 8, 8, 16, 1, 8, 8, 8, 8, 8, 8, 16, 1,
               8, 8, 8, 8, 8, 8, 16, 1, 8, 8, 8, 8, 8, 8, 16, 1
              };

int notesLength = 48; // the number of notes
int tempo = 150;

void setup() {
  Serial.begin(9600);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(13, OUTPUT);
  //The button which turns the alarm off
  pinMode(buttonPin, INPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
}

void loop() {
  if (Serial.available() > 0) {
    incoming_command = Serial.read() - self_code;
  } else {
    incoming_command = '0';
  }

  if (STATE == M_SILENCE) {
    if (incoming_command == 'H') {
      digitalWrite(13, HIGH);
      digitalWrite(BuzzerPin, HIGH);
      STATE = M_ALARMING;
    }
  }

  if (STATE == M_ALARMING) {
    alarm();
  }
}

void alarm() {
  for (int i = 0; i < notesLength; i++) {
    while (Serial.available() > 0) {
      incoming_command = Serial.read();
      if (incoming_command == 'Z'){
          break;
      }
    }
    if (digitalRead(buttonPin) == HIGH) {
      digitalWrite(BuzzerPin, LOW);
      digitalWrite(13, LOW);
      int counter = 100;
      while (counter > 0) {
        counter--;
        outgoing_command = self_code + 'L';
        Serial.println(outgoing_command);
        delay(10);
      }
      STATE = M_SILENCE;
      break;
    }
    if (incoming_command == 'Z') {
      digitalWrite(BuzzerPin, LOW);
      digitalWrite(13, LOW);
      STATE = M_SILENCE;
      break;
    }
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }
    delay(tempo);
  }
}

void playNote(char note, int duration) {
  char names[] = {'C', 'D', 'E', 'F', 'G', 'A', 'B',
                  'c', 'd', 'e', 'f', 'g', 'a', 'b',
                  'x', 'y'
                 };

  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014,
                  956,  834,  765,  593,  468,  346,  224,
                  655 , 715
                };
  int SPEE = 5;
  // play the tone corresponding to the note name
  for (int i = 0; i < 17; i++) {
    if (names[i] == note) {
      int newduration = duration / SPEE;
      playTone(tones[i], newduration);
    }
  }
}


void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(BuzzerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(BuzzerPin, LOW);
    delayMicroseconds(tone);
  }
}
