
void leer_boton(){
  int buttonState = digitalRead(D5);
       if (buttonState == HIGH && (millis()-timeOld3) > 1000) {
          t_funciona=0;
          digitalWrite(D6,LOW); // D6  abre toldo
          digitalWrite(D7,LOW);  // D7  cierra toldo
        timeOld3=millis();
        Serial.println("Boton pulsado");

        if( abriendo_toldo || cerrando_toldo){
          abriendo_toldo=0;
          cerrando_toldo=0;
          estados[0]=!estados[0];
          estado_anterior=!estado_anterior;
          return;
         }
          
        if ( estados[0] ){
          CerrarToldo(0);
        }else{
          AbrirToldo(0);
        }
      } 
   }

/////////////////////////////////////////Toldo1

 

void PuertaServoWeb(){
  mV_PH_ant=ciclo_actual;
  mV_PH=millis()/1000;
  PH_ant=Cenit;
  Cenit=spa.zenith;
  To_ant=To;
  mV_To=t_abierta;
  To=estados[0];
}

 
void controlToldo(boolean estado){
  if(estado){AbrirToldo(0);} else{CerrarToldo(0);}
}
      
void AbrirToldo_parcial(int brightness){
    if(brightness>0){
      if(estado_anterior){
           t_accionamiento=brightness*conf.a*1000/255;
        } else{
            t_accionamiento=brightness*conf.b*1000/255;
        }
    }else{
        if(estado_anterior){
           t_accionamiento=conf.a*1000; //tiempo maximo de mover el motor
        } else{
            t_accionamiento=conf.b*1000; //tiempo maximo de mover el motor
        }
    }
    
  if(estado_anterior){
    CerrarToldo(t_accionamiento); 
  } else{
    AbrirToldo(t_accionamiento);
  }

}
     
void AbrirToldo(int t_accionamiento){
  //int t_accionamiento=conf.a*1000; //tiempo maximo de mover el motor
  if(t_accionamiento==0){
      t_accionamiento=conf.a*1000;
    }
  if(estado_anterior){return;} //ya está abierta
  Serial.print("Abrir Toldo: ");Serial.println(t_funciona);

     if(!abriendo_toldo){
      Serial.print("Abriendo Toldo : ");
      abriendo_toldo=1;
      digitalWrite(D4,LOW);
      digitalWrite(D6,HIGH); // D6  abre toldo
      digitalWrite(D7,LOW);  // D7  cierra toldo
      t_ini_abrir=millis();
     }
     
      t_funciona=millis()-t_ini_abrir;
      
       if(t_funciona>t_accionamiento){
          abriendo_toldo=0;
          Serial.print("Toldo Abierto en (ms) : ");Serial.println(t_funciona);
          estado_anterior=1;
          estados[0]=1;
          t_funciona=0;
          digitalWrite(D6,LOW); // D6  abre toldo
          digitalWrite(D7,LOW);  // D7  cierra toldo
          }
    
}

void CerrarToldo(int t_accionamiento){
  
  if(t_accionamiento==0){
      t_accionamiento=conf.b*1000;
    }
   if(!estado_anterior){return;} //ya está cerrada
    Serial.print("Cerrar Toldo: ");Serial.println(t_funciona);

     if(!cerrando_toldo){
      Serial.print("Cerrando Toldo : ");
      cerrando_toldo=1;
      digitalWrite(D4,HIGH);
      digitalWrite(D6,LOW); // D6  abre toldo
      digitalWrite(D7,HIGH);  // D7  cierra toldo
      t_ini_cerrar=millis();
     }
     
      t_funciona=millis()-t_ini_cerrar;
      
       if(t_funciona>t_accionamiento){
          cerrando_toldo=0;
          Serial.print("Toldo cerrado en (ms) : ");Serial.println(t_funciona);
          estado_anterior=0;
          estados[0]=0;
          t_funciona=0;
          digitalWrite(D6,LOW); // D6  abre toldo
          digitalWrite(D7,LOW);  // D7  cierra toldo
          }
    
}



  
