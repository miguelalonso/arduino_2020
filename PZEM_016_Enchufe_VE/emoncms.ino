
void sendtoemoncms(){
  
      if((millis()-time_emoncms_ant )<conf.time_emoncms){
            return;
            }
            time_emoncms_ant =millis();
 
  Serial.println(WiFi.localIP());
  Serial.print("G:connecting emoncms to ");
  Serial.println(conf.ip_emoncms);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(ip_emoncms, httpPort)) {
    Serial.println("connection failed");
    return;
  }
   // We now create a URI for the request
  //http://192.168.8.112/emoncms/input/post?node=emontx&json={power1:100,power2:200,power3:300}


  String url = String(conf.url_base)+"/input/post?node="+String(conf.node_emon)+
  "&json={pzem.voltage:"+String(pzem.voltage)+
  ",pzem.voltage:"+String(pzem.voltage)+
  ",pzem.current:"+String(pzem.current)+
  ",pzem.power:"+String(pzem.power)+
  ",pzem.power2:"+String(pzem.power2)+
  ",pzem.energy:"+String(pzem.energy)+
  ",pzem.frequency:"+String(pzem.frequency)+
  ",pzem.power_factor:"+String(pzem.power_factor)+
  ",cenit:"+String(spa.zenith)+
  
  ",D0:"+String(estados[0])+
  ",D6:"+String(estados[1])+
  ",D7:"+String(estados[2])+
  ",D8:"+String(estados[3])+
  ",D4:"+String(estados[4])+
  
  "}&apikey="+String(conf.apikey); 

 

  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + String(conf.ip_emoncms) + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    DPRINT(line);
  }
  
  Serial.println();
  Serial.println("closing emoncms connection");
  }
