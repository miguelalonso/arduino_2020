uint8_t GetByteFromBools(const bool eightBools[8])
{
   uint8_t ret = 0;
   for (int i=0; i<8; i++) if (eightBools[i] == true) ret |= (1<<i);
   return ret;
}

void Int2BoolArray(uint8_t theByte, bool eightBools[8])
{
   for (int i=0; i<8; i++) eightBools[i] = ((theByte & (1<<i)) != 0);
}
/*
 * uint8_t theByte=85;
bool eightBools[8];
DecodeByteIntoEightBools(theByte, eightBools);
Serial.println("binary bool");
for(int i =0;i<8;i++){
Serial.print(eightBools[i]);
}
int numero=GetByteFromBools( eightBools);
Serial.println("numero");
Serial.println(numero);

*/

void sendTime() {

  String hora;
  String Fecha;
    
    if (hour() < 10) hora="0";
    hora+=String(hour());
     hora+=":";
    if (minute() < 10) hora+="0";
     hora+=String(minute());
    hora+=":";
    if (second() < 10) hora+="0";
    hora+=String(second());
  
  
  Fecha = dayStr(weekday());
  Fecha+=" - "+String(day())+ ".";
  Fecha+=  String(monthStr(month()))+ "."+ String(year()) ;
   
  
  String json = "{\"hora\":\"" + hora + "\",";
  json += "\"Fecha\":\"" + Fecha + "\",";
  json += "\"pzem.power\":\"" + String(pzem.power2) + "\"}";

  server.send(200, "application/json", json);
  Serial.println("Fecha enviada");
  Serial.println(json);

 
}
 void digitalClockDisplay(){
  char digitalTime[20];
 //sprintf(digitalTime, "%4d-%02d-%02d %02d:%02d:%02d\n", year(), month(), day(), hour(), minute());  //display second resolution
  sprintf(digitalTime, "%4d-%02d-%02d %02d:%02d\n", year(), month(), day(), hour(), minute());   //display minute resolution
  //Blynk.virtualWrite(V10, digitalTime);
}

