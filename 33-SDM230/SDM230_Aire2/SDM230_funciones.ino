void control_aire(){

//if(!lect_valid){return;}

  if((millis()-timeOld5)<20000){
    return;
  }
  timeOld5=millis();
  
 //DPRINTLN("conf.temp_mode");DPRINTLN(conf.temp_mode);
  if (conf.temp_mode==0){control_sonoff(0, 0,0);return;}
  if (conf.temp_mode==1){control_sonoff(1, 1,1);return;}
  //if (conf.temp_mode==2){} //auto
  if (conf.temp_mode==3){return;} //manual


  float potencia_FV;

  //potencia_FV= inversor.Power+tmt4.Power; //sumar las demas cuando se tengan
  potencia_FV= inversor.Power+bombeo.Power+tmt4.Power; //sumar las demas cuando se tengan
 
  if (potencia_FV > conf.temp_max){
    control_sonoff(1,1,1);
    }
    if (potencia_FV < conf.temp_min){
     control_sonoff(1, 0,1);
    }
    
}

void control_sonoff(int inversor, int aire, int bombeo){
  
  if (!conf.control_aire){return;}
  
  if(inversor==1){
    //encender inversor
    control_inversor(1);
    }
    if(inversor==0){
      //apagar inversor
      //control_inversor(0);
      }

      if(aire==1){
    //encender aire http://192.168.1.72/cm?cmnd=Power%20on
    control_aire(1);
    }
    if(aire==0){
      //apagar aire http://192.168.1.72/cm?cmnd=Power%20off
      control_aire(0);
      }
    if(bombeo==1){
    //encender aire http://192.168.1.72/cm?cmnd=Power%20on
    control_bombeo(1);
    }
    if(bombeo==0){
      //apagar aire http://192.168.1.72/cm?cmnd=Power%20off
      control_bombeo(0);
      }
}

