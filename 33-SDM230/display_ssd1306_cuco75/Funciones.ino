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

*/

void sendTime() {

  
    
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
  json += "\"sdm230.power\":\"" + String(sdm230.Active_Power) + "\"}";

  server.send(200, "application/json", json);
  DPRINTLN("Fecha enviada");
  DPRINTLN(json);

 
}
 void digitalClockDisplay(){
  char digitalTime[20];
 //sprintf(digitalTime, "%4d-%02d-%02d %02d:%02d:%02d\n", year(), month(), day(), hour(), minute());  //display second resolution
  sprintf(digitalTime, "%4d-%02d-%02d %02d:%02d\n", year(), month(), day(), hour(), minute());   //display minute resolution
  //Blynk.virtualWrite(V10, digitalTime);
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
      doc["pwd"] =          conf.pwd;
      doc["sunrise"] =      salida;
      doc["sunset"] =       puesta;
      doc["temp_max"] =     conf.temp_max;
      doc["temp_min"] =     conf.temp_min;
      doc["temp_mode"] =    conf.temp_mode;
      doc["ip_aire"] =      conf.ip_aire;
      doc["ip_inversor"] =  conf.ip_inversor;
      doc["ip_bombeo"] =    conf.ip_bombeo;
      doc["ip_sdm230"] =    conf.ip_sdm230;
    
      String json="";
      serializeJson(doc, json);
     
  server.send(200, "application/json", json);
  DPRINTLN("Datos de configuración enviados");
  DPRINTLN(json);
 }



void saveconfig_http(){
        DPRINTLN("Recibido comando saveconfig");
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
    conf.Longitud = server.arg("Longitud").toFloat();
    conf.Latitud = server.arg("Latitud").toFloat();
    conf.TimeZone = server.arg("TimeZone").toFloat();
    conf.IP_1 = server.arg("IP_1").toInt();
    conf.IP_2 = server.arg("IP_2").toInt();
    conf.IP_3 = server.arg("IP_3").toInt();
    conf.IP_4 = server.arg("IP_4").toInt();
    conf.IP_4 = server.arg("IP_4").toInt();
    
    valor= server.arg("pwd");
    valor.toCharArray(conf.pwd, 30);
    valor= server.arg("ip_aire");
    valor.toCharArray(conf.ip_aire, 16);
    valor= server.arg("ip_inversor");
    valor.toCharArray(conf.ip_inversor, 16);
    valor= server.arg("ip_bombeo");
    valor.toCharArray(conf.ip_bombeo, 16);
   
    //strcpy(conf.pwd,server.arg("pwd"));
   
    saveConfig();
    DPRINTLN("Guardando configuración");
    DPRINTLN(conf.pwd);
    
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
    DPRINTLN("Recibido comando savetemp");
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
    DPRINT("TimeOk: ");DPRINTLN(TimeOk);
    sprintf(cadena, "Hora UTC: %4d-%02d-%02d %02d:%02d\n", year(), month(), day(), hour(), minute());
    DPRINT(cadena);
    //    sprintf(cadena, "Azimuth: %4f\n", sun.dAzimuth-180);
    DPRINT(cadena);
    //  sprintf(cadena, "Zenith: %4f\n", sun.dZenithAngle);
    DPRINTLN(cadena);
    
    DPRINT("Angulos del SPA del NREL: ");
    sprintf(cadena, "%4d-%02d-%02d %02d:%02d\n", year(), month(), day(), hour(), minute());
    DPRINTLN(cadena);
    sprintf(cadena, "Julian Day: %4f\n", spa.jd);
    DPRINT(cadena);
    sprintf(cadena, "observer hour angle [degrees]: %4f\n", spa.h);
    DPRINT(cadena);
    sprintf(cadena, "Azimuth: %4f\n", spa.azimuth-180);
    DPRINT(cadena);
    sprintf(cadena, "Zenith: %4f\n", spa.zenith);
    DPRINT(cadena);
    sprintf(cadena, "surface incidence angle [degrees]: %4f\n", spa.incidence);
    DPRINT(cadena);
    sprintf(cadena, "local sun transit time (or solar noon) [fractional hour]: %4f\n", spa.suntransit);
    DPRINT(cadena);
    sprintf(cadena, "local sunrise time (+/- 30 seconds): %4f\n", spa.sunrise);
    DPRINT(cadena);
    sprintf(cadena, "local sunset time (+/- 30 seconds) : %4f\n", spa.sunset);
    DPRINT(cadena);

    float mint = 60.0 * (spa.sunrise - (int)(spa.sunrise));
    int sec = 60.0 * (mint - (int)mint);
    sprintf(cadena, "Sunrise: %02d:%02d:%02d\n", (int)spa.sunrise,(int)mint,sec);
    DPRINT(cadena);
 
    mint = 60.0 * (spa.sunset - (int)(spa.sunset));
    sec = 60.0 * (mint - (int)mint);
    sprintf(cadena, "Sunset: %02d:%02d:%02d\n", (int)spa.sunset,(int)mint,sec);
    DPRINT(cadena);
    conf.sunrise=(int)spa.sunrise+100*(int)mint;
    conf.sunset=(int)spa.sunset+100*(int)mint;
             
//    sprintf(cadena, "Diferencia Plataforma-NREL Acimut: %4f\n", sun.dAzimuth-spa.azimuth);
    DPRINT(cadena);
 //   sprintf(cadena, "Diferencia Plataforma-NREL Cenit: %4f\n", sun.dZenithAngle-spa.zenith);
    DPRINTLN(cadena);
    
  
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
                 
                }
                if ( etat == "0"){
                  //encender inversor
                  
                }
           }
         
        if ( gpio == "aire" && conf.temp_mode==3 ) {
              if ( etat == "1"){
                //encender aire
                
              }
              if ( etat == "0"){
                //encender inversor
               
              }
         }

         if ( gpio == "bombeo" && conf.temp_mode==3 ) {
              if ( etat == "1"){
                //encender bombeo
                
              }
              if ( etat == "0"){
                //encender inversor
                
              }
         }
 
 
 
  DPRINTLN(pin);
  if ( etat == "1" && num_pin<4 ) {
    digitalWrite(pin, LOW);
    estados[num_pin]=true;
  } else if ( etat == "0" && num_pin<4) {
    digitalWrite(pin, HIGH);
    estados[num_pin]=false;
  } else {
    success = "1";
    DPRINTLN("Error Salida Digital");
  }
  
  String json = "{\"gpio\":\"" + String(gpio) + "\",";
  json += "\"etat\":\"" + String(etat) + "\",";
  json += "\"success\":\"" + String(success) + "\"}";
    
  server.send(200, "application/json", json);
  DPRINTLN("GPIO Salidas digitales actuadas");
}

