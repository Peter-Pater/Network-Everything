#include <SPI.h>
#include <WiFi101.h>
#include <WiFiSSLClient.h>
#include <TembooSSL.h>
#include "TembooAccount.h" // Contains Temboo account information

WiFiSSLClient client;

int calls = 1;   // Execution count, so this doesn't run forever
int maxCalls = 10;   // Maximum number of times the Choreo should be executed

//temperature parameters
int Temp[5];
float temp;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);//blue
  pinMode(3, OUTPUT);//yellow
  pinMode(4, OUTPUT);//red
  pinMode(5, OUTPUT);//fan
  //   For debugging, wait until the serial console is connected
  delay(4000);
  while (!Serial);

  int wifiStatus = WL_IDLE_STATUS;

  // Determine if the WiFi Shield is present
  Serial.print("\n\nShield:");
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("FAIL");

    // If there's no WiFi shield, stop here
    while (true);
  }

  Serial.println("OK");

  // Try to connect to the local WiFi network
  while (wifiStatus != WL_CONNECTED) {
    Serial.print("WiFi:");
    wifiStatus = WiFi.begin(WIFI_SSID, WPA_PASSWORD);

    if (wifiStatus == WL_CONNECTED) {
      Serial.println("OK");
    } else {
      Serial.println("FAIL");
    }
    delay(5000);
  }

  Serial.println("Setup complete.\n");
}

void loop() {
  if (calls <= maxCalls) {
    Serial.println("Running GetTemperature - Run #" + String(calls++));

    TembooChoreoSSL GetTemperatureChoreo(client);

    // Invoke the Temboo client
    GetTemperatureChoreo.begin();
    Serial.println(1);

    // Set Temboo account credentials
    GetTemperatureChoreo.setAccountName(TEMBOO_ACCOUNT);
    Serial.println(2);
    GetTemperatureChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    Serial.println(3);
    GetTemperatureChoreo.setAppKey(TEMBOO_APP_KEY);
    Serial.println(4);
    GetTemperatureChoreo.setDeviceType(TEMBOO_DEVICE_TYPE);
    Serial.println(5);

    // Set Choreo inputs
    String AddressValue = "Shanghai";
    GetTemperatureChoreo.addInput("Address", AddressValue);
    Serial.println(6);

    // Identify the Choreo to run
    GetTemperatureChoreo.setChoreo("/Library/Yahoo/Weather/GetTemperature");
    Serial.println(7);

    // Run the Choreo; when results are available, print them to serial
    GetTemperatureChoreo.run();
    Serial.println(8);
    int i = 0;
    while (GetTemperatureChoreo.available()) {
      char c = GetTemperatureChoreo.read();
      Serial.print(c);
      if (c >= '0' && c <= '9') {
        Temp[i] = c - '0';
        i++;
      }
    }
    Serial.println("The info we parsed is:");
    if (Temp[0] == 0) {
      Serial.println("You have lost the connection");
      //faking temperature over 20 degrees
      digitalWrite(3, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(2, LOW);
    } else {
      temp = Temp[3] * 10 + Temp[4];
      temp = (temp - 32) / 1.8;
      Serial.println(temp);
      if (temp > 30) {
        digitalWrite(4, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(2, LOW);
      }
      else if (temp > 20) {
        digitalWrite(3, HIGH);
        digitalWrite(4, LOW);
        digitalWrite(2, LOW);
        digitalWrite(5, HIGH);
      }
      else if (temp > 10) {
        digitalWrite(2, HIGH);
        digitalWrite(4, LOW);
        digitalWrite(3, LOW);
        digitalWrite(5, HIGH);
      } else {
        digitalWrite(4, LOW);
        digitalWrite(3, LOW);
        digitalWrite(2, LOW);
      }
    }
    GetTemperatureChoreo.close();
  }

  Serial.println("\nWaiting...\n");
  delay(30000); // wait 30 seconds between GetTemperature calls
}
