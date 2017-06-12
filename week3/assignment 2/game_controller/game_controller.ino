const int led1 = 2;
const int led2 = 3;
const int led3 = 4;
const int led4 = 5;
const int led5 = 8;
const int led6 = 9;
const int led7 = 10;
const int led8 = 11;
int FSR;
int FSR2;
unsigned long currentMillis;
unsigned long previousMillis1;
unsigned long previousMillis2;
unsigned long previousMillis3;
unsigned long previousMillis4;
unsigned long previousMillis5;
unsigned long previousMillis6;
unsigned long previousMillis7;
unsigned long previousMillis8;
const long interval = 100;

void setup()
{
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
  establishContact();
}

void loop()
{
  delay(400);
  int sensorValue1 = analogRead(A0);
  int sensorValue2 = analogRead(A1);
  int sensorValue3 = analogRead(A2);

  int sensorValue4 = analogRead(A3);
  int sensorValue5 = analogRead(A4);
  int sensorValue6 = analogRead(A5);

  // Serial.println(sensorValue1);
  // Serial.println(sensorValue2);
  if (sensorValue1 < 200) {
    sensorValue1 = 1;
  }
  else if (sensorValue1 > 400) {
    sensorValue1 = -1;
  }
  else if (sensorValue1 <= 400 && sensorValue1 >= 200) {
    sensorValue1 = 0;
  }

  if (sensorValue2 < 200) {
    sensorValue2 = 1;
  }
  else if (sensorValue2 > 400) {
    sensorValue2 = -1;
  }
  else if (sensorValue2 <= 400 && sensorValue2 >= 200) {
    sensorValue2 = 0;
  }


  if (sensorValue4 < 200) {
    sensorValue4 = 1;
  }
  else if (sensorValue4 > 400) {
    sensorValue4 = -1;
  }
  else if (sensorValue4 <= 400 && sensorValue4 >= 200) {
    sensorValue4 = 0;
  }

  if (sensorValue5 < 200) {
    sensorValue5 = 1;
  }
  else if (sensorValue5 > 400) {
    sensorValue5 = -1;
  }
  else if (sensorValue5 <= 400 && sensorValue5 >= 200) {
    sensorValue5 = 0;
  }

  sensorValue3 = constrain(sensorValue3, 0, 500);
  sensorValue3 = map(sensorValue3, 0, 500, 0, 255);

  sensorValue6 = constrain(sensorValue6, 0, 500);
  sensorValue6 = map(sensorValue6, 0, 500, 0, 255);

  if (sensorValue3 >= 35 && sensorValue3 <= 64) {
    FSR = 2;
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
  } else if (sensorValue3 >= 65 && sensorValue3 <= 128) {
    FSR = 3;
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
  } else if (sensorValue3 >= 129 && sensorValue3 <= 192) {
    FSR = 4;
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, LOW);
  } else if (sensorValue3 >= 193 && sensorValue3 <= 256) {
    FSR = 5;
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
  } else {
    FSR = 1;
    light_out1();
  }

  if (sensorValue6 >= 35 && sensorValue6 <= 64) {
    FSR2 = 2;
    digitalWrite(led5, HIGH);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);
  } else if (sensorValue6 >= 65 && sensorValue6 <= 128) {
    FSR2 = 3;
    digitalWrite(led5, HIGH);
    digitalWrite(led6, HIGH);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);
  } else if (sensorValue6 >= 129 && sensorValue6 <= 192) {
    FSR2 = 4;
    digitalWrite(led5, HIGH);
    digitalWrite(led6, HIGH);
    digitalWrite(led7, HIGH);
    digitalWrite(led8, LOW);
  } else if (sensorValue6 >= 193 && sensorValue6 <= 256) {
    FSR2 = 5;
    digitalWrite(led5, HIGH);
    digitalWrite(led6, HIGH);
    digitalWrite(led7, HIGH);
    digitalWrite(led8, HIGH);
  } else {
    FSR2 = 1;
    light_out2();
  }

  if (Serial.available() > 0) {
    //  Serial.print("The X and Y coordinate is:");
    Serial.print(sensorValue1, DEC);
    Serial.print(",");
    Serial.print(sensorValue2, DEC);
    Serial.print(",");
    //  Serial.print("The Speed level is:");
    Serial.print(FSR);
    Serial.print(",");

    Serial.print(sensorValue4, DEC);
    Serial.print(",");
    Serial.print(sensorValue5, DEC);
    Serial.print(",");
    //  Serial.print("The Speed level is:");
    Serial.println(FSR2);
  }
}

void light_out1() {
  currentMillis = millis();
  if (currentMillis - previousMillis4 >= interval) {
    digitalWrite(led4, LOW);
    previousMillis4 = currentMillis;
  }
  if (currentMillis - previousMillis3 >= interval * 2) {
    digitalWrite(led3, LOW);
    previousMillis3 = currentMillis;
  }
  if (currentMillis - previousMillis2 >= interval * 3) {
    digitalWrite(led2, LOW);
    previousMillis2 = currentMillis;
  }
  if (currentMillis - previousMillis1 >= interval * 4) {
    digitalWrite(led1, LOW);
    previousMillis1 = currentMillis;
  }
}
void light_out2() {
  currentMillis = millis();
  if (currentMillis - previousMillis8 >= interval) {
    digitalWrite(led8, LOW);
    previousMillis4 = currentMillis;
  }
  if (currentMillis - previousMillis7 >= interval * 2) {
    digitalWrite(led7, LOW);
    previousMillis3 = currentMillis;
  }
  if (currentMillis - previousMillis6 >= interval * 3) {
    digitalWrite(led6, LOW);
    previousMillis2 = currentMillis;
  }
  if (currentMillis - previousMillis5 >= interval * 4) {
    digitalWrite(led5, LOW);
    previousMillis1 = currentMillis;
  }


}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("0,0,0");   // send an initial string
    delay(300);
  }
}


