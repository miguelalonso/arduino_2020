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

  if((millis()-timeOld4)<25549){
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
       }
      } 
     http.end();
    }
    if (http.begin(client, "http://"+String(conf.ip_aire)+"/cm?cmnd=status%208")) {  // Aire
      int httpCode = http.GET();
      if (httpCode > 0) {
        //Serial.printf("Tasmota AIre");
       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            payload_aire = http.getString();
            printparameters( payload_aire,0);
       }
      } 
     http.end();
    }
        if (http.begin(client, "http://"+String(conf.ip_bombeo)+"/cm?cmnd=status%208")) {  // Bombeo
      int httpCode = http.GET();
      if (httpCode > 0) {
        //Serial.printf("Tasmota Bombeo");
       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            payload_bombeo = http.getString();
            printparameters( payload_bombeo,2);
            lect_valid=true;
       }
      } 
     http.end();
    }
    else {
         //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
         lect_valid=false;
         aire.Power=-999.99;
          }


         if (http.begin(client, "http://"+String(conf.ip_sdm230)+"/tabmesures3.json")) {  // PZEM016
      int httpCode = http.GET();
      if (httpCode > 0) {
        //Serial.printf("PZEM016");
       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            payload_sdm230 = http.getString();
            printparameters_sdm230( payload_sdm230);
       }
      } 
     http.end();
    }else{ //pzem fallo o apagado (apagado o fuera de cobertura)
            sdm230.Active_Power=-9.99;
           
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
 }

  void printparameters_sdm230(String payload){

//{"Voltage":"235.53 V","Current":"7.04 A","Active_Power":"-1549.42 A",
//"Freq":"50.00 Hz","PowerFactor":"-0.93 ","PhaseAngle":"202.74 º",
//"ActApparentPwr":"1687.03 W","RctApparentPwr":"-511.13 W","ImpActEnergy":"23.35 kWh",
//"ExpActEnergy":"97.06 kWh","ImpRctEnergy":"0.00 kVArh","ExpRctEnergy":"54.73 kVArh",
//"TotActEnergy":"120.40 kWh","TotRctEnergy":"54.73 kVArh","TotSysPowDemand":"760.04 W",
//"MaxSysPowDemand":"-2372.91 W","CurPosPowDemand":"15.81 W","MaxPosPowDemand":"1640.80 W",
//"CurRevPowDemand":"744.23 W","MaxRevPowDemand":"2372.91 W","CurDemand":"3.83 A",
//"MaxCurDemand":"10.18 A","CurResTotActEn":"120.40 W","CurResTotReactEn":"54.73 VAr"}
// leemos sólo los del smd, los de los demás se leeen desde aquí
            Serial.println(payload);
            
            StaticJsonDocument<5048> doc;
            DeserializationError error = deserializeJson(doc, payload);
            
            Serial.println("Comenzando deserialización");
            
//          serializeJson(doc, Serial);
//          Serial.println("arriba");
            const char* temp = doc["Voltage"];
            sdm230.Voltage=atof(temp);
            temp= doc["Current"];
            sdm230.Current=atof(temp);
            
            temp= doc["Active_Power"];
            sdm230.Active_Power=atof(temp);
            temp= doc["Active_Power"];
            sdm230.Active_Power=atof(temp);

            temp= doc["Freq"];
            sdm230.Freq=atof(temp);
            temp= doc["ImpActEnergy"];
            sdm230.ImpActEnergy=atof(temp);
            temp= doc["Voltage"];
            sdm230.Voltage=atof(temp);
            temp= doc["PowerFactor"];
            sdm230.PowerFactor=atof(temp);
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
 