void meassureSDM() {
StaticJsonDocument <1024> JSONbuffer;
  if((millis()-timeOld7)<10000){
    return;
  }
  timeOld7=millis();
  
    
  float v = sdm.readVal(SDM230_VOLTAGE);                                       //read voltage [V]
  float c = sdm.readVal(SDM230_CURRENT);                                       //read current [A]
  float p = sdm.readVal(SDM230_POWER);                                         //read power [W]
  float f = sdm.readVal(SDM230_FREQUENCY);                                     //read frequency [Hz}
  float pf = sdm.readVal(SDM230_POWER_FACTOR);                                 //read Power Factor []
  float pa = sdm.readVal(SDM230_PHASE_ANGLE);                                  //read Phase Angle [Deg]
  float aap = sdm.readVal(SDM230_ACTIVE_APPARENT_POWER);                       //read Active Apparent Power [VA]
  float rap = sdm.readVal(SDM230_REACTIVE_APPARENT_POWER);                     //read Reactive Apparent Power [VAR]
  float iae = sdm.readVal(SDM230_IMPORT_ACTIVE_ENERGY);                        //read Import Active Energy [Wh]
  float eae = sdm.readVal(SDM230_EXPORT_ACTIVE_ENERGY);                        //read Export Active Energy [Wh]
  float ire = sdm.readVal(SDM230_IMPORT_REACTIVE_ENERGY);                      //read Import Active Energy [VARh]
  float ere = sdm.readVal(SDM230_EXPORT_REACTIVE_ENERGY);                      //read Export Active Energy [VARh]
  float tae = sdm.readVal(SDM230_TOTAL_ACTIVE_ENERGY);                         //read Export Active Energy [Wh]
  float tre = sdm.readVal(SDM230_TOTAL_REACTIVE_ENERGY);                       //read Export Active Energy [VARh]  
                      
                       
  float tspd = sdm.readVal(SDM230_TOTAL_SYSTEM_POWER_DEMAND);                         
  float mspd= sdm.readVal(SDM230_MAXIMUM_SYSTEM_POWER_DEMAND );                       
  float cppd = sdm.readVal( SDM230_CURRENT_POSITIVE_POWER_DEMAND); 
  float mppd= sdm.readVal( SDM230_MAXIMUM_POSITIVE_POWER_DEMAND );                      
  float crpd= sdm.readVal( SDM230_CURRENT_REVERSE_POWER_DEMAND );
  float mrpd= sdm.readVal( SDM230_MAXIMUM_REVERSE_POWER_DEMAND );
  float cd = sdm.readVal(SDM230_CURRENT_DEMAND);
  float mcd = sdm.readVal(SDM230_MAXIMUM_CURRENT_DEMAND);
  
  float crtae= sdm.readVal(SDM230_CURRENT_RESETTABLE_TOTAL_ACTIVE_ENERGY );   
  float crtre = sdm.readVal(SDM230_CURRENT_RESETTABLE_TOTAL_REACTIVE_ENERGY );   


//*********  JSon packaging ********************
  
  JSONbuffer["Voltage"]         = String(v)       + " V";
  JSONbuffer["Current"]         =  String(c)      + " A";
  JSONbuffer["Active_Power"]    =  String(p)      + " A";
  JSONbuffer["Freq"]            =  String(f)      + " Hz";
  JSONbuffer["PowerFactor"]     =  String(pf)     + " ";
  JSONbuffer["PhaseAngle"]     =  String(pa )     + " º";
  JSONbuffer["ActApparentPwr"]  =  String(aap)    + " W";
  JSONbuffer["RctApparentPwr"]  =  String(rap)    + " W"; 
  JSONbuffer["ImpActEnergy"]    =  String(iae)    + " kWh";  
  JSONbuffer["ExpActEnergy"]    =  String(eae)    + " kWh"; 
  JSONbuffer["ImpRctEnergy"]    =  String(ire)    + " kVArh";  
  JSONbuffer["ExpRctEnergy"]    =  String(ere)    + " kVArh";
  JSONbuffer["TotActEnergy"]    =  String(tae)    + " kWh";
  JSONbuffer["TotRctEnergy"]    =  String(tre)    + " kVArh";

   JSONbuffer["TotSysPowDemand"]  =  String(tspd)   + " W";
   JSONbuffer["MaxSysPowDemand"]  =  String(mspd)   + " W";
   JSONbuffer["CurPosPowDemand"]  =  String(cppd)   + " W";
   JSONbuffer["MaxPosPowDemand"]  =  String(mppd)   + " W";
   JSONbuffer["CurRevPowDemand"]  =  String(crpd)   + " W";
   JSONbuffer["MaxRevPowDemand"]  =  String(mrpd)   + " W";
   JSONbuffer["CurDemand"]        =  String(cd)     + " A";
   JSONbuffer["MaxCurDemand"]     =  String(mcd)    + " A";
   JSONbuffer["CurResTotActEn"]   =  String(crtae)  + " W";
   JSONbuffer["CurResTotReactEn"] =  String(crtre)  + " VAr";

        Active_Power=p;
        PowerFactor=pf;
        ImpActEnergy=iae;
        ExpActEnergy=eae;
        TotSysPowDemand=tspd;
        TotActEnergy=tae;
        TotRctEnergy=tre;
        PhaseAngle=pa;
      
   DPRINTLN("Medida...");
  //serializeJson(JSONbuffer, Serial);
  DPRINTLN("___...");
  DPRINTLN("dddd");
 DPRINTLN(Active_Power);
  json3="";
  serializeJson(JSONbuffer, json3);

}

void leer_estados_tasmotas(){
 
  if((millis()-timeOld3)<3000){
    return;
  }
  timeOld3=millis();
     
  yield();
  leer_estado_tasmotas();
  
}



//**************************************************//
//*************LEER HTTP TASMOTAS*******************//
//**************************************************//

void leer_http_tasmotas() {

  if((millis()-timeOld4)<20000){
    return;
  }
  timeOld4=millis();
  
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    
    if (http.begin(client, "http://"+String(conf.ip_inversor)+"/cm?cmnd=status%208")) {  // Inversor
      int httpCode = http.GET();
      if (httpCode > 0) {
        //Serial.printf("Tasmota Inversor abajo");
       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            payload_inversor = http.getString();
            printparameters( payload_inversor,1);
            lect_valid=true;
       }
      } 
            else {
         //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
         lect_valid=false;
      }
   
     http.end();
    }
    if (http.begin(client, "http://"+String(conf.ip_aire)+"/cm?cmnd=status%208")) {  // Aire
      int httpCode = http.GET();
      if (httpCode > 0) {
        //Serial.printf("Tasmota AIre");
       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        lect_valid=true;
            payload_aire = http.getString();
            printparameters( payload_aire,0);
       }
    } 
 
     http.end();
    }
     else {
         //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
         lect_valid=false;
         aire.Power=-999.99;
          }
          
          
        if (http.begin(client, "http://"+String(conf.ip_bombeo)+"/cm?cmnd=status%208")) {  // Bombeo
      int httpCode = http.GET();
      if (httpCode > 0) {
        //Serial.printf("Tasmota Bombeo");
       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            payload_bombeo = http.getString();
            printparameters( payload_bombeo,2);
       }
      } 
     http.end();
    }

       if (http.begin(client, "http://"+String(conf.ip_tmt4)+"/cm?cmnd=status%208")) {  // tasmota4
      int httpCode = http.GET();
      if (httpCode > 0) {
        //Serial.printf("Tasmota Bombeo");
       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            payload_tmt4 = http.getString();
            printparameters( payload_tmt4,3);
       }
      } 
     http.end();
    }

    
   
  }  //END

 
}

