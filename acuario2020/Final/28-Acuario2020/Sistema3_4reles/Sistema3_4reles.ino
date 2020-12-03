/*
 * Adaptado por Miguel Alonso Abella en junio de 2020
 * 
 * ESP8266 + DHT22 + BMP180 + BOOTSTRAP + SPIFFS + GOOGLE CHARTS
 * Copyright (C) 2017 http://www.projetsdiy.fr - http://www.diyprojects.io
 * 
 * 
 * Modificado por Miguel Alonso Abella Mayo 2020
 */


#define DEBUG   //If you comment this line, the DPRINT & DPRINTLN lines are defined as blank.
#ifdef DEBUG    //Macros are usually in all capital letters.
  #define DPRINT(...)    Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)  Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
#endif


#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <FS.h>
#include <ArduinoJson.h>  
#include <DNSServer.h>
#include <WiFiManager.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <math.h>
//#include <SunPos.h> //Sunpos de la plataforma solar de Almería
#include <spa.h>    //spa del NREL
#include "ESP8266_Utils_OTA.hpp"

// definimos el lugar, sun y time de acuerdo con sunpos.h
// cLocation lugar;
// cSunCoordinates sun;
// cTime fecha_hora_PSA;


long      time_thinkspeak_ant = 45000;
char thingspeak_api_key[20]="J2C1Y42NA3GCC9CL";
long      time_thinkspeak = 30000;

char ip_emoncms[16] = "192.168.8.100";
char url_base[50] = "http://192.168.8.100/emoncms";
char node[20]="Sistema_3_reles";
char apikey[50]="0aff5c530d2da22a8c8cdc1a3d56aeee";
long      time_emoncms_ant = 32000;
String Hora;
String Fecha;

 spa_data spa;
 
 
      

const uint8_t GPIOPIN[4] = {D5,D6,D7,D8};  // cambiar D4 a D5
#define      Relay_D5        D5   // (D5) 
#define      Relay_D6        D6   // (D6) 
#define      Relay_D7        D7   // (D7) 
#define      Relay_D8        D8   // (D8) 
#define      Button          D0   // (D8) 

float     t = 0 ;
float     h = 0 ;
float     pa = 0;
boolean   estados[4]; //estados de los relés
int       Num_reles=4;
int       i,j,k;

long      time_old[4];

struct StoreStruct {
  int           Mode;                       
  boolean       Estado_rele;
  char          ReleName[30];     
  int           ON_Time[7];     //tiempo de cada programa, puede ser para ON u OFF en función de "onoff"
  int           OFF_Time[7];    //ya no se utiliza, reserva
  boolean       ON_Days[7][8];
  char          progName[7][30];
  int           sun_set[7];       //0:timer, 1: sunrise, 2:sunset
  int           sign_delay[7];    //0 es +; 1 es -
  int           onoff[7];         // en caso de sunrise o sunset pone en on(1) o en off(0)
  int           delay_time[7];    // variable de tiempo de retraso/adelanto para sunrise y sunset.
  int           aleatorio_Time[7]; // variable de tiempo aleatorio.
  int           Tiempo_procesado[7]; //tiempos aleatorios procesados para control
 } timers[4] ;

struct StoreStruct2 {
  char          version[4];
  int           EEPROM_chk;        // EEPROM Check
  char          DevName[30];       // Device name, i.e Sistema 3 - Aireador
  float         Latitud;
  float         Longitud;
  int           TimeZone;
  int           IP_1;
  int           IP_2;
  int           IP_3;
  int           IP_4;
  char          pwd[8];
  int           sunrise;
  int           sunset;
  long          time_thinkspeak;
  char          thingspeak_api_key[20];
  char          ip_emoncms[16] ;
  char          url_base[50] ;
  char          node[20];
  char          apikey[50];
  long          time_emoncms;
} conf ;



unsigned long lastMillis = 0;