void sendDataTimers(){

 //DynamicJsonDocument doc(20000);
 //DynamicJsonDocument progs(2000);
 StaticJsonDocument<10000> doc;
 StaticJsonDocument<1024> progs;
 JsonArray programas = doc.createNestedArray("prog");
 
 
       String devID = server.arg("devID");
       int devIDi=devID.toInt()+5;
       int nr=devID.toInt();

       if(nr>Num_reles){
        Serial.println("error numero ID");
        return;
        }
   
       int hora;
       int minuto;
       char startTime[5];
       char endTime[5];


          doc["devName"]="Sistema3_ReleD"+String(devIDi);
          doc["ReleName"]=timers[nr].ReleName;
          doc["devID"]=devID;
          doc["Mode"]= timers[nr].Mode; //0:off,1:on,2:Auto

 
          for (j=0;j<7;j++){ 
            //DynamicJsonDocument progs(800);
            hora=timers[nr].ON_Time[j]/100;
            minuto=timers[nr].ON_Time[j]%100;
            sprintf(startTime, "%02d:%02d",  hora, minuto);
            progs["startTime"] =startTime;// "01:05";
                                              //OFF_Time ya no se utiliza, reserva
            hora=timers[nr].OFF_Time[j]/100;
            minuto=timers[nr].OFF_Time[j]%100;
            sprintf(endTime, "%02d:%02d",  hora, minuto);
            progs["endTime"] =endTime;// "01:05";
           
            if (timers[nr].progName[j]==""){
              progs["name"] = "Prog"+String(j)+"_DevID"+devIDi;
              }else{
                progs["name"] = timers[nr].progName[j];
              }
     
            progs["progID"]=j;
            progs["sun_set"]=timers[nr].sun_set[j];
            progs["sign_delay"]=timers[nr].sign_delay[j];
            progs["onoff"]=timers[nr].onoff[j];

            hora=timers[nr].aleatorio_Time[j]/100;
            minuto=timers[nr].aleatorio_Time[j]%100;
            sprintf(endTime, "%02d:%02d",  hora, minuto);
            progs["aleatorio"] =endTime;// "01:05";
            
            hora=timers[nr].delay_time[j]/100;
            minuto=timers[nr].delay_time[j]%100;
            sprintf(endTime, "%02d:%02d",  hora, minuto);
            progs["delay_time"] =endTime;// "01:05";

            JsonArray dias = progs.createNestedArray("days");
            for ( k=0;k<7;k++){
              dias.add(timers[nr].ON_Days[j][k]);
              }
            programas.add(progs);
            if(progs=="{}"){Serial.print("error Progs :");Serial.println(j);}

//            Serial.print("progs :");Serial.println(j);
//            serializeJson(progs, Serial);
//            Serial.println("");
        }
          
          
         String json;
         serializeJson(doc, json);
 
  
  server.send(200, "application/json", json);
  Serial.print("Datos de progrmas enviados: nr= "); Serial.println(nr);
  Serial.println(json);
        
 }

 void sendDataConf(){
    char salida[6];
    char puesta[6];
    int mm;
            mm = 60.0 * (spa.sunrise - (int)(spa.sunrise));
            sprintf(salida, "%02d:%02d",  (int)(spa.sunrise),(int)mm);
            conf.sunrise=(int)spa.sunrise*100+(int)mm;
            mm = 60.0 * (spa.sunset - (int)(spa.sunset));
            sprintf(puesta, "%02d:%02d",  (int)(spa.sunset),(int)mm);
            conf.sunset=(int)spa.sunset*100+(int)mm;

      //StaticJsonDocument<2900> doc;
      DynamicJsonDocument doc(3000);
      doc["version"] =      conf.version;
      doc["EEPROM_chk"]=    conf.EEPROM_chk;
      doc["DevName"] =      conf.DevName;
      doc["Latitud"] =      conf.Latitud;
      doc["Longitud"] =     conf.Longitud;
      doc["TimeZone"] =     conf.TimeZone;
      doc["IP_1"] =         conf.IP_1;
      doc["IP_2"] =         conf.IP_2;
      doc["IP_3"] =         conf.IP_3;
      doc["IP_4"] =         conf.IP_4;
      doc["username"] =     conf.username;
      doc["password"] =     conf.password;
      doc["pwd"] =          conf.pwd;
      doc["clientID"] =     conf.clientID;
      doc["sunrise"] =      salida;
      doc["sunset"] =       puesta;
      doc["temp_max"] =     conf.temp_max;
      doc["temp_min"] =     conf.temp_min;
      doc["temp_mode"] =    conf.temp_mode;
      doc["ip_aire"] =      conf.ip_aire;
      doc["ip_inversor"] =  conf.ip_inversor;
      doc["ip_bombeo"] =    conf.ip_bombeo;
    
      String json="";
      serializeJson(doc, json);
     
  server.send(200, "application/json", json);
  Serial.println("Datos de configuración enviados");
  Serial.println(json);
 }

 void save(){
        Serial.println("Recibido comando save");
//Request URL:
//http://192.168.1.84/
//save?cmd=save&nr=2&id=1&name=Prog1_DevID7&startTime=12:57&endTime=13:57&days=1
int f;

    int nr = server.arg("nr").toInt();
    int ID = server.arg("id").toInt();
    int dias = server.arg("days").toInt();
    String startTime = server.arg("startTime");
    String endTime = server.arg("endTime");
    String nombre = server.arg("name");

    int sun_set = server.arg("sun_set").toInt();
    timers[nr].sun_set[ID]=sun_set;
    int sign_delay = server.arg("sign_delay").toInt();
    timers[nr].sign_delay[ID]=sign_delay;
    int onoff = server.arg("onoff").toInt();
    timers[nr].onoff[ID]=onoff;
    String aleatorio = server.arg("aleatorio");
    String delay_time = server.arg("delay_time");
        
    f = startTime.indexOf(":");
    String hora = startTime.substring(0, f);
    String minuto = startTime.substring(f+1);
    timers[nr].ON_Time[ID]=hora.toInt()*100+minuto.toInt();
    f = endTime.indexOf(":");
    hora = endTime.substring(0, f);
    minuto = endTime.substring(f+1);
    timers[nr].OFF_Time[ID]=hora.toInt()*100+minuto.toInt();
    nombre.toCharArray(timers[nr].progName[ID], 30);
    Int2BoolArray(dias,timers[nr].ON_Days[ID]); //int a array bool

      f = aleatorio.indexOf(":");
      hora = aleatorio.substring(0, f);
      minuto = aleatorio.substring(f+1);
      timers[nr].aleatorio_Time[ID]=hora.toInt()*100+minuto.toInt();

      f = delay_time.indexOf(":");
      hora = delay_time.substring(0, f);
      minuto = delay_time.substring(f+1);
      timers[nr].delay_time[ID]=hora.toInt()*100+minuto.toInt();


   
      
    saveConfig();
    Serial.println(timers[nr].ON_Time[ID]);
    
   
    
  String success = "1";
  String json = "{\"name\":\"" + String(nombre) + "\",";
  json += "\"success\":\"" + String(success) + "\",";
  json += "\"horaON\":\"" + String(timers[nr].ON_Time[ID]) + "\"}";
  server.send(200, "application/json", json);
}

