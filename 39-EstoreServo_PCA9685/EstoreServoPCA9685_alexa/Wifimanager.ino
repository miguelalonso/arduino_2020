void check_wifi(){
  if((millis()-timeOld6)<60000){
    return;
  }
  timeOld6=millis();
  DPRINTLN(WiFi.status());
  Serial.print("millis: ");
  Serial.println(millis());
  Serial.print("WiFi.status: ");
  Serial.println(WiFi.status());
    if(WiFi.status() != WL_CONNECTED)
  {
     DPRINTLN("reconectando WIFI");
     StartWiFi();
  }
    Serial.println();
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
    Serial.println(WiFi.localIP());
     Serial.print("FreeMemory: ");
    Serial.println(ESP.getFreeHeap(),DEC);

     static uint32_t myfree;
  static uint16_t mymax;
  static uint8_t myfrag;
     ESP.getHeapStats(&myfree, &mymax, &myfrag);
     Serial.printf("(%d) -> free: %5d - max: %5d - frag: %3d%% <- \n", millis(), myfree, mymax, myfrag);

  
 }

//###############################################################################################
// callback notifying us of the need to save config
// 
//###############################################################################################
void saveConfigCallback () {
  shouldSaveConfig = true;
}


//###############################################################################################
// Start WiFi
// 
//###############################################################################################
void StartWiFi() {
  //read configuration from FS json
  //----------------------------------------------------------------------
  if (SPIFFS.begin()) {
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);
        //DynamicJsonBuffer jsonBuffer;
        DynamicJsonDocument json(1024);
       // JsonObject& json = jsonBuffer.parseObject(buf.get());
       serializeJson(json, Serial);

       // json.printTo(Serial);
        //if (json.success()) {
          //strcpy(mqtt_server, json["mqtt_server"]);
          //strcpy(mqtt_port, json["mqtt_port"]);
          //strcpy(blynk_token, json["blynk_token"]);
//          if(json["ip"]) {
//            strcpy(static_ip, json["ip"]);
//            strcpy(static_gw, json["gateway"]);
//            strcpy(static_sn, json["subnet"]);
//          }
         
      }
    }
  } 
  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  //----------------------------------------------------------------------
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  //----------------------------------------------------------------------
  WiFiManager wifiManager;
  //set config save notify callback
  //----------------------------------------------------------------------
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  //set static ip
  //----------------------------------------------------------------------
//  IPAddress _ip,_gw,_sn;
//  _ip.fromString(static_ip);
//  _gw.fromString(static_gw);
//  _sn.fromString(static_sn);
//  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);
 // wifiManager.setSTAStaticIPConfig(IPAddress(192,168,1,150), IPAddress(192,168,1,1), IPAddress(255,255,255,0)); // optional DNS 4th argument
  
  //reset settings
  //----------------------------------------------------------------------
  if (ResetWiFi == true) {
    wifiManager.resetSettings();
    ResetWiFi == false;
  }
  //set minimum quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //----------------------------------------------------------------------
  wifiManager.setMinimumSignalQuality();
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep in seconds
  //----------------------------------------------------------------------
  wifiManager.setTimeout(120);
  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "IoT Timer", password = password
  //and goes into a blocking loop awaiting configuration
  //----------------------------------------------------------------------
  if (!wifiManager.autoConnect(DefaultName)) {
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }
  //save the custom parameters to FS
  //----------------------------------------------------------------------
  if (shouldSaveConfig) {
    //DynamicJsonBuffer jsonBuffer;
    DynamicJsonDocument doc(1024);
    //JsonObject& json = jsonBuffer.createObject();
    doc["ip"] = WiFi.localIP().toString();
    doc["gateway"] = WiFi.gatewayIP().toString();
    doc["subnet"] = WiFi.subnetMask().toString();
    File configFile = SPIFFS.open("/config.json", "w");
    serializeJson(doc, Serial);
    serializeJson(doc, configFile);
    //json.prettyPrintTo(Serial);
    //json.printTo(configFile);
    configFile.close();
    //end save
  }
  //if you get here you have connected to the WiFi
  // Read settings from EEPROM
  //----------------------------------------------------------------------
  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  
  EEPROM.begin(512);
      
  // Setup NTP time requests
  //----------------------------------------------------------------------
  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(NTPfastReq);
  // Begin IoT server
  //----------------------------------------------------------------------
  server.begin();
}
