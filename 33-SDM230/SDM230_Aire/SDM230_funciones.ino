void control_aire(){

//if(!pzem_lect_valid){return;}

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

  //potencia_FV= inversor.Power+pzem.power; //sumar las demas cuando se tengan
  potencia_FV= inversor.Power+bombeo.Power; //sumar las demas cuando se tengan
 
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
      control_inversor(0);
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


void leer_SDM230(){

  switch (u8RequestState) {
    case IDLE_STATE:
      if (millis() > u32wait) {
        u8RequestState = DO_REQUEST_STATE;
      }
//      else {
//        server.handleClient();
//      }
      break;
    case DO_REQUEST_STATE:
      // Toggle the coil at address 0x0002 (Manual Load Control)
      node.writeSingleCoil(0x0002, state);
      state = !state;
      u8RequestState = RECEIVING_STATE;
      break;

    case RECEIVING_STATE: {
       
          DPRINTLN(codes[currentParamIdx].label);
           DPRINTLN(currentParamIdx);
       
        ESP.wdtDisable();
        uint8_t res = node.readInputRegisters(codes[currentParamIdx].code, 2);
        if (res == node.ku8MBSuccess) {
          u8RequestState = DATA_READY_STATE;
          ESP.wdtEnable(10);
        }
      }
      break;

    case DATA_READY_STATE: {
        float v = getValue();
        data[currentParamIdx] = v;
        Serial.println(currentParamIdx);
         Serial.println(v);
        currentParamIdx++;
        u8RequestState = DO_REQUEST_STATE;
        if (currentParamIdx == CODES_SIZE) {
          Serial.println("CODES_SIZE");
          Serial.println(CODES_SIZE);
          Serial.println("currentParamIdx");
          Serial.println(currentParamIdx);
          Serial.println("---------------------------------------------");
            for (int i = 0; i < currentParamIdx; i++) {
              Serial.print(codes[i].label);
              Serial.print(": ");
              Serial.print(data[i], 3);
             Serial.print(" ");
             Serial.println(codes[i].uom);
           }
           // Serial.println("---------------------------------------------");
         
          currentParamIdx = 0;
          u8RequestState = IDLE_STATE;
          u32wait = millis() + RDELAY;
          Serial.print("u32wait :"); Serial.println(u32wait);
          //asignar valores en array data a la estructura SDM230
          SDM230.Voltage=data[0];
          SDM230.Current=data[1];
          SDM230.Active_Power=data[2];
          SDM230.Apparent_Power=data[3];
          SDM230.Reactive_Power=data[4];
          SDM230.Power_Factor=data[5];
          SDM230.Phase_Angle=data[6];
          SDM230.Frequency=data[7];
          SDM230.Import_active_energy=data[8];
          SDM230.Export_active_energy=data[9];
          SDM230.Import_reactive_energy=data[10];
          SDM230.Export_reactive_energy=data[11];
          SDM230.TOTAL_SYSTEM_POWER_DEMAND=data[12];
          SDM230.MAXIMUM_SYSTEM_POWER_DEMAND=data[13];
          SDM230.CURRENT_POSITIVE_POWER_DEMAND=data[14];
          SDM230.MAXIMUM_POSITIVE_POWER_DEMAND=data[15];
          SDM230.CURRENT_REVERSE_POWER_DEMAND=data[16];
          SDM230.MAXIMUM_REVERSE_POWER_DEMAND=data[17];
          SDM230.CURRENT_DEMAND=data[18];
          SDM230.MAXIMUM_CURRENT_DEMAND=data[19];
          SDM230.TOTAL_ACTIVE_ENERGY=data[20];
          SDM230.TOTAL_REACTIVE_ENERGY=data[21];
          SDM230.CURRENT_RESETTABLE_TOTAL_ACTIVE_ENERGY=data[22];
          SDM230.CURRENT_RESETTABLE_TOTAL_REACTIVE_ENERGY=data[23]; 
        }
      }
      break;
  }

  if((millis()-timeOld3)<3000){
    return;
  }
  timeOld3=millis();
     
  yield();
  leer_estado_tasmotas();
  
}

float getValue() {
  uint16_t arr[2];
  for (int i = 0; i < 2; i++) {
    uint16_t v = node.getResponseBuffer(i);
    
    int a = 1;
    if (i == 1) {
      a = 0;
    }
    arr[a] = v;
  }
  uint32_t *val = (uint32_t *)arr;
  return convert(*val);
}

float convert(uint32_t x) {
  float y = *(float*)&x;
  return y;
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
       }
      } 
     http.end();
    }


         if (http.begin(client, "http://"+String(conf.ip_pzem)+"/tabmesures.json")) {  // PZEM016
      int httpCode = http.GET();
      if (httpCode > 0) {
        //Serial.printf("PZEM016");
       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            payload_pzem = http.getString();
            printparameters_pzem( payload_pzem);
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
 }

  void printparameters_pzem(String payload){
// {"pzem_voltage":"229.60 V","pzem_current":"1.93 A","pzem_power":"258.40 W","pzem_power2":"442.67 W","pzem_frequency":"50.00 Hz","pzem_power_factor":"0.58","inversor_Total":"372.40 kWh","inversor_Today":"1.54 kWh","inversor_Yesterday":"10.16 kWh","inversor_Power":"4.00 W","inversor_ApparentPower":"35.00 VA","inversor_ReactivePower":"34.00 VAr","inversor_Factor":"0.11","inversor_Voltage":"230.00 V","inversor_Current":"0.15 A","aire_Total":"247.24 kWh","aire_Today":"1.62 kwh","aire_Yesterday":"6.20 kWh","aire_Power":"0.00 W","aire_ApparentPower":"0.00 VA","aire_ReactivePower":"0.00 VAr","aire_Factor":"0.00","aire_Voltage":"0.00 V","aire_Current":"0.00 A","bombeo_Total":"91.70 kWh","bombeo_Today":"1.90 kwh","bombeo_Yesterday":"3.51 kWh","bombeo_Power":"0.00 W","bombeo_ApparentPower":"0.00 VA","bombeo_ReactivePower":"0.00 VAr","bombeo_Factor":"0.00","bombeo_Voltage":"230.00 V","bombeo_Current":"0.00 A","aire_estado":0,"inversor_estado":1,"bombeo_estado":1}
// leemos sólo los del pzem, los de los demás se leeen desde aquí
//            Serial.println(payload);
            
            StaticJsonDocument<400> doc;
            DeserializationError error = deserializeJson(doc, payload);
//            serializeJson(doc, Serial);
//            Serial.println("arriba");
            const char* temp = doc["pzem_voltage"];
            pzem.voltage=atof(temp);
            temp= doc["pzem_current"];
            pzem.current=atof(temp);
            
            temp= doc["pzem_power"];
            pzem.power=atof(temp);
            temp= doc["pzem_power2"];
            pzem.power2=atof(temp);

            temp= doc["pzem_frequency"];
            pzem.frequency=atof(temp);
            temp= doc["pzem_power_factor"];
            pzem.power_factor=atof(temp);
            temp= doc["pzem_energy"];
            pzem.energy=atof(temp);
           
        
 }

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
 
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
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
            comando="http://"+String(conf.ip_inversor)+"/cm?cmnd=Power%20off";
            //comando="http://"+String(conf.ip_inversor)+"/cm?cmnd=Power%20on";
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
 