void printparameters(String payload,int device){
//{"StatusSNS":{"Time":"2020-06-29T22:22:24","ENERGY":{"TotalStartTime":"2020-06-24T09:25:29","Total":41.573,"Yesterday":6.147,"Today":8.286,"Power":2,"ApparentPower":22,"ReactivePower":21,"Factor":0.08,"Voltage":230,"Current":0.094}}}
   
            StaticJsonDocument<400> doc;
            DeserializationError error = deserializeJson(doc, payload);
            //serializeJson(doc, Serial);
            //DPRINTLN("*****************************************");
            //DPRINTLN("Response:");
            const char* cadena = doc["StatusSNS"]["Time"];
            //DPRINTLN(cadena);
            float Power= doc["StatusSNS"]["ENERGY"]["Power"];
            //DPRINT("Power :");//DPRINTLN(Power);
            float Total= doc["StatusSNS"]["ENERGY"]["Total"];
            //DPRINT("Total :");//DPRINTLN(Total);
            float Yesterday= doc["StatusSNS"]["ENERGY"]["Yesterday"];
            //DPRINT("Yesterday :");//DPRINTLN(Yesterday);
            float Today= doc["StatusSNS"]["ENERGY"]["Today"];
            //DPRINT("Today :");//DPRINTLN(Today);
            float ApparentPower= doc["StatusSNS"]["ENERGY"]["ApparentPower"];
            //DPRINT("ApparentPower :");//DPRINTLN(ApparentPower);
            float ReactivePower= doc["StatusSNS"]["ENERGY"]["ReactivePower"];
            //DPRINT("ReactivePower :");//DPRINTLN(ReactivePower);
            float Factor= doc["StatusSNS"]["ENERGY"]["Factor"];
            //DPRINT("Factor :");//DPRINTLN(Factor);
            float Voltage= doc["StatusSNS"]["ENERGY"]["Voltage"];
            //DPRINT("Voltage :");//DPRINTLN(Voltage);
            float Current= doc["StatusSNS"]["ENERGY"]["Current"];
            //DPRINT("Current :");//DPRINTLN(Current);

            if (device==1){
                inversor.Total=Total;
                inversor.Today=Today;
                inversor.Yesterday=Yesterday;
                inversor.Power=Power;
                inversor.ApparentPower=ApparentPower;
                inversor.ReactivePower=ReactivePower;
                inversor.Factor=Factor;
                inversor.Voltage=Voltage;
                inversor.Current=Current;
            }
            if (device==0){
                aire.Total=Total;
                aire.Today=Today;
                aire.Yesterday=Yesterday;
                aire.Power=Power;
                aire.ApparentPower=ApparentPower;
                aire.ReactivePower=ReactivePower;
                aire.Factor=Factor;
                aire.Voltage=Voltage;
                aire.Current=Current;
            }
        if (device==2){
                bombeo.Total=Total;
                bombeo.Today=Today;
                bombeo.Yesterday=Yesterday;
                bombeo.Power=Power;
                bombeo.ApparentPower=ApparentPower;
                bombeo.ReactivePower=ReactivePower;
                bombeo.Factor=Factor;
                bombeo.Voltage=Voltage;
                bombeo.Current=Current;
            }
         if (device==3){
                tmt4.Total=Total;
                tmt4.Today=Today;
                tmt4.Yesterday=Yesterday;
                tmt4.Power=Power;
                tmt4.ApparentPower=ApparentPower;
                tmt4.ReactivePower=ReactivePower;
                tmt4.Factor=Factor;
                tmt4.Voltage=Voltage;
                tmt4.Current=Current;
            }
 }

 


