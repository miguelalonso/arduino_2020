

void sendtoemoncms(){
  
      if((millis()-time_emoncms_ant )<conf.time_emoncms){
            return;
            }
            time_emoncms_ant =millis();
 
  //Serial.println(WiFi.localIP());
  Serial.print("Estore:connecting emoncms to ");
  Serial.println(conf.ip_emoncms);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(conf.ip_emoncms, httpPort)) {
    Serial.println("connection failed");
    return;
  }
   // We now create a URI for the request
  //http://192.168.8.112/emoncms/input/post?node=emontx&json={power1:100,power2:200,power3:300}
 
  String url = String(conf.url_base)+"/input/post?node="+String(conf.node)+
  "&json={Cenit:"+String(spa.zenith)+
  ",seg:"+String(mV_PH)+
  ",To:"+String(To)+
  ",D4_Estore:"+String(estados[0])+
  
  ",IP:"+String(WiFi.localIP()[3])+
  ",arranques:"+String(conf.EEPROM_chk)+
  
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
