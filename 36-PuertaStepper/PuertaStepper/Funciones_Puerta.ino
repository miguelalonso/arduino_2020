/////////////////////////////////////////Puerta
 
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

  if (spa.zenith > 90- conf.a  && spa.zenith<conf.c){
    // si estamos en el rago de hora de inicio y hora de fin
     ciclo_puerta();
  }else{
    if (spa.zenith<conf.c){ AbrirPuerta();}
    
   
    }
   

  if (spa.zenith>conf.c){
    // si estamos a un angulo cenital mayor del configurado (de no che) cerrar la puerta
     CerrarPuerta();
  }
}

void AbrirPuerta(){
  if(estado_anterior){return;} //ya está abierta
  estado_anterior=1;
  estados[0]=1;
  digitalWrite(D4,LOW);
  DPRINT("Abrir puerta: ");
  DPRINT("clockwise");
       int stepsTaken=0;
       while(stepsTaken < conf.ciclos){
          yield(); // Let other things run
          FC_abierta= !digitalRead(D7);
          if(FC_abierta){
            Serial.println("Abrir puerta Nº paso: ");Serial.print(stepsTaken);
            stepper.step(-dir * 1);
          }
          stepsTaken++;
        }
         stepsTaken=0;
         digitalWrite(IN1, LOW);
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, LOW);

}
void CerrarPuerta(){
  if(!estado_anterior){return;} //ya está cerrada
  estado_anterior=0;
  estados[0]=0;
  digitalWrite(D4,HIGH);
  DPRINT("Cerrar puerta: ");
  DPRINT("counterclockwise");
       int stepsTaken=0;
       while(stepsTaken < conf.ciclos){
          yield(); // Let other things run
          FC_cerrada= !digitalRead(D8);
          if(FC_cerrada){
            Serial.println("Cerrar puerta Nº paso: ");Serial.print(stepsTaken);
            stepper.step(dir * 1);
          }
          stepsTaken++;
        }
         stepsTaken=0;
         digitalWrite(IN1, LOW);
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, LOW);
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
  puerta_baja = digitalRead(D8);
 
}
