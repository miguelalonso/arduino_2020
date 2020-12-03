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
    strcpy(conf.DevName,"nombre del dispositivo");
    strcpy(conf.thingspeak_api_key,thingspeak_api_key);
    conf.time_thinkspeak           = 30000;
    strcpy(conf.ip_aire,"192.168.1.130");
    strcpy(conf.ip_inversor,"192.168.1.131");
    strcpy(conf.ip_bombeo,"192.168.1.132");
    
    loadConfig();

   if (strlen(conf.thingspeak_api_key) < 10) {
      strcpy(conf.thingspeak_api_key,thingspeak_api_key);
    }
    if (conf.time_thinkspeak <= 0) {
      conf.time_thinkspeak  = 30000;
    }
  //saveConfig();
   // conf.temp_mode=3; // control manual al inicio quitar en modo final
   
    DPRINTLN("Comenzando programa");
    StartWiFi();
    DPRINTLN("Comenzando programa");
    

  DPRINT("DevName :");
  DPRINTLN(conf.DevName);
  DPRINTLN("____________________________");
  DPRINT("DevName en conf :");
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
