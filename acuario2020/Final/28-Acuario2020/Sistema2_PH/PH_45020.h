//http://192.168.8.107/saveconfig?cmd=saveconfig&PH_max=8&PH_min=2&DevName=Acuario_PH&version=2.0&EEPROM_chk=123&Latitud=40&Longitud=-3&TimeZone=0&IP_1=129&IP_2=6&IP_3=15&IP_4=28&username=f09f34e0-99d9-11ea-a67f-15e30d90bbf4&password=e0f11729e7f41a83941342db89d92e5fb08247a2&clientID=d88dfa20-c063-11ea-b767-3f1a8f1211ba&pwd=121212&a=-0.00571&b=21.92&PH_mode=2

#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <OneWire.h>
#include <DallasTemperature.h>

float PH;
float mV_PH;
float To;
float mV_To;
//float PH_objetivo = 22; //????
//float PH_histeresis = 5; 

float PH_ant;
float mV_PH_ant;
float To_ant;

boolean Boya_alta, Boya_baja, CO2_activado_timer;


// GPIO where the DS18B20 is connected to
const int oneWireBus = 3;    
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

void setup_PH_45020(){
        // Comenzamos la lectura del sensor de ph a traves del ADS1115
        sensors.begin();
        // The ADC input range (or gain) can be changed via the following
        // functions, but be careful never to exceed VDD +0.3V max, or to
        // exceed the upper and lower limits if you adjust the input range!
        // Setting these values incorrectly may destroy your ADC!
        //                                                                ADS1015  ADS1115
        //                                                                -------  -------
        // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
           ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
        // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
        // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
        // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
        // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
        ads.begin();
}


/////////////////////////////////////////PH
 
void PH_45020(){
  //float multiplier = 0.1875F; /* ADS1115  @ +/- 6.144V gain (16-bit results) */
  float multiplier = 0.125F;  // ganancia 1
  int16_t results = ads.readADC_Differential_0_1();  
 // DPRINT("Differential: "); DPRINT(results); DPRINT("("); DPRINT(results * multiplier); DPRINTLN("mV)");
  mV_PH_ant=mV_PH;
  mV_PH=results * multiplier;
  PH_ant=PH;
  PH=conf.a*mV_PH+conf.b;

  //calibración el 22 de julio de 2020
  //y=-0.0058x+22.035
  

  int16_t results2 = ads.readADC_Differential_2_3();
 // mV_To_ant=mV_To;
 To_ant=To;
  mV_To=results2 * multiplier;
  To=mV_To;
}

void activarRelCO2(int e1){
  estados[0]=e1;
  e1=!e1; //en Nodem van al revés
  digitalWrite(Relay_D3_CO2,e1);
  //DPRINT("Activando Rele Co2: ");DPRINTLN(e1);
}
  
void controlPH(bool estado){
  //estado viene del control Timers
  //DPRINTLN("conf.PH_mode");DPRINTLN(conf.PH_mode);
  if (conf.PH_mode==0){activarRelCO2(0);return;}
  if (conf.PH_mode==1){activarRelCO2(1);return;}
  //if (conf.PH_mode==2){} //auto
  if (conf.PH_mode==3){//manual
    
    return;
    } 

 ////a verificar el tipo de control del ph?
  if (PH > conf.PH_max+0.2){
    activarRelCO2(1);
    }
    if (PH < conf.PH_min-0.2  ){
     activarRelCO2(0);
    }
    if (PH > conf.PH_min && PH < conf.PH_max ){ //el PH correcto, verificar control timers
      if(estado>0 && pactivo_ph==0){activarRelCO2(1);}else{activarRelCO2(0);}
      //DPRINT("ph en rango"); DPRINTLN(estado);
    }
    
}

void activarRelleno(int e1){

  if (Boya_alta){
     digitalWrite(Relay_D4_bomba,1); // si ya está lleno, parar la bomba
     estados[4]=e1;
     return;
    }
      estados[4]=e1;
      e1=!e1; //en Nodem van al revés
      digitalWrite(Relay_D4_bomba,e1);
  }
  
void controlRelleno(){
  
  Boya_alta = digitalRead(Input_D3_boya_alta);
  Boya_baja = digitalRead(Input_D5_boya_baja);

  //PULLUP - boya abierta  ==> se lee un 1
  //boya_alta==1 es que el acuario está lleno
  //boya_baja==1 es que hay agua en el depósito
  
  if (conf.Relleno_mode==0){activarRelleno(0);return;}
  if (conf.Relleno_mode==1){activarRelleno(1);return;}
  //if (conf.relleno_mode==2){} //auto
  if (conf.Relleno_mode==3){return;} //manual
 
    if (Boya_alta){
      activarRelleno(0);
    }else{
      activarRelleno(1);
      }
    
    if (!Boya_baja){
     activarRelleno(0);
    }
    if (Boya_alta && Boya_baja){ //hay un error, las dos activadas?
      activarRelleno(0);
    }
}
