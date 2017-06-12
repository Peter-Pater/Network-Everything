/*
  Class: Network Everthing
  Project: Missing stuff finder(Final)
  @author: Xincheng Huang(Peter), and Yaming Xu(Diana)
  This part of the code has three functions:
  1. Serve the website, and enable server-client communication
  2. Track username and user operation, send to laptop, allow API to work
  3. Report user operation to Xbee hub, and start finding!
*/
#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
byte data;
byte twitter_control;


// MAC address from Ethernet shield sticker under board
byte mac[] = { 0xAE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 20); // IP address, may need to change depending on network
EthernetServer server(80);  // create a server at port 80

String HTTP_request;          
boolean XBee_32_Status = 0;   
boolean XBee_58_Status = 0;   
boolean XBee_dd_Status = 0;   
boolean found = 0;
int prev = -1;
int page = 0;
String username;

void setup()
{
  Serial.begin(9600);       // for diagnostics
  Wire.begin();
  Ethernet.begin(mac, ip);  // initialize Ethernet device
  server.begin();           // start to listen for clients
}

void loop()
{
  EthernetClient client = server.available();  // try to get client

  if (client) {  // is a client present?
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {   // client data available to read
        char c = client.read(); // read 1 byte (character) from client
        if (HTTP_request.length() < 50) {
          HTTP_request += c;
        }
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE html>");
          client.println("<html>");
          client.println("<body style =\"background-color:#f1e8ca;width:100%;position:absolute;text-align:center;color:#745151;\">");
          //FIRST TIME ROUND, LOGIN PAGE
          if (page == 0) {
            ProcessSubmit(client);
          }
          //AFTER THAT, FINDER PAGE
          //FOR SD CARD ATTEMPTS, THE PARTS WHICH SHOULD READ FROM SD CARD SHOULD START FROM HERE
          if (page == 1) {
            client.println("<br><br><br><br><br><h1>Hi, " + username + "! Welcome to Your Stuff Finder!</h1>");
            client.println("<br><h2>Click to find what you've lost.</h3 >");
            client.println("<style> button{width: 300px;height:150px;margin:200px 20px 20px;font-size:16pt;color:#745151} </style>");
            client.println("<br><a href= \"/?32XBee\"><button>BEAR</button></a >");
            client.println("<a href=\"/?58XBee\"><button>CHARGER</button></a ></div>");
            client.println("<a href=\"/?ddXBee\"><button>KEY</button></a >");
            client.println("<a href=\"/?Giveup\"><button>GIVEUP</button></a >");
            client.println("</body>");
            client.println("</html>");
            ProcessCheckbox1();
            ProcessCheckbox2();
            ProcessCheckbox3();
            ProcessCheckbox4();
          }

          HTTP_request = "";
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
  }
}


void ProcessCheckbox1()
{
  if (HTTP_request.indexOf("?32XBee") > -1) {  // see if checkbox was clicked
    data = 0;
    for (int i = 0; i < 5; i++) {
      wire_transmission();
    }
    twitter_control = 0;
    Serial.println(twitter_control + username);
  }
}


void ProcessCheckbox2()
{
  if (HTTP_request.indexOf("?58XBee") > -1 ) {
    data = 1;
    for (int i = 0; i < 5; i++) {
      wire_transmission();
    }
    twitter_control = 1;
    Serial.println(twitter_control + username);
  }
}


void ProcessCheckbox3()
{
  if (HTTP_request.indexOf("?ddXBee") > -1 ) {
    data = 2;
    for (int i = 0; i < 5; i++) {
      wire_transmission();
    }
    twitter_control = 2;
    Serial.println(twitter_control + username);
  }
}



void ProcessCheckbox4()
{
  if (HTTP_request.indexOf("?Giveup") > -1) {
    data = 3;
    for (int i = 0; i < 5; i++) {
      wire_transmission();
    }
  }
}

void ProcessSubmit(EthernetClient client) {
  if (HTTP_request.indexOf("?Username") > -1) {
    int from = HTTP_request.indexOf("Username") + 9;
    int to = HTTP_request.indexOf("&button");
    username = HTTP_request.substring(from, to);
    Serial.println(username);
    page = 1;
  } else {
    //I USED THE FORM ELEMENT HERE TO UPLOAD INFO TO SERVER,
    //SINCE THERE IS AN INPUT BOX, SO THIS MIGHT BE THE ONLY WAY
    //FORTUNATELY IT WORKS WELL THIS TIME
    client.println("<form align=\"center\" style=\"margin-top: 20%; font-size: 16pt;\"\" method=\"get\">");
    client.println("<p style=\"font-size: 16pt;\">Please input your name:</p>");
    client.println("<input name=\"Username\" style=\"font-size:16pt;\" value=\"\">");
    client.println("<button name=\"button\" style=\"font-size:16pt;\" onclick=\"submit();\">submit</button>");
    client.println("</form>");
  }
}


void wire_transmission() {
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(data);// sends one byte
  Wire.endTransmission();    // stop transmitting
}

