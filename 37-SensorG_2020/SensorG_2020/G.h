//http://192.168.8.107/saveconfig?cmd=saveconfig&G_max=8&G_min=2&DevName=Acuario_G&version=2.0&EEPROM_chk=123&Latitud=40&Longitud=-3&TimeZone=0&IP_1=129&IP_2=6&IP_3=15&IP_4=28&username=f09f34e0-99d9-11ea-a67f-15e30d90bbf4&password=e0f11729e7f41a83941342db89d92e5fb08247a2&clientID=d88dfa20-c063-11ea-b767-3f1a8f1211ba&pwd=121212&a=-0.00571&b=21.92&G_mode=2

#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <OneWire.h>
#include <DallasTemperature.h>

float G;
float mV_G;
float G2;
float mV_G2;
float G3;
float mV_G3;
float G4;
float mV_G4;

//float G_objetivo = 22; //????
//float G_histeresis = 5; 

float G_ant;
float mV_G_ant;
float G2_ant;
float mV_G2_ant;
float G3_ant;
float mV_G3_ant;
float G4_ant;
float mV_G4_ant;

boolean Boya_alta, Boya_baja, CO2_activado_timer;


// GPIO where the DS18B20 is connected to
const int oneWireBus = 3;    
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

void setup_G(){
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


/////////////////////////////////////////G
 
void Sensor_G(){
  //float multiplier = 0.1875F; /* ADS1115  @ +/- 6.144V gain (16-bit results) */
  float multiplier = 0.125F;  // ganancia 1

  // para medida diferencial
//  int16_t results = ads.readADC_Differential_0_1();  
// // DPRINT("Differential: "); DPRINT(results); DPRINT("("); DPRINT(results * multiplier); DPRINTLN("mV)");
//  mV_G_ant=mV_G;
//  mV_G=results * multiplier;
//  G_ant=G;
//  G=conf.a*mV_G+conf.b;
// //Serial.println("mV_Dif_0_1 :" + String(results) + "--> Irradiancia :" + String(G));
//
//  int16_t results2 = ads.readADC_Differential_2_3();
//  // mV_G2_ant=mV_G2;
//  G2_ant=G2;
//  mV_G2=results2 * multiplier;
//  G2=mV_G2;
//  //Serial.println("mV_Dif_1_2 :" + String(results2) + "--> Canal2 :" + String(G2));

// para medida single ended

  int16_t adc0, adc1, adc2, adc3;

  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3); 
//  Serial.print("AIN0: "); Serial.println(adc0 * multiplier);
//  Serial.print("AIN1: "); Serial.println(adc1);
//  Serial.print("AIN2: "); Serial.println(adc2);
//  Serial.print("AIN3: "); Serial.println(adc3);
  //Serial.println(" ");
  
  mV_G_ant=mV_G;
  mV_G=adc0 * multiplier;
  G_ant=G;
  G=conf.a*mV_G+conf.b;

  mV_G2_ant=mV_G2;
  mV_G2=adc1 * multiplier;
  G2_ant=G2;
  G2=conf.a2*mV_G2+conf.b2;

  mV_G3_ant=mV_G3;
  mV_G3=adc2 * multiplier;
  G3_ant=G3;
  G3=conf.a3*mV_G3+conf.b3;

  mV_G4_ant=mV_G4;
  mV_G4=adc3 * multiplier;
  G4_ant=G4;
  G4=conf.a4*mV_G4+conf.b4;

}

void activarRelCO2(int e1){
  estados[0]=e1;
  e1=!e1; //en Nodem van al revés
  digitalWrite(Relay_D3_CO2,e1);
  //DPRINT("Activando Rele Co2: ");DPRINTLN(e1);
}
  
void controlG(bool estado){
  //estado viene del control Timers
  //DPRINTLN("conf.G_mode");DPRINTLN(conf.G_mode);
  if (conf.G_mode==0){activarRelCO2(0);return;}
  if (conf.G_mode==1){activarRelCO2(1);return;}
  //if (conf.G_mode==2){} //auto
  if (conf.G_mode==3){//manual
    
    return;
    } 

 ////a verificar el tipo de control del ph?
  if (G > conf.G_max+0.2){
    activarRelCO2(1);
    }
    if (G < conf.G_min-0.2  ){
     activarRelCO2(0);
    }
    if (G > conf.G_min && G < conf.G_max ){ //el G correcto, verificar control timers
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
