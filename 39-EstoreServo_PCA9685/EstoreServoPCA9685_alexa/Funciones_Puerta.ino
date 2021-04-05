
void leer_boton(){
  int buttonState = digitalRead(D5);
       if (buttonState == HIGH && (millis()-timeOld3) > 1000) {
        timeOld3=millis();
        Serial.println("boton pulsado");
        if ( estados[0]){
          CerrarPuerta();
        }else{
          AbrirPuerta();
        }
      } 
   }

/////////////////////////////////////////Estore

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
  PH_ant=Cenit;
  Cenit=spa.zenith;
  To_ant=To;
  mV_To=t_abierta;
  To=estados[0];
}

 
void controlestore(boolean estado){
  if(estado){AbrirPuerta();} else{CerrarPuerta();}
}
      
      //  1.5 ms neutral
      //  1.25 ms 0 degrees
      //  1.75 ms 180 degress
      //pwm.setPWM(servonum, 0, SERVOMIN);
      
void AbrirPuerta(){
   //int t_accionamiento=conf.b*1000; //tiempo maximo de mover el motor
   int t_accionamiento=conf.a*1000;
   
   int t_funciona=0;
    
  if(estado_anterior){return;} //ya está abierta
  estado_anterior=1;
  estados[0]=1;
  digitalWrite(D4,LOW);
  Serial.println("Abrir puerta: ");
  
      FC_abierta= digitalRead(D7);
      int t_ini_abrir=millis();
       while(!FC_abierta && t_funciona<t_accionamiento){
          t_funciona=millis()-t_ini_abrir;
          yield(); // Let other things run
          FC_abierta= digitalRead(D7);
          if(!FC_abierta){
            Serial.print("Abriendo : ");Serial.println(t_funciona);
            setServoPulse(0, 0.0005); //1.2 ms
          }
              int buttonState = digitalRead(D5);
              if (buttonState == HIGH && (millis()-timeOld3) > 1000){timeOld3=millis();  break;}
        }
         
    setServoPulse(0, 0.0); //parar motor
    Serial.println("Puerta abierta: ");
}
void CerrarPuerta(){
  //int t_accionamiento=conf.b*1000; //tiempo maximo de mover el motor
    int t_accionamiento=conf.b*1000;
    int t_funciona=0;
   
  if(!estado_anterior){return;} //ya está cerrada
  estado_anterior=0;
  estados[0]=0;
  digitalWrite(D4,HIGH);
  Serial.println("Cerrar puerta: ");
  
       FC_cerrada= digitalRead(D6);
       int t_ini_cerrar=millis();
       while(!FC_cerrada && t_funciona<t_accionamiento){
         t_funciona=millis()-t_ini_cerrar;
          yield(); // Let other things run
          FC_cerrada= digitalRead(D6);
          if(!FC_cerrada){
            Serial.print("Cerrando : ");Serial.println(t_funciona);
            setServoPulse(0, 0.0018); //1.8 ms
          }
              int buttonState = digitalRead(D5);
               if (buttonState == HIGH && (millis()-timeOld3) > 1000){timeOld3=millis();  break;}
        }
         setServoPulse(0, 0.0); //parar motor
         Serial.println("Puerta cerrada: ");
}



  
void LeeFinalesCarrera(){  
  puerta_alta = digitalRead(D7);
  puerta_baja = digitalRead(D6);
//    if ( puerta_alta || puerta_baja){
//        setServoPulse(0, 0.0); //parar motor
//     }
 }