void savemode(){
        Serial.println("Recibido comando savemode");
/*
 *   $.post("savemode?cmd=" + 'savemode' +
          "&nr=" + nr+
          "&id=" + timers[nr].devID+
          "&name=" + timers[nr].ReleName+
          "&mode=" + timers[nr].Mode
 */
    int nr = server.arg("nr").toInt();
    int ID = server.arg("id").toInt();
    int mode = server.arg("mode").toInt();
    String nombre= server.arg("name");
    timers[nr].Mode=mode;
    nombre.toCharArray(timers[nr].ReleName, 30);
    saveConfig();
    String success = "1";
  String json = "{\"name\":\"" + String(nombre) + "\",";
  json += "\"conseguido\":\"" + String(success) + "\",";
  json += "\"success\":\"" + String(success) + "\"}";
  server.send(200, "application/json", json);
 }

void saveconfig_http(){
        Serial.println("Recibido comando saveconfig");
/*
 *   ?version=&EEPROM_chk=
 *   &DevName=&Latitud=&Longitud=
 *   &TimeZone=&IP_1=&IP_2=&IP_3=&IP_4=
 *   &username=&password=&clientID=&pwd=
 */
    
    
    String valor= server.arg("version");
    valor.toCharArray(conf.version, 4);
    conf.EEPROM_chk = server.arg("EEPROM_chk").toInt();
    valor= server.arg("DevName");
    valor.toCharArray(conf.DevName, 30);
    conf.Longitud = server.arg("Longitud").toInt();
    conf.Latitud = server.arg("Latitud").toInt();
    conf.TimeZone = server.arg("TimeZone").toInt();
    conf.IP_1 = server.arg("IP_1").toInt();
    conf.IP_2 = server.arg("IP_2").toInt();
    conf.IP_3 = server.arg("IP_3").toInt();
    conf.IP_4 = server.arg("IP_4").toInt();
    valor= server.arg("username");
    valor.toCharArray(conf.username, 50);
    valor= server.arg("password");
    valor.toCharArray(conf.password, 50);
    valor= server.arg("clientID");
    valor.toCharArray(conf.clientID, 50);
    valor= server.arg("pwd");
    valor.toCharArray(conf.pwd, 30);
    valor= server.arg("ip_aire");
    valor.toCharArray(conf.ip_aire, 16);
    valor= server.arg("ip_inversor");
    valor.toCharArray(conf.ip_inversor, 16);
    valor= server.arg("ip_bombeo");
    valor.toCharArray(conf.ip_bombeo, 16);
    conf.temp_max = server.arg("temp_max").toFloat();
    conf.temp_min = server.arg("temp_min").toFloat();
    conf.temp_mode = server.arg("temp_mode").toInt();
    //strcpy(conf.pwd,server.arg("pwd"));
   
    saveConfig();
    Serial.println("Guardando configuración");
    Serial.println(conf.pwd);
    
  String success = "1";
  String json = "{\"name\":\"" + String(conf.DevName) + "\",";
  json += "\"conseguido\":\"" + String(success) + "\",";
  json += "\"success\":\"" + String(success) + "\"}";
  server.send(200, "application/json", json);
  //server.sendHeader("Location", String("/#tab_configuration"), true);
  //server.send ( 302, "text/plain", "");
  server.send ( 200, "text/plain", "");
 }

