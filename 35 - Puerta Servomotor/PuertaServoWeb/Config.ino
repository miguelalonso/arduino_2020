void setupConfig(){
    for ( int x = 0 ; x < 4 ; x++ ) { ///D0,D3,D4;D8  D0 es el estado de la puerta
    pinMode(GPIOPIN[x], OUTPUT);
    digitalWrite(GPIOPIN[x],HIGH);
  }

   pinMode(D6, INPUT);
   pinMode(D7, INPUT);


   servo.attach(D5); // attaches the servo on pin 8 to the servo object amotor
    pinMode(FULLY_OPEN_SENSOR_PIN, INPUT); //repetido??
    pinMode(FULLY_CLOSE_SENSOR_PIN, INPUT);
    holdWindow();  

   
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
     if (conf.a == 0) {  //en conf.a viene la hora de inicio del ciclo
      conf.a  = 9;
    }
     if (conf.b == 0) { //en conf.b viene la hora de fin del ciclo
      conf.b  = 22;
    }

     if (conf.c == 0) { //en conf.c viene el ángulo cenital fin de ciclo
      conf.c  = 95;
    }

    if (conf.PH_max  == 0) {  //en conf.PH_max viene el tiempo abierto en el ciclo
      conf.PH_max   = 10;
    }
    if (conf.PH_min  == 0) {  //en conf.PH_min viene el tiempo cerrado del ciclo
      conf.PH_min   = 10;
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