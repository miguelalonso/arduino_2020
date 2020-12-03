/////////////////////////////////////////Puerta
 
void PuertaServoWeb(){
  mV_PH_ant=ciclo_actual;
  mV_PH=millis();
  PH_ant=PH;
  PH=spa.zenith;
  To_ant=To;
  mV_To=t_abierta;
  To=t_cerrada;
}



  
void controlPH(bool estado){
  //estado viene del control Timers
  //DPRINTLN("conf.PH_mode");DPRINTLN(conf.PH_mode);
  if (conf.PH_mode==0){AbrirPuerta(0);return;}
  if (conf.PH_mode==1){AbrirPuerta(1);return;}
  //if (conf.PH_mode==2){} //auto
  if (conf.PH_mode==3){//manual
     return;
    } 

 ////a verificar el tipo de control de la puerta
  if (hour() > conf.a && hour()<conf.b && spa.zenith<conf.c){
    // si estamos en el rago de hora de inicio y hora de fin
     ciclo_puerta();
  }else{
    AbrirPuerta(estado);
  }

//  if (spa.zenith>conf.c){
//    // si estamos a un angulo cenital mayor del configurado (de no che) cerrar la puerta
//     AbrirPuerta(0);
//  }
}

void AbrirPuerta(int e1){
  //Serial.print("Abrir puerta: ");Serial.println(e1);
  estados[0]=e1;
  //e1=!e1; //en Nodem van al revés
  digitalWrite(Relay_D0_EstadoPuerta,e1);
  //DPRINT("Abrir puerta: ");DPRINTLN(e1);

  unsigned long t_ini=millis();
  
          if (e1){//abrir la puerta
                if (!isWindowFullyOpen()) {
                  openWindow();
                }
                while (!isWindowFullyOpen()) {
                  delay(1);
                  if (millis()-t_ini > 20000){      // bail out on sensor detect
                    t_ini=millis();
                     //Serial.println("Error abriendo puerta, exit -1");
                     break;
                  }  
    
                }
          }
        
        if(!e1){//cerrar la puerta
           if (!isWindowFullyClose()) {
                  closeWindow();
                }                while (!isWindowFullyClose()) {
                  delay(1);
                  if (millis()-t_ini > 20000){      // bail out on sensor detect
                     t_ini=millis();
                    // Serial.println("Error cerrando puerta, exit -2");
                     break;
                  }  
                }
                
        }
 holdWindow();       
}

void ciclo_puerta(){
  int valor3=conf.PH_max*1000*60; 
  int valor4=conf.PH_min*1000*60; 

//  Serial.print("Time apertura (minutos): ");Serial.println(conf.PH_max);
//  Serial.print("Time cierre (minutos): ");Serial.println(conf.PH_min);
//   Serial.print("Ciclo_actual: ");Serial.println(ciclo_actual);
  
  if (ciclo_actual==1){
    t_abierta=millis()-t_inicio_abierta;
    if (t_abierta > valor3){
      t_abierta=0;
      t_inicio_cerrada=millis();
      t_inicio_abierta=millis();
      ciclo_actual=0;
      AbrirPuerta(0);
      //Serial.println("Puerta cerrada, ciclo 0");
    }
    }
    
  if (ciclo_actual==0){
    t_cerrada=millis()-t_inicio_cerrada;
    if (t_cerrada > valor4){
      t_cerrada=0;
      t_inicio_cerrada=millis();
      t_inicio_abierta=millis();
      ciclo_actual=1;
      AbrirPuerta(1);
      //Serial.println("Puerta abierta, ciclo 1");
    }
    }
   
}

  
void LeeFinalesCarrera(){
  
  puerta_alta = digitalRead(Input_D6_puerta_alta);
  puerta_baja = digitalRead(Input_D7_puerta_baja);

  //PULLUP - boya abierta  ==> se lee un 1
  //puerta_alta==1 rele NO activado
  //puerta_baja==1 rele baja No activado
  
 
}


void holdWindow() {
 // Serial.println("Hold window");
  if (servo.attached()) {
    //servo.write(90);
    servo.write(90);
    //delay(1000);
    delay(125);
    servo.detach();
    delay(1);
  }
}

void openWindow() {
  //Serial.println("Opening window");
  if (!servo.attached()) {
    servo.attach(SERVO_PIN);
    delay(1);
  }
  //servo.write(0);
  //servo.write(55);
  servo.write(75);
  delay(1);
}

void closeWindow() {
  //Serial.println("Closing window");
  if (!servo.attached()) {
    servo.attach(SERVO_PIN);
    delay(1);
  }
  servo.write(180);
  //servo.write(115);
  //servo.write(95);
  delay(1);
}


bool isPinHigh(int pin) {
  int val = digitalRead(pin);
  delay(1);
  bool result = val == HIGH ? true : false;
  return result;
}

bool isPinLow(int pin) {
  int val = digitalRead(pin);
  delay(1);
  bool result = val == LOW ? true : false;
  return result;
}

bool isWindowFullyOpen() {
  //bool result = !isPinHigh(FULLY_OPEN_SENSOR_PIN);
  bool result = isPinLow(FULLY_OPEN_SENSOR_PIN);
  if (result) Serial.println("Activado Final de Carrera ABIERTA");
  return result;
}

bool isWindowFullyClose() {
  //bool result = !isPinHigh(FULLY_CLOSE_SENSOR_PIN);
  bool result = isPinLow(FULLY_CLOSE_SENSOR_PIN);
  if (result) Serial.println("Activado Final de Carrera CERRADA");
  return result;
}
