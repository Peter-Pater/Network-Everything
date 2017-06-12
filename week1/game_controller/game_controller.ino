const int led1 = 2;
const int led2 = 3;
const int led3 = 4;
const int led4 = 5;

void setup()
{
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
}

void loop()
{
  int sensorValue1 = analogRead(A0);
  int sensorValue2 = analogRead(A1);
  int sensorValue3 = analogRead(A2);
  sensorValue1 = map(sensorValue1, 170, 500, 0, 255);
  sensorValue2 = map(sensorValue2, 170, 500, 0, 255);
  sensorValue3 = constrain(sensorValue3, 0, 500);
  sensorValue3 = map(sensorValue3, 0, 500, 0, 255);
  //    Serial.println(DEC);
  Serial.print("The X and Y coordinate is:");
  Serial.print(sensorValue1, DEC);
  Serial.print(",");
  Serial.println(sensorValue2, DEC);
  Serial.print("The Speed level is:");
  Serial.println(sensorValue3);
  Serial.println("-------------------------------------------");
  delay(200);

  if (sensorValue3 >= 3 && sensorValue3 <= 64) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
  } else if (sensorValue3 >= 65 && sensorValue3 <= 128) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
  } else if (sensorValue3 >= 129 && sensorValue3 <= 192) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, LOW);
  } else if (sensorValue3 >= 193 && sensorValue3 <= 256) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
  } else {
    delay(100);
    digitalWrite(led4, LOW);
    delay(100);
    digitalWrite(led3, LOW);
    delay(100);
    digitalWrite(led2, LOW);
    delay(100);
    digitalWrite(led1, LOW);
  }
}

