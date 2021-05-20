#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,2,101);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

///------------------------------------------------------------------------------------------------------
///--------------------------------------------------PINY------------------------------------------------
///------------------------------------------------------------------------------------------------------
const int historia = 6, dzialanie = 8, zygalski = 9, rozycki = 7, rejewski = 4;

// Client variables 
char linebuf[80];
int charcount=0;

void setup() { 
  // Relay module prepared 
  pinMode(historia, OUTPUT);
  digitalWrite(historia, LOW);
  
  pinMode(dzialanie, OUTPUT);
  digitalWrite(dzialanie, LOW);
  
  pinMode(zygalski, OUTPUT);
  digitalWrite(zygalski, LOW);

   pinMode(rozycki, OUTPUT);
  digitalWrite(rozycki, LOW);

  pinMode(rejewski, OUTPUT);
  digitalWrite(rejewski, LOW);
  
  // Open serial communication at a baud rate of 9600
  Serial.begin(9600);
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

// Display dashboard page with on/off button for relay
// It also print Temperature in C and F
void dashboardPage(EthernetClient &client) {
  client.println("<!DOCTYPE HTML><html><head>");
  client.println("<meta charset=\"utf-8\" name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body>");                                                                      
  client.println("<h1 style=\"font-size: 38px;\"><center>WIELCY POLSCY MATEMATYCY</center></h1>");              
  client.println("<a href=\"/historia\"><button style=\"width:350px; height:105px; font-size: 34px;font-family:Georgia; background: #8ba181;\"><i>HISTORIA ENIGMY</i></button></a><br><br>");  
  client.println("<a href=\"/dzialanie\"><button style=\"width:350px; height:105px; font-size: 34px;font-family:Georgia;background: #748d68;\"><i>DZIAŁANIE ENIGMY</i></button></a><br><br>");  
  client.println("<a href=\"/zygalski\"><button style=\"width:350px; height:105px; font-size: 34px;font-family:Georgia;background: #5b734f;\"><i>HENRYK ZYGALSKI</i></button></a><br><br>");   
  client.println("<a href=\"/rozycki\"><button style=\"width:350px; height:105px; font-size: 34px;font-family:Georgia;background: #3c5c2c;\"><i>JERZY RÓŻYCKI</i></button></a><br><br>");  
  client.println("<a href=\"/rejewski\"><button style=\"width:350px; height:105px; font-size: 34px;font-family:Georgia;background: #335224;\"><i>MARIAN REJEWSKI</i></button></a><br><br>");                                                                          
  client.println("</body></html>"); 
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    //Serial.println("new client");
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
       char c = client.read();
       //read char by char HTTP request
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          dashboardPage(client);
          break;
        }
        if (c == '\n') {
          if (strstr(linebuf,"GET /historia") > 0){
            Serial.println("HISTORIA ENIGMY!");
            digitalWrite(historia, HIGH);
            delay(100);
            digitalWrite(historia, LOW);
          }
          else if (strstr(linebuf,"GET /dzialanie") > 0){
            Serial.println("DZIALANIE ENIGMY!");
            digitalWrite(dzialanie, HIGH);
            delay(100);
            digitalWrite(dzialanie, LOW);
          }
          else if (strstr(linebuf,"GET /zygalski") > 0){
            Serial.println("zygalski!");
            digitalWrite(zygalski, HIGH);
            delay(100);
            digitalWrite(zygalski, LOW);
          }
          else if (strstr(linebuf,"GET /rozycki") > 0){
            Serial.println("rozycki!");
            digitalWrite(rozycki, HIGH);
            delay(100);
            digitalWrite(rozycki, LOW);
          }
          else if (strstr(linebuf,"GET /rejewski") > 0){
            Serial.println("rejewski!");
            digitalWrite(rejewski, HIGH);
            delay(100);
            digitalWrite(rejewski, LOW);
          }
          // you're starting a new line
          currentLineIsBlank = true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;          
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    //Serial.println("client disonnected");
  }
}
