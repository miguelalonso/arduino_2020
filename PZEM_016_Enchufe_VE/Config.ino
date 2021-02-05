void setupConfig(){

  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
  
  mySerial.begin(9600, SWSERIAL_8N1, D5, D6, false, 95, 11);
  node.begin(1, mySerial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  
    for ( int x = 0 ; x < 4 ; x++ ) {
    pinMode(GPIOPIN[x], OUTPUT);
    digitalWrite(GPIOPIN[x],HIGH);
  }
  
  Serial.begin ( 9600 );
  
    strcpy(conf.pwd,"121212");
    conf.IP_1            = 129;
    conf.IP_2            = 6;
    conf.IP_3            = 15;
    conf.IP_4            = 28;
    conf. EEPROM_chk     =123;
    conf.Latitud         =40.41;
    conf.Longitud        =-3.731;
    strcpy(conf.DevName,"PZEM_Enche_VE");
    strcpy(conf.thingspeak_api_key,thingspeak_api_key);
    conf.time_thinkspeak           = 30000;
    strcpy(conf.ip_aire,"192.168.1.130");
    strcpy(conf.ip_inversor,"192.168.1.131");
    strcpy(conf.ip_bombeo,"192.168.1.132");
    strcpy(conf.ip_emoncms,ip_emoncms);
    strcpy(conf.url_base,url_base);
    strcpy(conf.apikey,apikey);
    strcpy(conf.node_emon,node_emon);
    conf.time_emoncms           = 30000;
    
    loadConfig();

   if (strlen(conf.thingspeak_api_key) < 10) {
      strcpy(conf.thingspeak_api_key,thingspeak_api_key);
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
    if (conf.time_thinkspeak <= 0) {
      conf.time_thinkspeak  = 30000;
    }

  if (strlen(conf.apikey) < 10) {
      strcpy(conf.apikey,apikey);
    }
    if (strlen(conf.ip_emoncms) < 10) {
      strcpy(conf.ip_emoncms,ip_emoncms);
    }
    if (strlen(conf.url_base) < 10) {
      strcpy(conf.url_base,url_base);
    }
    if (strlen(conf.node_emon) < 10) {
      strcpy(conf.node_emon,node_emon);
    }
    if (conf.time_emoncms <= 0) {
      conf.time_emoncms  = 30000;
    }
     if (conf.a == 0) {
      conf.a  = 1;
    }
     if (conf.b == 0) {
      conf.b  = 0;
    }

     if (conf.a2 == 0) { conf.a2  = 1; }
     if (conf.b2 == 0) { conf.b2  = 0; }
     if (conf.a3 == 0) { conf.a3  = 1; }
     if (conf.b3 == 0) { conf.b3  = 0; }
     if (conf.a4 == 0) { conf.a4  = 1; }
     if (conf.b4 == 0) { conf.b4  = 0; }

    
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
        myFile.read((byte *)&timers, sizeof(timers));
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
    myFile.write((byte *)&timers, sizeof(timers));
    myFile.write((byte *)&conf, sizeof(conf));
    myFile.close(); 
    DPRINTLN("guardando configuracion");
    DPRINTLN(conf.DevName); 
}
