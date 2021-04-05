/*
 * Adaptado por Miguel Alonso Abella en junio de 2020
 * 
 * Estore Servo con controlador PCA9685 16 canales PWM servomotor
 *  
 * D1 --> SCL
 * D2 --> SDA
 * D5--> UP
 * D7--> Rele puerta abierta
 * D6 --> Rele puerta cerrada
 * D8-->DOWN
 * 
 *  NodeMCU         PCA9685
 * D1               SCL
 * D2               SDA
 * 3V               VCC
 * G                GND
 *                  OE  pin de enable, no conectar
 *
 *Alimentar a 5V la PCA9685 y conectar el sevomotor a bus 0 (tiene 16)
 * si hay un segundo motor conectar a    bus1       
 * Los reles de los timers son virtuales, voy a usar 2
 
 */



//#define DEBUG   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG    //Macros are usually in all capital letters.
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif

#define CONFIG_VERSION "v01"
#define CONFIG_START 0

#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
//#include <ESPAsyncWebServer.h>
//#include <DHT.h>
#include <FS.h>
#include <ArduinoJson.h>  
#include <DNSServer.h>
#include <WiFiManager.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <EEPROM.h>

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <math.h>
//#include <SunPos.h> //Sunpos de la plataforma solar de Almería
#include <spa.h>    //spa del NREL

#include "variables.h" 
#include "PuertaStepper.h"  
#include "ESP8266_Utils_OTA.hpp"



void setup() {
  setupConfig();
  
  server.on("/tabmesures.json", sendTabMesures);
  server.on("/mesures.json", sendMesures);
  server.on("/gpio", updateGpio);
  server.on("/readgpio.json", sendGPIO);
  server.on("/hora.json", sendTime);
  server.on("/sendDataTimers.json", sendDataTimers);
  server.on("/sendDataConf.json", sendDataConf);
  server.on("/save", save);
  server.on("/savemode", savemode);
  server.on("/saveconfig", saveconfig_http);
  server.on("/savePH", savePH);

  server.serveStatic("/js", SPIFFS, "/js");
  server.serveStatic("/css", SPIFFS, "/css");
  server.serveStatic("/img", SPIFFS, "/img");
  server.serveStatic("/html", SPIFFS, "/html");
  server.serveStatic("/", SPIFFS, "/index.html");
  server.begin();
  Serial.println ( "HTTP server started" );

  
  InitOTA();

}

void loop() {
  server.handleClient();
  PuertaServoWeb();
  CheckNTPtime();
  server_read();
  horasolar();
  
  LeeFinalesCarrera();
  //sendThinkspeak();
  
    //controlPH(0);
    ArduinoOTA.handle();
    check_wifi();
    display_ssd1306();
    sendtoemoncms();
    for (j=0;j<Num_reles;j++){  controlTimers(j);}
    yield();
    
  }
