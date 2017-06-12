/// Simple Server with DHCP

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network
// create your own MAC address for ther DF Robot shields
// no two boards can have the same address
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

// Initialize the Ethernet server library
// with the IP port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications :
  Serial.begin(9600);

  Serial.println("Starting Etherweb ..... ");
  // start the Ethernet connection and the server:
  Ethernet.begin(mac); // ethernet connection with your defined MAC address
  server.begin(); // start the server
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP()); // print the address provided by the router
}


void loop() {
  // create an instance of the client class
  // this is needed for an incoming client
  EthernetClient client = server.available();

  if (client) { // if a client connects

    Serial.println("new client");

    // HTTP requests end with a blank line
    // we'll use this to know the request from the client has finished
    boolean currentLineIsBlank = true;

    while (client.connected()) { // while someone is connected to the server
      
      if (client.available()) { // if there are bytes from the client 
                                // these would be sent via a broweser, etc
                                
        char c = client.read(); // read a byte sent from a client
        Serial.write(c); // print it to the serial monitor
        
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh:1"); // refresh the page every second
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>"); // you need to expicitly wriet out all html
          
          // read the value of the input pin
          int sensorReading = analogRead(A0);
          // print it to the page
          client.print("analog input A0 is ");
          client.print(sensorReading);
          client.println("<br />");
          // change the font size every time the page is refreshed
          client.print("<p style=\"font-size:");
          client.print(map(sensorReading, 0, 1023, 1, 144));
          client.println("px\">This will change size when refreshed.</p>");
          client.println("</html>"); // close the tag
          break; // escape the statement
        }
        if (c == '\n') { 
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
