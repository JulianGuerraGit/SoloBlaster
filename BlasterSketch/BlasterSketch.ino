#include <FastLED.h>
#include <ESP8266WiFi.h>



const char* ssid     = "SoloBlaster";
const char* password = "Iloveleia69";

const char* ssid2     = "xxx";
const char* password2 = "xxx";

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

  handleLED(redString.toInt(), greenString.toInt(), blueString.toInt());
  server.handleClient();

}
