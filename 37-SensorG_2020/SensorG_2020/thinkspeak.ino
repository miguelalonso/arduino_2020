#include <ThingSpeak.h>

const char* server_thingspeak = "api.thingspeak.com";
//char thingspeak_api_key[20]="1J8HKOKXIW0ZBHVR";

WiFiClient client_thingspeak;
//long      time_thingspeak = 45000;
long      time_thingspeak_ant = 45000;


  // // thingspeak needs minimum 15 sec delay between updates
  
  void sendthingspeak(){
    if((millis()-time_thingspeak_ant )<conf.time_thingspeak){
      return;
      }
      time_thingspeak_ant =millis();
    
    if (client_thingspeak.connect(server_thingspeak,80)) { // "184.106.153.149" or api.thingspeak.com
      String apiKey = String(conf.thingspeak_api_key);
      String postStr = apiKey;
      postStr +="&field1=";
      postStr += String(G);
      postStr +="&field2=";
      postStr += String(mV_G);
      postStr +="&field3=";
      postStr += String(mV_G2);
      postStr +="&field4=";
      postStr += String(G2);
      postStr +="&field5=";
      postStr += String(estados[0]);
      postStr +="&field6=";
      postStr += String(estados[4]);
      postStr += "\r\n\r\n";
      
      client_thingspeak.print("GET /update HTTP/1.1\n");
      client_thingspeak.print("Host: api.thingspeak.com\n");
      client_thingspeak.print("Connection: close\n");
      client_thingspeak.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
      client_thingspeak.print("Content-Type: application/x-www-form-urlencoded\n");
      client_thingspeak.print("Content-Length: ");
      client_thingspeak.print(postStr.length());
      client_thingspeak.print("\n\n");
      client_thingspeak.print(postStr);

      DPRINT("Api Key: ");
      DPRINTLN(conf.thingspeak_api_key);
      
      Serial.print("Api Key: ");
      Serial.println(conf.thingspeak_api_key);
      Serial.println(postStr);
    
      Serial.print(" G: datos enviados to Thingspeak");
      Serial.println("");
      // Print the IP address
        Serial.print("Use this URL to connect: ");
        Serial.print("http://");
        Serial.print(WiFi.localIP());
        Serial.println("/");
  
    }
    client_thingspeak.stop();
  
  }
