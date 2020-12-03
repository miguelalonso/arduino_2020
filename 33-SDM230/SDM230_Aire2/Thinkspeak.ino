const char* server_thingspeak = "api.thingspeak.com";
//SMD230
//Channel ID: 112004
//Author: miguelalonso
//Access: Public
//SMD230 Cuco75

//char thingspeak_api_key[20]="BTWBFPS831JV3KZX";         
//https://thingspeak.com/channels/1110149/api_keys
WiFiClient client_thingspeak;
//long      time_thingspeak = 30000;

long      time_thingspeak_ant = 30000;
long      time_thingspeak_ant2 = 30000;


  // // thingspeak needs minimum 15 sec delay between updates
  
  void sendthingspeak(){
    if((millis()-time_thingspeak_ant )<conf.time_thingspeak && conf.sendthing1){
      return;
      }
      time_thingspeak_ant =millis();
      if(!lect_valid){return;}
    
    if (client_thingspeak.connect(server_thingspeak,80)) { // "184.106.153.149" or api.thingspeak.com
      String apiKey = String(conf.thingspeak_api_key);
      String postStr = apiKey;
      postStr +="&field1=";
      postStr += String(Active_Power);
      postStr +="&field2=";
      postStr += String(PowerFactor);
      postStr +="&field3=";
      postStr += String(ImpActEnergy);
      postStr +="&field4=";
      postStr += String(ExpActEnergy);
      postStr +="&field5=";
      postStr += String(TotSysPowDemand);
      postStr +="&field6=";
      postStr += String(TotActEnergy);
      postStr +="&field7=";
      postStr += String(TotRctEnergy);
      postStr +="&field8=";
      postStr += String(PhaseAngle);
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
      DPRINTLN(" Los datos han sido enviados a Thingspeak https://thingspeak.com/channels/1110149/api_keys");
      DPRINTLN("");
    }
    client_thingspeak.stop();
  
  }

   void sendthingspeak2(){
    if((millis()-time_thingspeak_ant2 )<conf.time_thingspeak2 && conf.sendthing2){
      return;
      }
      time_thingspeak_ant2 =millis();
    
    if (client_thingspeak.connect(server_thingspeak,80)) { // "184.106.153.149" or api.thingspeak.com
      String apiKey = String(conf.thingspeak_api_key2);
      String postStr = apiKey;
      postStr +="&field1=";
      postStr += String(inversor.Power);
      postStr +="&field2=";
      postStr += String(bombeo.Power);
      postStr +="&field3=";
      postStr += String(aire.Power);
      postStr +="&field4=";
      postStr += String(tmt4.Power);
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

      DPRINT("Api Key2: ");
      DPRINTLN(conf.thingspeak_api_key2);
      DPRINTLN(" Los datos han sido enviados a Thingspeak https://thingspeak.com/channels/1110149/api_keys");
      DPRINTLN("");
    }
    client_thingspeak.stop();
  
  }
