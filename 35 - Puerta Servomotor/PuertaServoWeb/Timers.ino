void randomTimers(){
    //crea las variables aleatorias una vez al dia
    int ds=weekday() - 2; //(1=domingo) (1~7)
    
    char str[6];
            
           int mm = 60.0 * (spa.sunrise - (int)(spa.sunrise));
            sprintf(str, "%02d:%02d",  (int)(spa.sunrise),(int)mm);
            conf.sunrise=(int)spa.sunrise*100+(int)mm;
            conf.sunrise+=conf.TimeZone;
            mm = 60.0 * (spa.sunset - (int)(spa.sunset));
            sprintf(str, "%02d:%02d",  (int)(spa.sunset),(int)mm);
            conf.sunset=(int)spa.sunset*100+(int)mm;
            conf.sunset+=conf.TimeZone;
   
     if (ds==ds_anterior){
      return;
     }
    ds_anterior=ds;
    // timers[nr].Tiempo_procesado[7];
    
    for (i=0;i<Num_reles;i++){
      for (j=0;j<7;j++){
                    int signo;
                    if(timers[i].sign_delay[j]>0){
                      signo=-1;
                    }else
                    {
                      signo=1;
                      }
                    timers[i].Tiempo_procesado[j]=timers[i].ON_Time[j];
                   if(timers[i].sun_set[j]==1 ){ //en este caso es un delay
                        timers[i].Tiempo_procesado[j]=conf.sunrise+signo*timers[i].delay_time[j];
                     }
                    if( timers[i].sun_set[j]==2){
                        timers[i].Tiempo_procesado[j]=conf.sunset+signo*timers[i].delay_time[j];
                    }
                   if(timers[i].aleatorio_Time[j]!=0){
                          int aleat=random(-timers[i].aleatorio_Time[j],timers[i].aleatorio_Time[j]);
                          timers[i].Tiempo_procesado[j]+=aleat;
                   }
      }
    }
     DPRINTLN("Random timers :");
     DPRINTLN(timers[0].Tiempo_procesado[0]);
}