// Variables
//----------
#define      ErrMsg            "<font color=\"red\"> < < Error !</font>"
boolean      ResetWiFi       = false;
long         timeNow         = 0;
long         timeOld         = 300000;
long         timeOld2        = 0;
long         timeOld6         = 0;
boolean      TimeOk          = false;
boolean      NTPtimeOk       = false;
String       request         = "";
byte         Page            = 1;
boolean      TimeCheck       = false;
int          NewHH           = 0;
int          NewMM           = 0;
int          Newdd           = 0;
int          Newmm           = 0;
int          Newyy           = 0;
int          PgmNr           = 0;
int          LastHH          = 0;
int          LastMM          = 0;
int          Lastdd          = 0;
int          Lastmm          = 0;
int          Lastyy          = 0;
byte         old_sec         = 0;
long         old_ms          = 0;
boolean      WebButtonON     = false;
boolean      PgmPrev         = false;
boolean      PgmNext         = false;
boolean      PgmSave         = false;
boolean      Error1          = false;
boolean      Error2          = false;
boolean      Error3          = false;
boolean      Error3b         = false;
boolean      Error4          = false;
boolean      Error5          = false;
boolean      Error6          = false;
boolean      Error7          = false;
boolean      Error8          = false;
boolean      D[8]            = {false, false, false, false, false, false, false, false};
int          PgmNrG           = 0;
int          OnHour          = 0;
int          OnMinute        = 0;
int          OffHour         = 0;
int          OffMinute       = 0;
int           ds_anterior=-5;

 
// Defines
#define      DefaultName       "Acuario_Sistema3"  // Default device name
#define      NTPfastReq        10                 // NTP time request in seconds when  time not set
#define      NTPslowReq        3600               // NTP time request in seconds after time is  set
#define      Version           "1.00"             // Firmware version
String Argument_Name;

// NTP Server details
//-------------------
IPAddress timeServer(129, 6, 15, 28);              // time.nist.gov NTP server
WiFiUDP Udp;
unsigned int localPort       = 3232;               // Local port to listen for UDP packets
bool shouldSaveConfig = false;

// Création des objets / create Objects
//DHT dht(DHTPIN, DHTTYPE);
//Adafruit_BMP085 bmp;
ESP8266WebServer server ( 80 );


void updateGpio(){
  String gpio = server.arg("id");
  String etat = server.arg("etat");
  String success = "1";
  int pin = D5;
  int num_pin=0;
 if ( gpio == "D6" ) {
      pin = D6;
      num_pin=1;
 } else if ( gpio == "D7" ) {
     pin = D7;
     num_pin=2;
 } else if ( gpio == "D8" ) {
     pin = D8;  
     num_pin=3;
 } else {   
      pin = D5;
      num_pin=0;
  }
  DPRINTLN(pin);
  if ( etat == "1" ) {
    digitalWrite(pin, HIGH); //logica negativa modificado
    estados[num_pin]=true;
  } else if ( etat == "0" ) {
    digitalWrite(pin, LOW);
    estados[num_pin]=false;
  } else {
    success = "1";
    DPRINTLN("Error Salida Digital");
  }
  
  String json = "{\"gpio\":\"" + String(gpio) + "\",";
  json += "\"etat\":\"" + String(etat) + "\",";
  json += "\"success\":\"" + String(success) + "\"}";
    
  server.send(200, "application/json", json);
  DPRINTLN("GPIO Salidas digitales actuadas");
}

void sendMesures() {
  String json = "{\"t\":\"" + String(t) + "\",";
  json += "\"h\":\"" + String(h) + "\",";
  json += "\"pa\":\"" + String(pa) + "\"}";

  server.send(200, "application/json", json);
  DPRINTLN("Mesures envoyees");
  DPRINTLN(json);
}

void sendGPIO() {
  String json = "{\"D5\":\"" + String(estados[0]) + "\",";
  json += "\"D6\":\"" + String(estados[1]) + "\",";
  json += "\"D7\":\"" + String(estados[2]) + "\",";
  json += "\"D8\":\"" + String(estados[3]) + "\"}";

  server.send(200, "application/json", json);
  DPRINTLN("GPIO enviado");
  DPRINTLN(json);
}


void sendTabMesures() {
  double temp = random(-12,45);      // Récupère la plus ancienne mesure (temperature) - get oldest record (temperature)
  String json = "[";
  json += "{\"medida\":\"Temperatura\",\"valor\":\"" + String(t) + "\",\"unidad\":\"°C\",\"precedente\":\"" + String(temp) + "\"},";
  temp = random(0,100);             // Récupère la plus ancienne mesure (humidite) - get oldest record (humidity)
  json += "{\"medida\":\"Humedad\",\"valor\":\"" + String(h) + "\",\"unidad\":\"%\",\"precedente\":\"" + String(temp) + "\"},";
  temp = random(1000,1024);             // Récupère la plus ancienne mesure (pression atmospherique) - get oldest record (Atmospheric Pressure)
  json += "{\"medida\":\"Presión Atmosférica\",\"valor\":\"" + String(pa) + "\",\"unidad\":\"mbar\",\"precedente\":\"" + String(temp) + "\"}";
  json += "]";
  server.send(200, "application/json", json);
  DPRINTLN("Tableau mesures envoyees");
}


