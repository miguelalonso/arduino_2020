void setupConfig(){
     setup_flowmeter();
  
    
    for ( int x = 0 ; x < 4 ; x++ ) {
      pinMode(GPIOPIN[x], OUTPUT);
      digitalWrite(GPIOPIN[x],HIGH);
      }
   digitalWrite(D4,HIGH);
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
    strcpy(conf.username,username);
    strcpy(conf.password,password);
    strcpy(conf.clientID,clientID);
   
    
    loadConfig();

    //conf.temp_mode=3; // control manual al inicio quitar en modo final
   
    DPRINTLN("Comenzando programa");
    StartWiFi();
    DPRINTLN("Comenzando programa");
    DPRINTLN(conf.username);
    
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
        loadcaudales();
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