void savetemp(){
    Serial.println("Recibido comando savetemp");
    conf.temp_max = server.arg("temp_max").toFloat();
    conf.temp_min = server.arg("temp_min").toFloat();
    saveConfig();
    String success = "1";
  String json = "{\"conf.temp_max\":\"" + String(conf.temp_max) + "\",";
  json += "\"conseguido\":\"" + String(success) + "\",";
  json += "\"success\":\"" + String(success) + "\"}";
  server.send(200, "application/json", json);
 }
 
 void print_horasolar(){
      char cadena[20];
    Serial.print("TimeOk: ");Serial.println(TimeOk);
    sprintf(cadena, "Hora UTC: %4d-%02d-%02d %02d:%02d\n", year(), month(), day(), hour(), minute());
    Serial.print(cadena);
    //    sprintf(cadena, "Azimuth: %4f\n", sun.dAzimuth-180);
    Serial.print(cadena);
    //  sprintf(cadena, "Zenith: %4f\n", sun.dZenithAngle);
    Serial.println(cadena);
    
    Serial.print("Angulos del SPA del NREL: ");
    sprintf(cadena, "%4d-%02d-%02d %02d:%02d\n", year(), month(), day(), hour(), minute());
    Serial.println(cadena);
    sprintf(cadena, "Julian Day: %4f\n", spa.jd);
    Serial.print(cadena);
    sprintf(cadena, "observer hour angle [degrees]: %4f\n", spa.h);
    Serial.print(cadena);
    sprintf(cadena, "Azimuth: %4f\n", spa.azimuth-180);
    Serial.print(cadena);
    sprintf(cadena, "Zenith: %4f\n", spa.zenith);
    Serial.print(cadena);
    sprintf(cadena, "surface incidence angle [degrees]: %4f\n", spa.incidence);
    Serial.print(cadena);
    sprintf(cadena, "local sun transit time (or solar noon) [fractional hour]: %4f\n", spa.suntransit);
    Serial.print(cadena);
    sprintf(cadena, "local sunrise time (+/- 30 seconds): %4f\n", spa.sunrise);
    Serial.print(cadena);
    sprintf(cadena, "local sunset time (+/- 30 seconds) : %4f\n", spa.sunset);
    Serial.print(cadena);

    float mint = 60.0 * (spa.sunrise - (int)(spa.sunrise));
    int sec = 60.0 * (mint - (int)mint);
    sprintf(cadena, "Sunrise: %02d:%02d:%02d\n", (int)spa.sunrise,(int)mint,sec);
    Serial.print(cadena);
 
    mint = 60.0 * (spa.sunset - (int)(spa.sunset));
    sec = 60.0 * (mint - (int)mint);
    sprintf(cadena, "Sunset: %02d:%02d:%02d\n", (int)spa.sunset,(int)mint,sec);
    Serial.print(cadena);
    conf.sunrise=(int)spa.sunrise+100*(int)mint;
    conf.sunset=(int)spa.sunset+100*(int)mint;
             
//    sprintf(cadena, "Diferencia Plataforma-NREL Acimut: %4f\n", sun.dAzimuth-spa.azimuth);
    Serial.print(cadena);
 //   sprintf(cadena, "Diferencia Plataforma-NREL Cenit: %4f\n", sun.dZenithAngle-spa.zenith);
    Serial.println(cadena);
    
  
 }


