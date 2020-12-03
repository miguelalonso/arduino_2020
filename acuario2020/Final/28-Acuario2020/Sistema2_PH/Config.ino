void setupConfig(){
    for ( int x = 0 ; x < 5 ; x++ ) { ///D3,D6,D7;D8 y D4 para la bomba relleno, D3 es el CO2
    pinMode(GPIOPIN[x], OUTPUT);
    digitalWrite(GPIOPIN[x],HIGH);
  }

   pinMode(D0, INPUT);
   pinMode(D5, INPUT);
   
    Serial.begin ( 9600 );
    strcpy(conf.pwd,"121212");
      init_display();
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
    strcpy(conf.ip_emoncms,ip_emoncms);
    strcpy(conf.url_base,url_base);
    strcpy(conf.apikey,apikey);
    strcpy(conf.node,node);
    conf.time_emoncms           = 30000;
    
    loadConfig();

    if (strlen(conf.thingspeak_api_key) < 10) {
      strcpy(conf.thingspeak_api_key,thingspeak_api_key);
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
    if (strlen(conf.node) < 10) {
      strcpy(conf.node,node);
    }
    if (conf.time_emoncms <= 0) {
      conf.time_emoncms  = 30000;
    }
     if (conf.a == 0) {
      conf.a  = -0.00571;
    }
     if (conf.b == 0) {
      conf.b  = 21.92;
    }
    
   
    StartWiFi();
    DPRINTLN("Comenzando programa");
 
   
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