void control_aire(int estado) {
        if (estado!=aire_estado_anterior){
          aire_estado_anterior=estado;
          WiFiClient client;
          HTTPClient http;
          String comando;
          if (estado){
              comando="http://"+String(conf.ip_aire)+"/cm?cmnd=Power%20on";
            }else{
              comando="http://"+String(conf.ip_aire)+"/cm?cmnd=Power%20off";
            }
          DPRINTLN(comando);
          if (http.begin(client, comando)) {  
            int httpCode = http.GET();
            if (httpCode > 0) {
             DPRINTLN("Aire controlado");
           } 
           http.end();
          }
        }
}

void control_inversor(int estado) {
    if (estado!=inversor_estado_anterior){
          inversor_estado_anterior=estado;
          WiFiClient client;
          HTTPClient http;
          String comando;
        if (estado){
            comando="http://"+String(conf.ip_inversor)+"/cm?cmnd=Power%20on";
          }else{
            //comando="http://"+String(conf.ip_inversor)+"/cm?cmnd=Power%20off";
            comando="http://"+String(conf.ip_inversor)+"/cm?cmnd=Power%20on";
          }
        DPRINTLN(comando);
        if (http.begin(client, comando)) {  
          int httpCode = http.GET();
          if (httpCode > 0) {
           DPRINTLN("Inversor controlado");
         } 
         http.end();
        }
    }
}

void control_bombeo(int estado) {
    if (estado!=bombeo_estado_anterior){
          bombeo_estado_anterior=estado;
          WiFiClient client;
          HTTPClient http;
          String comando;
        if (estado){
            comando="http://"+String(conf.ip_bombeo)+"/cm?cmnd=Power%20on";
          }else{
            comando="http://"+String(conf.ip_bombeo)+"/cm?cmnd=Power%20off";
            //comando="http://"+String(conf.ip_bombeo)+"/cm?cmnd=Power%20on";
          }
        DPRINTLN(comando);
        if (http.begin(client, comando)) {  
          int httpCode = http.GET();
          if (httpCode > 0) {
           DPRINTLN("inversor de bombeo controlado");
         } 
         http.end();
        }
    }
}



void leer_estado_tasmotas() {
   
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    
    if (http.begin(client, "http://"+String(conf.ip_inversor)+"/cm?cmnd=status")) {  // Inversor
      int httpCode = http.GET();
      if (httpCode > 0) {
        //Serial.printf("Tasmota Inversor abajo");
       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload_inv = http.getString();
            printstatus( payload_inv,1);
       }
      } 
     http.end();
    }
    if (http.begin(client, "http://"+String(conf.ip_aire)+"/cm?cmnd=status")) {  // Aire
      int httpCode = http.GET();
      if (httpCode > 0) {
        //Serial.printf("Tasmota AIre");
       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload_air = http.getString();
            printstatus( payload_air,0);
       }
      } 
     http.end();
    }

     if (http.begin(client, "http://"+String(conf.ip_bombeo)+"/cm?cmnd=status")) {  // Aire
      int httpCode = http.GET();
      if (httpCode > 0) {
        //Serial.printf("Tasmota bombeo");
       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload_bombeo = http.getString();
            printstatus( payload_bombeo,2);
       }
      } 
     http.end();
    }
     
  }  //END

 
}

void printstatus(String payload,int device){
//{"Status":{"Module":43,"DeviceName":"Tasmota Aire","FriendlyName":["Tasmota Aire"],"Topic":"tasmota_506AF9","ButtonTopic":"0","Power":0,"PowerOnState":3,"LedState":1,"LedMask":"FFFF","SaveData":1,"SaveState":1,"SwitchTopic":"0","SwitchMode":[0,0,0,0,0,0,0,0],"ButtonRetain":0,"SwitchRetain":0,"SensorRetain":0,"PowerRetain":0}} 
            StaticJsonDocument<400> doc;
            DeserializationError error = deserializeJson(doc, payload);
            //serializeJson(doc, Serial);
            //DPRINTLN("*****************************************");
            //DPRINTLN("Response:");
            int estado = doc["Status"]["Power"];
           if (device==1){
                inversor.estado=estado;
                inversor_estado_anterior=estado;
            }
            if (device==0){
                aire.estado=estado;
                aire_estado_anterior=estado;
            }
            if (device==2){
                bombeo.estado=estado;
                bombeo_estado_anterior=estado;
            }
 }
 
