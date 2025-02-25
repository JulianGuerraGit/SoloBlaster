#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <ArduinoOTA.h>

// WIFI SSID AND PASSWORD *OTA PASSWORD IS SAME AS WIFI PASSWORD*
const char* ssid     = "xxxx";
const char* password = "xxxx";

// ACCESS POINT SSID AND PASSWORD
const char* APssid     = "Blaster";
const char* APpassword = "Iloveleia69";

// POWER LIMITS
#define VOLTS 5
#define MILLIAMPS 250

// TRIGGER PIN ON WEMOS
#define TRIGGER_PIN 4

// LED PIN AND COUNT
#define LED_PIN     2
#define NUM_LEDS    6

CRGB led[NUM_LEDS];

ESP8266WebServer server(80);

// INITIAL COLOR AND STATE
int r = 255;
int g = 0;
int b = 0;
bool rainbow = false;
uint8_t hue = 0;

int TriggerState = 0;

// VOLTAGE MATRIX TO MAP BATTERY VOLTAGES TO PERCENTAGES
float fVoltageMatrix[22][2] = {
    {4.10, 100},
    {4.08, 95},
    {4.05, 90},
    {4.02, 85},
    {3.99, 80},
    {3.96, 75},
    {3.93, 70},
    {3.90, 65},
    {3.85, 60},
    {3.82, 55},
    {3.79, 50},
    {3.73, 45},
    {3.69, 40},
    {3.65, 35},
    {3.59, 30},
    {3.56, 25},
    {3.52, 20},
    {3.45, 15},
    {3.42, 10},
    {3.37, 5},
    {3.27, 0},
    {0, 0}
};

int i, perc;

void setup() {
  
  i = 0; // set i to 0 for wifi connect timeout
  
  WiFi.mode(WIFI_AP_STA); // Chooses both Access point mode and station mode
  WiFi.softAP(APssid, APpassword); // Start access point
  WiFi.begin(ssid, password); // begin wifi connection
  while (WiFi.status() != WL_CONNECTED) { // wait while wifi connects
    delay(200);
    Serial.print(".");
    if ( i >= 15 ) {  i=0;  break;  } // check if its been longer than 3 seconds
    else {  i++;  } 
  }

  // Pin setup
  pinMode(A0, INPUT); 
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  // FastLED initialization
  FastLED.addLeds<NEOPIXEL, LED_PIN>(led, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MILLIAMPS);

  // OTA setup
  ArduinoOTA.setHostname(APssid);
  ArduinoOTA.setPassword(password);

  // Begin filesystem
  LittleFS.begin();

  // setup webserver server responses
  server.on("/", serverHomepage );
  server.on("/setColor", setLED);
  server.on("/rainbow", toggleRainbow);
  server.on("/battery", checkBattery);
  server.on("/fire", Fire);

  // begin webserver and OTA
  server.begin();
  ArduinoOTA.begin();

}

// Access homepage html from file system and stream it to client.
void serverHomepage() {
  File file = LittleFS.open("colorpicker.html", "r");
  server.streamFile(file, "text/html");
  file.close();

}

// Battery check calculations and HTML display
void checkBattery() {

  int nVoltageRaw = analogRead(A0);
  float fVoltage = (float)nVoltageRaw * 0.00464;

  perc = 100;

  for(i=20; i>=0; i--) {
    if(fVoltageMatrix[i][0] >= fVoltage) {
      perc = fVoltageMatrix[i + 1][1];
      break;
    }
  }

  server.send(200, "text/html", String("<!DOCTYPE html><html><head><meta name=\"viewport\"content=\"width=device-width, initial-scale=1\">") + String("<style>body {background-color: #242429;} h1{color: white;text-shadow: 2px 2px black;text-align: center;}</style></head><body><div class=\"container\"><div><h1>Voltage: ") + String(fVoltage) + String(" V <br><br> Percent:") + String(perc) + " %</h1></div></div></body></html>");
  
}

// Webpage remote fire
void Fire() {
  handleLED(r, g, b, rainbow, true);
  delay(250);
  serverHomepage();
}

// Rainbow mode toggle
void toggleRainbow() {
  rainbow = !rainbow;
  serverHomepage();
}

// Check how long trigger is held and toggle rainbow
void triggerHold(bool clear) {
  if(clear && i!=0) { i = 0; }
  else if(i  >= 800) {  toggleRainbow();  i = 0;  }
  else{ delay(10); i++;}
  
}

// LED color change request
void setLED() {
  r = server.arg("r").toInt();
  g = server.arg("g").toInt();
  b = server.arg("b").toInt();
  rainbow = false;
  serverHomepage();
}

// LED firing animation/rainbow mode
void handleLED(int r, int g, int b, bool rainbow, bool Fire) {
  
  TriggerState = digitalRead(TRIGGER_PIN);

  if (TriggerState == LOW || Fire) {
    if (rainbow) { 
      fill_solid(led, NUM_LEDS, CHSV(hue, 255, 255)); 
      EVERY_N_MILLISECONDS( 5 ) {  hue+=2; }
    }
    else{ 
      fill_solid(led, NUM_LEDS, CRGB(r, g, b)); 
    }
    triggerHold(false);
    FastLED.show();
  }
  else {
    triggerHold(true);
    fadeToBlackBy(led, NUM_LEDS, 25);
    delay(10);
    FastLED.show();
  }
  
}

// Main Loop
void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  handleLED(r, g, b, rainbow, false);
}