void sendMesures() {
  String json = "{\"pzem_power\":\"" + String(sdm230.Active_Power) + "\",";
  json += "\"inversor_Power\":\"" + String(inversor.Power) + "\",";
  json += "\"bombeo_Power\":\"" + String(bombeo.Power) + "\",";
  json += "\"SDM230_Power\":\"" + String(Active_Power) + "\",";
  json += "\"aire_Power\":\"" + String(aire.Power) + "\"}";
  server.send(200, "application/json", json);
  DPRINTLN("Medidas enviadas");
  DPRINTLN(json);
}

void sendGPIO() {
  String json = "{\"D0\":\"" + String(estados[0]) + "\",";
  json += "\"D3\":\"" + String(estados[1]) + "\",";
  json += "\"D7\":\"" + String(estados[2]) + "\",";
  json += "\"D8\":\"" + String(estados[3]) + "\"}";
  server.send(200, "application/json", json);
  DPRINTLN("GPIO enviado");
  DPRINTLN(json);
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

                char salida[6];
                char puesta[6];
                int mm;
            mm = 60.0 * (spa.sunrise - (int)(spa.sunrise));
            sprintf(salida, "%02d:%02d",  (int)(spa.sunrise),(int)mm);
            conf.sunrise=(int)spa.sunrise*100+(int)mm;
            mm = 60.0 * (spa.sunset - (int)(spa.sunset));
            sprintf(puesta, "%02d:%02d",  (int)(spa.sunset),(int)mm);
            conf.sunset=(int)spa.sunset*100+(int)mm;
            
      hora="";
      Fecha="";
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
  
   }

 
