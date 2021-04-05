/////////////////////////////////////////Puerta

 //para el control del servomotor con PCA9685
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
  //Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  //Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000000;  // convert input seconds to us
  pulse /= pulselength;
  //Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}


void PuertaServoWeb(){
  mV_PH_ant=ciclo_actual;
  mV_PH=millis()/1000;
  PH_ant=PH;
  PH=spa.zenith;
  To_ant=To;
  mV_To=t_abierta;
  To=t_cerrada;
}



  
void controlPH(bool estado){

  //estado viene del control Timers
  //DPRINTLN("conf.PH_mode");DPRINTLN(conf.PH_mode);
  if (conf.PH_mode==0){CerrarPuerta();return;}
  if (conf.PH_mode==1){AbrirPuerta();return;}
  //if (conf.PH_mode==2){} //auto
//  if (conf.PH_mode==3){//manual
//     return;
//    } 

  
/*
 ////a verificar el tipo de control de la puerta
  if (hour() > conf.a && hour()<conf.b && spa.zenith<conf.c){
    // si estamos en el rago de hora de inicio y hora de fin
     ciclo_puerta();
  }else{AbrirPuerta();}
*/
  ///modificamos para que sea hora fija
// en conf.a un ángulo en grados (entero) p.e 8º a partir de ese
// ya no hace ciclo y permanece abierto

  if (spa.zenith < 90- conf.a  && spa.zenith<conf.c){
      ciclo_puerta();
        }else{
        if (spa.zenith<conf.c){ AbrirPuerta();}
  }
   

  if (spa.zenith>conf.c){
    // si estamos a un angulo cenital mayor del configurado (de no che) cerrar la puerta
     CerrarPuerta();
  }
}
      
      //  1.5 ms neutral
      //  1.25 ms 0 degrees
      //  1.75 ms 180 degress
      //pwm.setPWM(servonum, 0, SERVOMIN);
      
void AbrirPuerta(){
   int t_accionamiento=conf.b*1000; //tiempo maximo de mover el motor
   int t_funciona=0;
    
  if(estado_anterior){return;} //ya está abierta
  estado_anterior=1;
  estados[0]=1;
  digitalWrite(D4,LOW);
  Serial.println("Abrir puerta: ");
  DPRINT("clockwise");
      FC_abierta= digitalRead(D7);
      int t_ini_abrir=millis();
       while(FC_abierta && t_funciona<t_accionamiento){
          t_funciona=millis()-t_ini_abrir;
          yield(); // Let other things run
          FC_abierta= digitalRead(D7);
          if(FC_abierta){
            Serial.print("Abriendo : ");Serial.println(t_funciona);
            setServoPulse(0, 0.0012); //1.2 ms
          }
        }
         
    setServoPulse(0, 0.0); //parar motor
    Serial.println("Puerta abierta: ");
}
void CerrarPuerta(){
  int t_accionamiento=conf.b*1000; //tiempo maximo de mover el motor
  int t_funciona=0;
   
  if(!estado_anterior){return;} //ya está cerrada
  estado_anterior=0;
  estados[0]=0;
  digitalWrite(D4,HIGH);
  Serial.println("Cerrar puerta: ");
  DPRINT("counterclockwise");
       FC_cerrada= digitalRead(D6);
       int t_ini_cerrar=millis();
       while(FC_cerrada && t_funciona<t_accionamiento){
         t_funciona=millis()-t_ini_cerrar;
          yield(); // Let other things run
          FC_cerrada= digitalRead(D6);
          if(FC_cerrada){
            Serial.print("Cerrando : ");Serial.println(t_funciona);
            setServoPulse(0, 0.0018); //1.8 ms
          }
         }
         setServoPulse(0, 0.0); //parar motor
         Serial.println("Puerta cerrada: ");
}


void ciclo_puerta(){
  int valor3=conf.PH_max*1000*60; //tiempo de puerta abierta
  int valor4=conf.PH_min*1000*60; //tiempo de puerta cerrada

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
      CerrarPuerta();
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
      AbrirPuerta();
      //Serial.println("Puerta abierta, ciclo 1");
    }
    }
   
}

  
void LeeFinalesCarrera(){
  
  puerta_alta = digitalRead(D7);
  puerta_baja = digitalRead(D6);
 
}
