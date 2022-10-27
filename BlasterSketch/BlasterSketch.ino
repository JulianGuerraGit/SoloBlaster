#include <FastLED.h>
#include <ESP8266WiFi.h>


const char* ssid     = "SoloBlaster";
const char* password = "Iloveleia69";

const char* ssid2     = "***REMOVED***";
const char* password2 = "***REMOVED***";

#define TRIGGER_PIN 3

#define LED_PIN     1
#define NUM_LEDS    1

CRGB led[NUM_LEDS];

WiFiServer server(80);
 
String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
String html_1 = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>Blaster Control</title></head><body><div id='main'><h2>Blaster Control</h2>";
String html_2 = "<form id='F1' action='FIRE'><input class='button' type='submit' value='FIRE' ></form><br>";
String html_3 = "<form id='F2' action='RED'><input class='button' type='submit' value='RED' ></form><br>";
String html_4 = "<form id='F2' action='GREEN'><input class='button' type='submit' value='GREEN' ></form><br>";
String html_5 = "<form id='F2' action='BLUE'><input class='button' type='submit' value='BLUE' ></form><br>";
String html_6 = "</div></body></html>";
 
String request = "";  // Variable to store the HTTP request

String redString = "255";
String greenString = "0";
String blueString = "0";
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;

int r = 255;
int g = 0;
int b = 0;


int TriggerState = 0;


void handleLED(int r, int g, int b) {
  TriggerState = digitalRead(TRIGGER_PIN);

  if(TriggerState == LOW) {

    fill_solid(led, NUM_LEDS, CRGB(r, g, b));
    FastLED.show();
    Serial.println("Pew \n");
  }
  else {
    fadeToBlackBy(led, NUM_LEDS, 25);
    delay(10);
    FastLED.show();
  }
}

void setup() {

  Serial.begin(115200);
  
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(led, NUM_LEDS);

  //boolean conn = WiFi.softAP(ssid, password);
  WiFi.begin(ssid2, password2);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  
}

void loop() {

  WiFiClient client = server.available();

  handleLED(redString.toInt(), greenString.toInt(), blueString.toInt());
  
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      handleLED(redString.toInt(), greenString.toInt(), blueString.toInt());
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
                   
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=2\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\">");
            client.println("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>");
            client.println("</head><body><div class=\"container\"><div class=\"row\"><h1>Blaster Control</h1></div>");
            client.println("<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color\" role=\"button\">Change Color</a></div>");
            client.println("<div<input class=\"jscolor {onFineChange:'update(this)'}\" id=\"rgb\"></div>");
            client.println("<script>function update(picker) {document.getElementById('rgb').innerHTML = Math.round(picker.rgb[0]) + ', ' +  Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);");
            client.println("document.getElementById(\"change_color\").href=\"?r\" + Math.round(picker.rgb[0]) + \"g\" +  Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}</script></body></html>");
            // The HTTP response ends with another blank line
            client.println();

            // Request sample: /?r201g32b255&
            // Red = 201 | Green = 32 | Blue = 255
            if(header.indexOf("GET /?r") >= 0) {
              pos1 = header.indexOf('r');
              pos2 = header.indexOf('g');
              pos3 = header.indexOf('b');
              pos4 = header.indexOf('&');
              redString = header.substring(pos1+1, pos2);
              greenString = header.substring(pos2+1, pos3);
              blueString = header.substring(pos3+1, pos4);
             
            }
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }


//  // Check if a client has connected
//  WiFiClient client = server.available();
//  if (!client)  {  return;  }
//
//  // Read the first line of the request
//  request = client.readStringUntil('\r');
//
//  if       ( request.indexOf("FIRE") > 0 )  { 
//    TriggerState = HIGH;
//    delay(250);
//    TriggerState = LOW;
//  }
//  else if  ( request.indexOf("RED") > 0 ) { 
//
//    int r = 255;
//    int g = 0;
//    int b = 0;
//    
//  }
//  else if  ( request.indexOf("GREEN") > 0 ) { 
//
//    int r = 0;
//    int g = 255;
//    int b = 0;
//    
//  }
//  else if  ( request.indexOf("RED") > 0 ) { 
//
//    int r = 0;
//    int g = 0;
//    int b = 255;
//    
//  }
//  // Request sample: /?r201g32b255&
//  // Red = 201 | Green = 32 | Blue = 255
//  if(request.indexOf("GET /?r") >= 0) {
//    pos1 = header.indexOf('r');
//    pos2 = header.indexOf('g');
//    pos3 = header.indexOf('b');
//    pos4 = header.indexOf('&');
//    redString = header.substring(pos1+1, pos2);
//    greenString = header.substring(pos2+1, pos3);
//    blueString = header.substring(pos3+1, pos4);
//    /*Serial.println(redString.toInt());
//    Serial.println(greenString.toInt());
//    Serial.println(blueString.toInt());*/
//  }
//
//  client.flush();
//
//  client.print( header );
//  client.print( html_1 );
//  client.print( html_2 );
//  client.print( html_3 );
//  client.print( html_4 );
//  client.print( html_5 );
//  client.print( html_6 );
//
//  
//  delay(5);

}