void updateGpio(){
  String gpio = server.arg("id");
  String etat = server.arg("etat");
  String success = "1";
  int pin = D4;
  int num_pin=5;
 if ( gpio == "D0" ) {
      pin = D0;
      num_pin=0;
 } else if ( gpio == "D3" ) {
     pin = D3;
     num_pin=1;
 } else if ( gpio == "D7" ) {
     pin = D7;  
     num_pin=2;
  } else if ( gpio == "D8" ) {
     pin = D8;
     num_pin=3;
 } 

          //conf.temp_mode modo de control on, off, auto, man(3) --> coólo en manual
          if ( gpio == "inversor" && conf.temp_mode==3 ) {
                if ( etat == "1"){
                  //encender inversor
                  control_inversor(1);
                }
                if ( etat == "0"){
                  //encender inversor
                  control_inversor(0);
                }
           }
         
        if ( gpio == "aire" && conf.temp_mode==3 ) {
              if ( etat == "1"){
                //encender aire
                control_aire(1);
              }
              if ( etat == "0"){
                //encender inversor
                control_aire(0);
              }
         }

         if ( gpio == "bombeo" && conf.temp_mode==3 ) {
              if ( etat == "1"){
                //encender bombeo
                control_bombeo(1);
              }
              if ( etat == "0"){
                //encender inversor
                control_bombeo(0);
              }
         }
 
 
 
  Serial.println(pin);
  if ( etat == "1" && num_pin<4 ) {
    digitalWrite(pin, LOW);
    estados[num_pin]=true;
  } else if ( etat == "0" && num_pin<4) {
    digitalWrite(pin, HIGH);
    estados[num_pin]=false;
  } else {
    success = "1";
    Serial.println("Error Salida Digital");
  }
  
  String json = "{\"gpio\":\"" + String(gpio) + "\",";
  json += "\"etat\":\"" + String(etat) + "\",";
  json += "\"success\":\"" + String(success) + "\"}";
    
  server.send(200, "application/json", json);
  Serial.println("GPIO Salidas digitales actuadas");
}

void sendMesures() {
  
  String json = "{\"pzem_power\":\"" + String(pzem.power2) + "\",";
  json += "\"inversor_Power\":\"" + String(inversor.Power) + "\",";
  json += "\"bombeo_Power\":\"" + String(bombeo.Power) + "\",";
  json += "\"aire_Power\":\"" + String(aire.Power) + "\"}";

  server.send(200, "application/json", json);
  Serial.println("Medidas enviadas");
  Serial.println(json);
}

void sendGPIO() {
  String json = "{\"D0\":\"" + String(estados[0]) + "\",";
  json += "\"D3\":\"" + String(estados[1]) + "\",";
  json += "\"D7\":\"" + String(estados[2]) + "\",";
  json += "\"D8\":\"" + String(estados[3]) + "\"}";
 

  server.send(200, "application/json", json);
  Serial.println("GPIO enviado");
  Serial.println(json);
}


void sendTabMesures() {
  double temp = pzem.voltage;
  String json = "[";
  json += "{\"medida\":\"Temperatura\",\"valor\":\"" + String(pzem.voltage) + "\",\"unidad\":\"°C\",\"precedente\":\"" + String(temp) + "\"},";
  temp = pzem.voltage;        
  json += "{\"medida\":\"mV_TDS\",\"valor\":\"" + String(pzem.voltage) + "\",\"unidad\":\"mV\",\"precedente\":\"" + String(temp) + "\"},";
  temp = pzem.voltage;     
  json += "{\"medida\":\"TDS\",\"valor\":\"" + String(pzem.voltage) + "\",\"unidad\":\"ppm\",\"precedente\":\"" + String(temp) + "\"}";
  json += "]";
  server.send(200, "application/json", json);
  Serial.println("Tabla de medidas enviada");
}