void controlTimers(int j){ 
  timeNow = (100 * hour()) + minute();
  boolean salida=false;
  
  //j es numero de rele 0 a 3
  //i de 0 a 6 --> 7 programas

   if (timers[j].Mode == 0) {
    salida = false;
    control_rele(j,0);
    return;
  }
  
  if (timers[j].Mode == 1) {
    salida = true;
    control_rele(j,1);
    return;
  }
  
  // Is mode invalid or man=3
  if (timers[j].Mode != 2) {
    if(j==0){ controlPH(0);} //control puerta control en manual da igual 0 que 1, se controla por ph
    return;
  }

   if (time_old[j] != timeNow) {
      time_old[j] = timeNow;
      randomTimers();
         int ds=weekday() - 2; //(1=domingo) (1~7)
         if (ds<0){ds=6;} //si ds=-1  es domingo dia 6
          int pactivo=0;
          byte p=0;
          byte d=0;
          int tiempoMayor=0;
          int estadoMayor=0;

//            int temp = 0;
//            int temp2=0;
//             //Calculate length of array arr  
//            int length = sizeof(timers[j].Tiempo_procesado)/sizeof(timers[j].Tiempo_procesado[0]);  
//            //Sort the array in ascending order  
//                for (int i = 0; i < length; i++) {   
//                    for (int k = i+1; k < length; k++) {   
//                       if(timers[j].Tiempo_procesado[i] > timers[j].Tiempo_procesado[k]) {  
//                           temp = timers[j].Tiempo_procesado[i];
//                           temp2 = timers[j].onoff[i];  
//                           timers[j].Tiempo_procesado[i] = timers[j].Tiempo_procesado[k];
//                           timers[j].onoff[i]=timers[j].onoff[k];  
//                           timers[j].Tiempo_procesado[k] = temp;
//                           timers[j].onoff[k]=temp2;  
//                       }   
//                    }   
//                }  
//    


                for ( p = 0; p < 7; p++) {
                  if(timers[j].ON_Days[p][ds] == true){
                  if (timers[j].Tiempo_procesado[p]>tiempoMayor){
                      tiempoMayor=timers[j].Tiempo_procesado[p];
                      estadoMayor=timers[j].onoff[p];
                      if (timeNow >= timers[j].Tiempo_procesado[p]) {salida=timers[j].onoff[p];}
                    }
                  }
                }

                  for ( p = 0; p < 7; p++) {
                   if(ds==0 && timers[j].ON_Days[p][6] == true){// es lunes, hay que mirar los valores del domingo
                      if (timers[j].onoff[p] >0){salida=true;}    
                      if (timers[j] .onoff[p] <=0){salida=false;} 
                    }
                  }

          
          for ( d = 0; d <= ds; d++) { //hasta ds dia semana de hoy
             for ( p = 0; p < 7; p++) {
              if (timers[j].ON_Days[p][d] == true) { //el programa está activo ya que hay un dia activado
                  pactivo++;
                
                  if(d<ds){
                    if (timers[j].onoff[p] >0){salida=true;}    
                    if (timers[j].onoff[p] <=0){salida=false;} 
                  }
//                 if(ds==0 && timers[j].ON_Days[p][6] == true){// es lunes, hay que mirar los valores del domingo
//                    if (timers[j].onoff[p] >0){salida=true;}    
//                    if (timers[j] .onoff[p] <=0){salida=false;} 
//                  }
                  if (d==ds && timeNow >= timers[j].Tiempo_procesado[p]) {
                    if (timers[j].onoff[p] >0){salida=true; DPRINTLN("--->salida true");}    
                    if (timers[j].onoff[p] <=0){salida=false;DPRINTLN("--->salida false");} 
                  }
              }
            }//d
      } //p

//si pactivo==0 entonces no hay ningún programa activo hasta el dia de hoy
// hay que mirar los dias siguientes comenzando por el domingo



  long t_anterior=0;
  p=0;
  d=0;
  
    if(pactivo==0){
      DPRINTLN("buscando desde el domingo");
              for ( d = 6; d > 0; d--) { //comineza en domingo y baja
                  for ( p = 0; p < 7; p++) {
                  if (timers[j].ON_Days[p][d] == true) {
                     pactivo++;
                      if(timers[j].ON_Time[p]>t_anterior ){ 
                        t_anterior = timers[j].ON_Time[p];
                        if (timers[j].onoff[p] >0){salida=true;DPRINTLN("ppp--salida true");}    
                        if (timers[j].onoff[p] <=0){salida=false;DPRINTLN("ppp--salida false");} 
                      }
                     }
                }//p
                break; //ya se encontro un timer anterior activado y salimos.
        } //d
      }
        
       if (pactivo>0){ //si no hay ningún dia seleccionado dejar en manual
          control_rele(j,salida);
//          DPRINT("------------------------->Programa activo: "); DPRINTLN(pactivo);
//          DPRINT("Control rele "); DPRINT(j);DPRINT(":");
//          DPRINTLN(salida); 
        }
        
      DPRINT("control timers rele :");
      DPRINT(j);
      //DPRINT(" dia semana :");
      //DPRINTLN(ds);
      DPRINT(" Salida: ");
      DPRINTLN(salida);
      DPRINT(" ds: ");DPRINT(ds);DPRINT(" timeNow: ");DPRINT(timeNow);
      DPRINT(" p: ");DPRINT(p);
      DPRINT(" timers[0].ON_Time[0]: ");DPRINTLN(timers[0].ON_Time[0]);
      DPRINT(" timers[j].Tiempo_procesado[p]: ");DPRINTLN(timers[j].Tiempo_procesado[p]);
      DPRINT(" timers[0].onoff[0]: ");DPRINTLN(timers[0].onoff[0]);
      DPRINT(" timers[0].onoff[p]: ");DPRINTLN(timers[0].onoff[p]);
      
      if (j==0 && pactivo==0){pactivo_ph=0;control_rele(j,salida);} //CO2 siempre controla
      if (j==0 && pactivo>0){pactivo_ph=1;}
      //control cada minuto
      }//timeold
   
  
  
  
}
  
void control_rele(int j, int Output){
  timers[j].Estado_rele=Output;
  estados[j]=Output;
  //Output=!Output; //en el NodemCu van al revés...
  
  //digitalWrite(GPIOPIN[j],Output);
   switch( j){
    case 0: // La puerta se controla por timers o por configuración
    //Serial.print("Control puerta timers"); Serial.println(Output);
      controlPH(Output);
      break;
    case 1:
     digitalWrite(Relay_D3,!Output);
     break;
    case 2:
     digitalWrite(Relay_D4,!Output);
     break;
    case 3:
     digitalWrite(Relay_D8,!Output);
     break;
   }
}