////////////////////////////////SETUP ////////////////////////////
void setup() {
  for ( int x = 0 ; x < 5 ; x++ ) {
    pinMode(GPIOPIN[x], OUTPUT);
    digitalWrite(GPIOPIN[x],LOW);
  }
  pinMode(Button,INPUT_PULLUP);
  Serial.begin ( 9600 );
    strcpy(conf.pwd,"121212");
    conf.IP_1            = 129;
    conf.IP_2            = 6;
    conf.IP_3            = 15;
    conf.IP_4            = 28;
    conf. EEPROM_chk     =123;
    conf.Latitud         =40.41;
    conf.Longitud        =-3.731;
    strcpy(conf.DevName,"nombre del dispositivo");
    strcpy(conf.thingspeak_api_key,thingspeak_api_key);
    conf.time_thinkspeak           = 30000;
    strcpy(conf.ip_emoncms,ip_emoncms);
    strcpy(conf.url_base,url_base);
    strcpy(conf.apikey,apikey);
    strcpy(conf.node,node);
    conf.time_emoncms           = 30000;
        
    loadConfig();
    if (strlen(conf.thingspeak_api_key) < 10) {
      strcpy(conf.thingspeak_api_key,thingspeak_api_key);
    }
    if (conf.time_thinkspeak <= 0) {
      conf.time_thinkspeak  = 30000;
    }

    if (strlen(conf.apikey) < 10) {
      strcpy(conf.apikey,apikey);
    }
    if (strlen(conf.ip_emoncms) < 10) {
      strcpy(conf.ip_emoncms,ip_emoncms);
    }
    if (strlen(conf.url_base) < 10) {
      strcpy(conf.url_base,url_base);
    }
    if (strlen(conf.node) < 10) {
      strcpy(conf.node,node);
    }
    if (conf.time_emoncms <= 0) {
      conf.time_emoncms  = 30000;
    }
    
   
    StartWiFi();
    DPRINTLN("Comenzando programa");
    
  DPRINTLN(conf.DevName);
  DPRINTLN("____________________________");
 
  Serial.println ( "" );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );

  if (!SPIFFS.begin())
  {
    DPRINTLN("SPIFFS Mount failed");
  } else {
    DPRINTLN("SPIFFS Mount succesfull");
  }

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

  server.serveStatic("/js", SPIFFS, "/js");
  server.serveStatic("/css", SPIFFS, "/css");
  server.serveStatic("/img", SPIFFS, "/img");
  server.serveStatic("/html", SPIFFS, "/html");
  server.serveStatic("/", SPIFFS, "/index.html");

  server.begin();
  Serial.println ( "HTTP server started" );
  InitOTA();
  init_display();
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  t=12.34+rand()%4;//t = dht.readTemperature();
  h=50.56+rand()%4;//h = dht.readHumidity();
  pa=1023.48+rand()%4;//pa = bmp.readPressure() / 100.0F;
  
  //delay(100);
  
  
    CheckNTPtime();
    server_read();
    sendtoemoncms();
    sendThinkspeak();
    
            spa.year = year();
            spa.month = month();
            spa.day = day();
            spa.hour = hour();
            spa.minute = minute();
            spa.second = second();
            spa.timezone = 0.0;
            spa.delta_ut1 = 0;
            spa.delta_t = 67;
            spa.longitude = conf.Longitud;
            spa.latitude = conf.Latitud;
            spa.elevation = 680;
            spa.pressure = 820;
            spa.temperature = 11;
            spa.slope = 30;
            spa.azm_rotation = 0;
            spa.atmos_refract = 0.5667;
            spa.function = SPA_ALL;
            spa_calculate(&spa);    
  
 
  for (j=0;j<Num_reles;j++){  controlTimers(j);}
  check_wifi();
  display_ssd1306();
}
