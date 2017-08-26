#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"
#define WIFI_SSID "prettyflyforsomewifi"
#define WIFI_PASS "password-goes-here"
#define SERIAL_BAUDRATE                 115200


fauxmoESP fauxmo;

// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("waiting on wifi.");
        delay(1000);
    }
    Serial.println();

    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

}

void setup() {
    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();
    // Wifi
    wifiSetup();
// DEFINE THE OUTPUTS 
   pinMode(LED_BUILTIN, OUTPUT);
   pinMode(12, OUTPUT);
   pinMode(13, OUTPUT);
   pinMode(14, OUTPUT);

// DONT FORGET TO SET UP THE PINS and for the love of god lable them 
  digitalWrite(LED_BUILTIN, LOW); //LED_BULITIN LOW IS ON HIGH IS OFF 
  digitalWrite(12, LOW); //pin6 relay1 gpio12 - bed side lamp 
  digitalWrite(13, LOW); //pin7 relay3 gpio13 - nothing 
  digitalWrite(14, LOW); //pin5 relay2 gpio14 - why the fuck is this backwards 

// Fauxmo - name the faux's them 
  fauxmo.addDevice("built in led"); //testing onboard led
  fauxmo.addDevice("relay two"); //pin5 relay2 gpio14 - why the fuck is this backwards
  fauxmo.addDevice("speekers"); //pin6 gpio12 relay1 - bedside lamp
  fauxmo.addDevice("relay three"); //pin7 relay3 gpio13 - nothing
  
// magic stuff here, dont know why the ! is needed for LED, dont remove it- EDIT its because something is bassackwards with onboard LED
  fauxmo.onMessage([](unsigned char device_id, const char * device_name, bool state) {
      Serial.printf("[MAIN] Device #%d (%s) state: %s\n", device_id, device_name, state ? "ON" : "OFF");
if (device_id == 0) digitalWrite(LED_BUILTIN,!state);
if (device_id == 1) digitalWrite(12, state); //pin 5
if (device_id == 2) digitalWrite(13, state); //pin 6
if (device_id == 3) digitalWrite(14, state); //pin 7
 
});
}

void loop() {  // main loop here 

    // Since fauxmoESP 2.0 the library uses the "compatibility" mode by
    // default, this means that it uses WiFiUdp class instead of AsyncUDP.
    // The later requires the Arduino Core for ESP8266 staging version
    // whilst the former works fine with current stable 2.3.0 version.
    // But, since it's not "async" anymore we have to manually poll for UDP
    // packets
    fauxmo.handle();
    static unsigned long last = millis();
    if (millis() - last > 5000) {
        last = millis();
        //Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
    }

}
