const char* server_thinkspeak = "api.thingspeak.com";
char thingspeak_api_key[20]="ROZ5CCKX71G402VR";
char sensor_type[10] = "YF-S201"; //Default to YF-S201
WiFiClient client_thinkspeak;
long      time_thinkspeak = 45000;
long      time_thinkspeak_ant = 45000;


  // // thingspeak needs minimum 15 sec delay between updates
  
  void sendThinkspeak(){
    if((millis()-time_thinkspeak_ant )<time_thinkspeak){
      return;
      }
      time_thinkspeak_ant =millis();
    
    if (client_thinkspeak.connect(server_thinkspeak,80)) { // "184.106.153.149" or api.thingspeak.com
      String apiKey = String(thingspeak_api_key);
      String postStr = apiKey;
      postStr +="&field1=";
      postStr += String(flowMilliLitres);
      postStr +="&field2=";
      float total=totalMilliLitres/1000;
      postStr += String(total);
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
      DPRINTLN(thingspeak_api_key);
      DPRINT("Sensor Type: ");
      DPRINTLN(sensor_type);
      DPRINT("totalMilliLitres: ");
      DPRINTLN(totalMilliLitres);
      DPRINT("Flow Rate : ");
      DPRINT(flowMilliLitres);
      DPRINTLN(" litres/hour send to Thingspeak");
      DPRINTLN("");
    }
    client_thinkspeak.stop();
  
  }
