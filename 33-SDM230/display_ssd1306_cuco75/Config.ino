void setupConfig(){

  Serial.begin ( 9600 );
  Serial.println("Starting...");
  init_display();
  
    strcpy(conf.pwd,"121212");
    conf.IP_1            = 129;
    conf.IP_2            = 6;
    conf.IP_3            = 15;
    conf.IP_4            = 28;
    conf. EEPROM_chk     =123;
    conf.Latitud         =40.41;
    conf.Longitud        =-3.731;
    strcpy(conf.DevName,"Display Cuco 75");
    
    
    strcpy(conf.ip_aire,"192.168.1.130");
    strcpy(conf.ip_inversor,"192.168.1.131");
    strcpy(conf.ip_bombeo,"192.168.1.132");
    strcpy(conf.ip_sdm230,"192.168.1.145");
    
    loadConfig();

  
    if (strlen(conf.ip_aire) < 10) {
      strcpy(conf.ip_aire,"192.168.1.130");
    }
    if (strlen(conf.ip_inversor) < 10) {
       strcpy(conf.ip_inversor,"192.168.1.131");
    }
    if (strlen(conf.ip_bombeo) < 10) {
     strcpy(conf.ip_bombeo,"192.168.1.132");
    }
     if (strlen(conf.ip_sdm230) < 10) {
     strcpy(conf.ip_sdm230,"192.168.1.145");
    }
     
  
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
