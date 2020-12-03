const char* server_thinkspeak = "api.thingspeak.com";
//char thingspeak_api_key[20]="J2C1Y42NA3GCC9CL";

WiFiClient client_thinkspeak;
//long      time_thinkspeak = 45000;



  // // thingspeak needs minimum 15 sec delay between updates
  
  void sendThinkspeak(){
    if((millis()-time_thinkspeak_ant )<conf.time_thinkspeak){
      return;
      }
      time_thinkspeak_ant =millis();
    
    if (client_thinkspeak.connect(server_thinkspeak,80)) { // "184.106.153.149" or api.thingspeak.com
      String apiKey = String(conf.thingspeak_api_key);
      String postStr = apiKey;
      postStr +="&field1=";
      postStr += String(estados[0]);
      postStr +="&field2=";
      postStr += String(estados[1]);
      postStr +="&field3=";
      postStr += String(estados[2]);
       postStr +="&field4=";
      postStr += String(estados[3]);
      postStr += "\r\n\r\n";
      
      client_thinkspeak.print("GET /update HTTP/1.1\n");
      client_thinkspeak.print("Host: api.thingspeak.com\n");
      client_thinkspeak.print("Connection: close\n");
      client_thinkspeak.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
      client_thinkspeak.print("Content-Type: application/x-www-form-urlencoded\n");
      client_thinkspeak.print("Content-Length: ");
      client_thinkspeak.print(postStr.length());
      client_thinkspeak.print("\n\n");
      client_thinkspeak.print(postStr);

      DPRINT("Api Key: ");
      DPRINTLN(conf.thingspeak_api_key);
    
     Serial.println(" datos enviados to Thingspeak");
      DPRINTLN("");
      // Print the IP address
        Serial.println("Use this URL to connect: ");
        Serial.print("http://");
        Serial.print(WiFi.localIP());
        Serial.println("/");
  
    }
    client_thinkspeak.stop();
  
  }
