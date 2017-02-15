/******************************************
   The unmodified version of this code is originally
   by kakopappa and can be found at http://bit.ly/2kKQiRg.

   This version of the code has been modified by Charles Gantt
   and requires five additional files which can be found at http://bit.ly/2lRDwAJ

   Find out more about this project on Charles' website
   http://www.themakersworkbench.com

   Follow Charles and TheMakersWorkbench on the following sites:
   YouTube: bit.ly/TMWB-on-YouTube
   TMWB on Facebook: bit.ly/TMWB-on-Facebook
   CJGanttMakes on Facebook: bit.ly/CJGanttMakes
   TMWB on Twitter: bit.ly/TMWB-on-Twitter
   Charles Gantt on Twitter: bit.ly/CJGanttOnTwitter
   Instructables: bit.ly/CJGanttOnInstructables
   TMWB Website: bit.ly/TheMakersWorkbench
   Charles Gantt on Element14: bit.ly/CJGantt-On-E14
   Charles Gantt on GitHub: bit.ly/CJGantt-on-Github
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// prototypes
boolean connectWifi();

//on/off callbacks
void lightOneOn();
void lightOneOff();
void lightTwoOn();
void lightTwoOff();
void outletOneOn();
void outletOneOff();
void outletTwoOn();
void outletTwoOff();

// Change this before you flash
const char* ssid = "Skynet";
const char* password = "8039795700";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *lightOne = NULL;
Switch *lightTwo = NULL;
Switch *outletOne = NULL;
Switch *outletTwo = NULL;

// Set Relay Pins
int relayOne = 14;
int relayTwo = 15;
int relayThree = 03;
int relayFour = 01;

// Addr: 0x3F, 20 chars & 4 lines. Sometimes display boards use address 0x27
LiquidCrystal_I2C lcd(0x3F, 4, 20); //Frentally display, use 0x3F if not working try 0x27

void setup()
{
  //Initalize LCD
  lcd.init();
  lcd.noBacklight();
  lcd.backlight();
  lcd.begin(20, 4);

  //Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();
  //Serial.print("WiFi Connected");

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

    // Show WiFi status on LCD along with SSID of network
    lcd.setCursor(0, 0);
    lcd.print("   WiFi Connected   ");
    lcd.print(ssid);

    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
    lightOne = new Switch("Light One", 80, lightOneOn, lightOneOff);
    lightTwo = new Switch("Light Two", 81, lightTwoOn, lightTwoOff);
    outletOne = new Switch("Outlet One", 82, outletOneOn, outletOneOff);
    outletTwo = new Switch("Outlet Two", 83, outletTwoOn, outletTwoOff);

    //Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*lightOne);
    upnpBroadcastResponder.addDevice(*lightTwo);
    upnpBroadcastResponder.addDevice(*outletOne);
    upnpBroadcastResponder.addDevice(*outletTwo);

    //Set relay pins to outputs
    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);
    pinMode(03, OUTPUT);
    pinMode(01, OUTPUT);

    //Create Polling Message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Polling Status   ");
    lcd.setCursor(0, 2);
    lcd.print("  Of Smart Devices  ");
    delay(2000);

    //Set each relay pin to HIGH this display status messagefor each relay
    digitalWrite(relayOne, HIGH);   // sets relayOne on
    lcd.clear();
    lcd.print("Light  One: Off     ");
    delay(500);
    digitalWrite(relayTwo, HIGH);   // sets relayOne on
    lcd.setCursor(0, 1);
    lcd.print("Light  Two: Off     ");
    delay(500);
    digitalWrite(relayThree, HIGH);   // sets relayOne on
    lcd.setCursor(0, 2);
    lcd.print("Outlet One: Off     ");
    delay(500);
    digitalWrite(relayFour, HIGH);   // sets relayOne on
    delay(500);
    lcd.setCursor(0, 3);
    lcd.print("Outlet Two: Off     ");

    //Create system initialized message
    lcd.clear();
    lcd.setCursor(0, 0);
    delay(1000);
    lcd.print(" System Initialzed  ");
    delay(1000);
    lcd.setCursor(0, 2);
    lcd.print(" Ready For Commands ");
    delay(2000);

    //Set up device status message
    lcd.clear();
    lcd.print("Light  One: Off     ");
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print("Light  Two: Off     ");
    delay(500);
    lcd.setCursor(0, 2);
    lcd.print("Outlet One: Off     ");
    delay(500);
    lcd.setCursor(0, 3);
    lcd.print("Outlet Two: Off     ");


  }
}

void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();
    lightOne->serverLoop();
    lightTwo->serverLoop();
    outletOne->serverLoop();
    outletTwo->serverLoop();
  }
}

void lightOneOn() {
  // Serial.print("Switch 1 turn on ...");
  digitalWrite(relayOne, LOW);   // sets relayOne on
  lcd.setCursor(0, 0);
  lcd.print("Light  One: On      ");
}

void lightOneOff() {
  // Serial.print("Switch 1 turn off ...");
  digitalWrite(relayOne, HIGH);   // sets relayOne off
  lcd.setCursor(0, 0);
  lcd.print("Light  One: Off     ");
}

void lightTwoOn() {
  // Serial.print("Switch 2 turn on ...");
  digitalWrite(relayThree, LOW);   // sets relayTwo on
  lcd.setCursor(0, 1);
  lcd.print("Light  Two: On      ");
}

void lightTwoOff() {
  // Serial.print("Switch 2 turn off ...");
  digitalWrite(relayThree, HIGH);   // sets relayTwo Off
  lcd.setCursor(0, 1);
  lcd.print("Light  Two: Off     ");
}

//sockets

void outletOneOn() {
  //  Serial.print("Socket 1 turn on ...");
  digitalWrite(relayFour, LOW);   // sets relayThree on
  lcd.setCursor(0, 2);
  lcd.print("Outlet One: On      ");
}

void outletOneOff() {
  // Serial.print("Socket 1 turn off ...");
  digitalWrite(relayFour, HIGH);   // sets relayThree off
  lcd.setCursor(0, 2);
  lcd.print("Outlet One: Off     ");
}

void outletTwoOn() {
  // Serial.print("Socket 2 turn on ...");
  digitalWrite(relayTwo, LOW);   // sets relayFour on
  lcd.setCursor(0, 3);
  lcd.print("Outlet Two: On      ");
}

void outletTwoOff() {
  // Serial.print("Socket 2 turn off ...");
  digitalWrite(relayTwo, HIGH);   // sets relayFour off
  lcd.setCursor(0, 3);
  lcd.print("Outlet Two: Off     ");
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  // Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }

  if (state) {
    //  Serial.println("");
    //  Serial.print("Connected to ");
    //  Serial.println(ssid);
    // Serial.print("IP address: ");
    //  Serial.println(WiFi.localIP());
  }
  else {
    // Serial.println("");
    //Serial.println("Connection failed.");
  }

  return state;
}
