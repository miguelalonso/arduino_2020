/*
    ************************************************************
    Modified by AlexAlx 2020
    ************************************************************
     
    ____ original *** esp-sdm-mqtt - patrik.mayer@codm.de _____
    SDMXXX (SDM120 / SDM220 / SDM630) series power meters from eastron over modbus
    using an esp8266 and an 3.3V RS485 tranceiver.
    This uses the SDM library by reaper7 https://github.com/reaper7/SDM_Energy_Meter
    mqtt client from https://github.com/knolleary/pubsubclient/
    Tasker from https://github.com/sticilface/Tasker
*/

// MQTT_MAX_PACKET_SIZE : Maximum packet size
#undef  MQTT_MAX_PACKET_SIZE // un-define max packet size
#define MQTT_MAX_PACKET_SIZE 512  // fix for MQTT client dropping messages over 128B

#include <SDM.h>
#include <SoftwareSerial.h> 
#include <ArduinoJson.h>





// ************  Modbus RS485 settings ************
const int rxPin = D5;//12;
const int txPin = D6;//13;
const int derePin = D1;//4;
const long baud = 4800;

SoftwareSerial swSerSDM;                                            //config SoftwareSerial
SDM sdm(swSerSDM, baud, derePin, SWSERIAL_8N1, rxPin, txPin);       //config SDM
StaticJsonDocument <512> JSONbuffer;

   float Voltage;
   float Current;
   float Active_Power;
   float Apparent_Power;
   float Reactive_Power;
   float Power_Factor;
   float Phase_Angle;
   float Frequency;
   float Import_active_energy;
   float Export_active_energy;
   float Import_reactive_energy;
   float Export_reactive_energy;
   float TOTAL_SYSTEM_POWER_DEMAND;
   float MAXIMUM_SYSTEM_POWER_DEMAND;
   float CURRENT_POSITIVE_POWER_DEMAND;
   float MAXIMUM_POSITIVE_POWER_DEMAND;
   float CURRENT_REVERSE_POWER_DEMAND;
   float MAXIMUM_REVERSE_POWER_DEMAND;
   float CURRENT_DEMAND;
   float MAXIMUM_CURRENT_DEMAND;
   float TOTAL_ACTIVE_ENERGY;
   float TOTAL_REACTIVE_ENERGY;
   float CURRENT_RESETTABLE_TOTAL_ACTIVE_ENERGY;
   float CURRENT_RESETTABLE_TOTAL_REACTIVE_ENERGY;
   
void setup() {
  Serial.begin(9600);                                                         //initialize serial
  Serial.println("Starting...");

  sdm.begin();



}

void loop() {

  meassureSDM();
  Serial.println("Medida...");
  serializeJson(JSONbuffer, Serial);
  Serial.println("_____________...");
  delay(5000);



}

//void meassureSDM(int)
void meassureSDM() {
  float v = sdm.readVal(SDM230_VOLTAGE);                                       //read voltage [V]
  float c = sdm.readVal(SDM230_CURRENT);                                       //read current [A]
  float p = sdm.readVal(SDM230_POWER);                                         //read power [W]
  float f = sdm.readVal(SDM230_FREQUENCY);                                     //read frequency [Hz}
  float pf = sdm.readVal(SDM230_POWER_FACTOR);                                 //read Power Factor []
  float pa = sdm.readVal(SDM230_PHASE_ANGLE);                                  //read Phase Angle [Deg]
  float aap = sdm.readVal(SDM230_ACTIVE_APPARENT_POWER);                       //read Active Apparent Power [VA]
  float rap = sdm.readVal(SDM230_REACTIVE_APPARENT_POWER);                     //read Reactive Apparent Power [VAR]
  float iae = sdm.readVal(SDM230_IMPORT_ACTIVE_ENERGY);                        //read Import Active Energy [Wh]
  float eae = sdm.readVal(SDM230_EXPORT_ACTIVE_ENERGY);                        //read Export Active Energy [Wh]
  float ire = sdm.readVal(SDM230_IMPORT_REACTIVE_ENERGY);                      //read Import Active Energy [VARh]
  float ere = sdm.readVal(SDM230_EXPORT_REACTIVE_ENERGY);                      //read Export Active Energy [VARh]
  float tae = sdm.readVal(SDM230_TOTAL_ACTIVE_ENERGY);                         //read Export Active Energy [Wh]
  float tre = sdm.readVal(SDM230_TOTAL_REACTIVE_ENERGY);                       //read Export Active Energy [VARh]  

                    
                        
                       
  float tspd = sdm.readVal(SDM230_TOTAL_SYSTEM_POWER_DEMAND);                         
  float mspd= sdm.readVal(SDM230_MAXIMUM_SYSTEM_POWER_DEMAND );                       
  float cppd = sdm.readVal( SDM230_CURRENT_POSITIVE_POWER_DEMAND); 
  float mppd= sdm.readVal( SDM230_MAXIMUM_POSITIVE_POWER_DEMAND );                      
  float crpd= sdm.readVal( SDM230_CURRENT_REVERSE_POWER_DEMAND );
  float mrpd= sdm.readVal( SDM230_MAXIMUM_REVERSE_POWER_DEMAND );
  float cd = sdm.readVal(SDM230_CURRENT_DEMAND);
  float mcd = sdm.readVal(SDM230_MAXIMUM_CURRENT_DEMAND);
  
  float crtae= sdm.readVal(SDM230_CURRENT_RESETTABLE_TOTAL_ACTIVE_ENERGY );   
  float crtre = sdm.readVal(SDM230_CURRENT_RESETTABLE_TOTAL_REACTIVE_ENERGY );   


//*********  JSon packaging ********************
  
  JSONbuffer["Voltage"] = v;
  JSONbuffer["Current"] =  c;
  JSONbuffer["Active_Power"] =  p;
  JSONbuffer["Freq"] =  f;
  JSONbuffer["PowerFactor"] =  pf;
  JSONbuffer["PhaseAngle"] =  pa;
  JSONbuffer["ActApparentPwr"] =  aap;
  JSONbuffer["RctApparentPwr"] =  rap; 
  JSONbuffer["ImpActEnergy"] =  iae;  
  JSONbuffer["ExpActEnergy"] =  eae; 
  JSONbuffer["ImpRctEnergy"] =  ire;  
  JSONbuffer["ExpRctEnergy"] =  ere;
  JSONbuffer["TotActEnergy"] =  tae;
  JSONbuffer["TotRctEnergy"] =  tre;

   JSONbuffer["TotSysPowDemand"] =  tspd;
   JSONbuffer["MaxSysPowDemand"] =  mspd;
   JSONbuffer["CurPosPowDemand"] =  cppd;
   JSONbuffer["MaxPosPowDemand"] =  mppd;
   JSONbuffer["CurRevPowDemand"] =  crpd;
   JSONbuffer["MaxRevPowDemand"] =  mrpd;
   JSONbuffer["CurDemand"] =  cd;
   JSONbuffer["MaxCurDemand"] =  mcd;
   JSONbuffer["CurResTotActEn"] =  crtae;
   JSONbuffer["CurResTotReactEn"] =  crtre;
 
  

}