void sendTabMesures2() {

      DynamicJsonDocument doc(3000);
      doc["pzem_voltage"]       = String(pzem.voltage) + " V";
      doc["pzem_current"]       = String(pzem.current)+ " A";
      doc["pzem_power"]         = String(pzem.power) + " W";
      doc["pzem_power2"]         = String(pzem.power2) + " W";
      doc["pzem_frequency"]     = String(pzem.frequency) + " Hz";
      doc["pzem_power_factor"]  = String(pzem.power_factor) + "";
      doc["inversor_Total"]           =  String(inversor.Total) + " kWh";
      doc["inversor_Today"]           =  String(inversor.Today) + " kWh";
      doc["inversor_Yesterday"]       =  String(inversor.Yesterday) + " kWh";
      doc["inversor_Power"]           =  String(inversor.Power) + " W";
      doc["inversor_ApparentPower"]   =  String(inversor.ApparentPower) + " VA";
      doc["inversor_ReactivePower"]   =  String(inversor.ReactivePower) + " VAr";
      doc["inversor_Factor"]          =  String(inversor.Factor) + "";
      doc["inversor_Voltage"]         =  String(inversor.Voltage) + " V";
      doc["inversor_Current"]         =  String(inversor.Current) + " A";
      doc["aire_Total"]           =  String(aire.Total) + " kWh";
      doc["aire_Today"]           =  String(aire.Today) + " kwh";
      doc["aire_Yesterday"]       =  String(aire.Yesterday) + " kWh";
      doc["aire_Power"]           =  String(aire.Power) + " W";
      doc["aire_ApparentPower"]   =  String(aire.ApparentPower) + " VA";
      doc["aire_ReactivePower"]   =  String(aire.ReactivePower) + " VAr";
      doc["aire_Factor"]          =  String(aire.Factor) + "";
      doc["aire_Voltage"]         =  String(aire.Voltage) + " V";
      doc["aire_Current"]         =  String(aire.Current) + " A";
      doc["bombeo_Total"]           =  String(bombeo.Total) + " kWh";
      doc["bombeo_Today"]           =  String(bombeo.Today) + " kwh";
      doc["bombeo_Yesterday"]       =  String(bombeo.Yesterday) + " kWh";
      doc["bombeo_Power"]           =  String(bombeo.Power) + " W";
      doc["bombeo_ApparentPower"]   =  String(bombeo.ApparentPower) + " VA";
      doc["bombeo_ReactivePower"]   =  String(bombeo.ReactivePower) + " VAr";
      doc["bombeo_Factor"]          =  String(bombeo.Factor) + "";
      doc["bombeo_Voltage"]         =  String(bombeo.Voltage) + " V";
      doc["bombeo_Current"]         =  String(bombeo.Current) + " A";

      doc["aire_estado"]         =  aire.estado;
      doc["inversor_estado"]      =  inversor.estado;
      doc["bombeo_estado"]      =  bombeo.estado;
      
    
      String json="";
      serializeJson(doc, json);
     
  server.send(200, "application/json", json);
  Serial.println("Datos de medidas enviados");
  Serial.println(json);
}

void horasolar(){
            spa.year = year();
            spa.month = month();
            spa.day = day();
            spa.hour = hour();
            spa.minute = minute();
            spa.second = second();
            spa.timezone = 0.0;
            spa.delta_ut1 = 0;
            spa.delta_t = 67;
            spa.longitude = conf.Longitud;
            spa.latitude = conf.Latitud;
            spa.elevation = 680;
            spa.pressure = 820;
            spa.temperature = 11;
            spa.slope = 30;
            spa.azm_rotation = 0;
            spa.atmos_refract = 0.5667;
            spa.function = SPA_ALL;
            spa_calculate(&spa);    
   }

 
