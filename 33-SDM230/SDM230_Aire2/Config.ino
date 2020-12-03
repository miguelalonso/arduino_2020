void setupConfig(){

  Serial.begin ( 9600 );
  Serial.println("Starting...");

  sdm.begin();
  
    strcpy(conf.pwd,"121212");
    conf.IP_1            = 129;
    conf.IP_2            = 6;
    conf.IP_3            = 15;
    conf.IP_4            = 28;
    conf. EEPROM_chk     =123;
    conf.Latitud         =40.41;
    conf.Longitud        =-3.731;
    strcpy(conf.DevName,"Control Aire Cuco 75");
    strcpy(conf.thingspeak_api_key,thingspeak_api_key);
    conf.time_thingspeak           = 30000;
    strcpy(conf.ip_aire,"192.168.1.130");
    strcpy(conf.ip_inversor,"192.168.1.131");
    strcpy(conf.ip_bombeo,"192.168.1.132");
    strcpy(conf.ip_tmt4,"192.168.1.133");
    
    loadConfig();

   if (strlen(conf.thingspeak_api_key) < 10) {
      strcpy(conf.thingspeak_api_key,thingspeak_api_key);
    }
    if (strlen(conf.thingspeak_api_key2) < 10) {
      strcpy(conf.thingspeak_api_key2,thingspeak_api_key2);
    }
    if (strlen(conf.ip_aire) < 10) {
      strcpy(conf.ip_aire,"192.168.1.130");
    }
    if (strlen(conf.ip_inversor) < 10) {
       strcpy(conf.ip_inversor,"192.168.1.131");
    }
    if (strlen(conf.ip_bombeo) < 10) {
     strcpy(conf.ip_bombeo,"192.168.1.132");
    }
     if (strlen(conf.ip_tmt4) < 10) {
     strcpy(conf.ip_tmt4,"192.168.1.140");
    }
    if (conf.time_thingspeak <= 0) {
      conf.time_thingspeak  = 30000;
    }
    if (conf.time_thingspeak2 <= 0) {
      conf.time_thingspeak2  = 30000;
    }
     
    //saveConfig();
   // conf.temp_mode=3; // control manual al inicio quitar en modo final
   
    DPRINTLN("Comenzando programa");
    StartWiFi();
     

  DPRINT("DevName :");
  DPRINTLN(conf.DevName);
  DPRINTLN("____________________________");
  
  Serial.println ( "" );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );

  if (!SPIFFS.begin())
  {
    DPRINTLN("SPIFFS Mount failed");
  } else {
    DPRINTLN("SPIFFS Mount succesfull");
  }




  }

void loadConfig() {
       if(!SPIFFS.begin()){
        DPRINTLN("Card Mount Failed");
        return;
        }
        File myFile = SPIFFS.open("/conf.txt", "r");
        
        myFile.read((byte *)&conf, sizeof(conf));
        myFile.close();
        DPRINTLN("leyendo configuracion");
        DPRINTLN(conf.DevName);
        //Serial.printf( "read: %s, %.5f\n", myStruct.someString, myStruct.someFloat );
}

void saveConfig() {
  DPRINTLN("saving config");
   if(!SPIFFS.begin()){
        DPRINTLN("Card Mount Failed");
        return;
    }
    File myFile = SPIFFS.open("/conf.txt", "w");
   
    myFile.write((byte *)&conf, sizeof(conf));
    myFile.close(); 
    DPRINTLN("guardando configuracion");
    DPRINTLN(conf.DevName); 
}
