
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
 
  String url = String(conf.url_base)+"/input/post?node="+String(conf.node)+
  "&json={G:"+String(G)+
  ",mV_G:"+String(mV_G)+
  ",G2:"+String(G2)+
  ",mV_G2:"+String(mV_G2)+
  ",G3:"+String(G3)+
  ",mV_G3:"+String(mV_G3)+
  ",G4:"+String(G4)+
  ",mV_G4:"+String(mV_G4)+
  ",cenit:"+String(spa.zenith)+
  
  ",D3_CO2:"+String(estados[0])+
  ",D6:"+String(estados[1])+
  ",D7:"+String(estados[2])+
  ",D8:"+String(estados[3])+
  ",D4_Nivel:"+String(estados[4])+
  
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
