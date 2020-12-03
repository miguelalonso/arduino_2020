const char* server_thinkspeak = "api.thingspeak.com";
//char thingspeak_api_key[20]="1I7AFQQ708LMPOIL";         
//https://thingspeak.com/channels/1110149/api_keys
WiFiClient client_thinkspeak;
//long      time_thinkspeak = 30000;

long      time_thinkspeak_ant = 30000;


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
      postStr += String(inversor.Power);
      postStr +="&field2=";
      postStr += String(bombeo.Power);
      postStr +="&field3=";
      postStr += String(aire.Power);
      postStr +="&field4=";
      postStr += String(pzem.power2);
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
      DPRINTLN(" Los datos han sido enviados a Thingspeak https://thingspeak.com/channels/1110149/api_keys");
      DPRINTLN("");
    }
    client_thinkspeak.stop();
  
  }
