//char ip_emoncms[16] = "192.168.8.112";
//char url_base[50] = "http://192.168.8.112/emoncms";
//char node[20]="Sis_1_Temp_TDS";
//char apikey[50]="0aff5c530d2da22a8c8cdc1a3d56aeee";
//long      time_thinkspeak_ant = 32000;

void sendtoemoncms(){
  
      if((millis()-time_emoncms_ant )<conf.time_emoncms){
            return;
            }
            time_emoncms_ant =millis();
 
  Serial.println("connecting emoncms to ");
  DPRINTLN(conf.ip_emoncms);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(ip_emoncms, httpPort)) {
    DPRINTLN("connection failed");
    return;
  }
   // We now create a URI for the request
  //http://192.168.8.112/emoncms/input/post?node=emontx&json={power1:100,power2:200,power3:300}
 
  String url = String(conf.url_base)+"/input/post?node="+String(conf.node)+
  "&json={rele_0:"+String(estados[0])+
  ",rele_1:"+String(estados[1])+
  ",rele_2:"+String(estados[2])+
  ",rele_3:"+String(estados[3])+
  "}&apikey="+String(conf.apikey); 

 

  DPRINT("Requesting URL: ");
  DPRINTLN(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + String(conf.ip_emoncms) + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      DPRINTLN(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    DPRINT(line);
  }
  
  DPRINTLN();
  Serial.println("closing emoncms connection");
  }
