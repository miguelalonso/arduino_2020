/*
    ************************************************************
    Modified by AlexAlx 2020
    ************************************************************
     https://github.com/alex-alx/PowerMeter-SDM220-MQTT-ESP8266-MCU1.0-/blob/a18c5c3c080d17dfa42228cc5e4f2bc7d9ab7e2a/SDM220_MQTT.ino
     
    ____ original *** esp-sdm-mqtt - patrik.mayer@codm.de _____
    SDMXXX (SDM120 / SDM220 / SDM630) series power meters from eastron over modbus
    using an esp8266 and an 3.3V RS485 tranceiver.
    This uses the SDM library by reaper7 https://github.com/reaper7/SDM_Energy_Meter
    mqtt client from https://github.com/knolleary/pubsubclient/
    Tasker from https://github.com/sticilface/Tasker
*/



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


//   float Voltage;
//   float Current;
//   float Active_Power;
//   float Apparent_Power;
//   float Reactive_Power;
//   float Power_Factor;
//   float Phase_Angle;
//   float Frequency;
//   float Import_active_energy;
//   float Export_active_energy;
//   float Import_reactive_energy;
//   float Export_reactive_energy;
//   float TOTAL_SYSTEM_POWER_DEMAND;
//   float MAXIMUM_SYSTEM_POWER_DEMAND;
//   float CURRENT_POSITIVE_POWER_DEMAND;
//   float MAXIMUM_POSITIVE_POWER_DEMAND;
//   float CURRENT_REVERSE_POWER_DEMAND;
//   float MAXIMUM_REVERSE_POWER_DEMAND;
//   float CURRENT_DEMAND;
//   float MAXIMUM_CURRENT_DEMAND;
//   float TOTAL_ACTIVE_ENERGY;
//   float TOTAL_REACTIVE_ENERGY;
//   float CURRENT_RESETTABLE_TOTAL_ACTIVE_ENERGY;
//   float CURRENT_RESETTABLE_TOTAL_REACTIVE_ENERGY;

float data[29];

float temp;

void setup() {
 
  Serial.begin(9600);                             //initialize serial
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


void meassureSDM() {
  float Voltage = sdm.readVal(SDM230_VOLTAGE);
  float Current = sdm.readVal(SDM230_CURRENT); 
  float Active_Power = sdm.readVal(SDM230_POWER);
  float Apparent_Power = sdm.readVal(SDM230_ACTIVE_APPARENT_POWER);
  float Reactive_Power= sdm.readVal(SDM230_REACTIVE_APPARENT_POWER);
  float Power_Factor = sdm.readVal(SDM230_POWER_FACTOR );                                 
  float Phase_Angle = sdm.readVal(SDM230_PHASE_ANGLE );                      
  float Frequency= sdm.readVal(SDM230_FREQUENCY);                     
//  data[8]= sdm.readVal(SDM230_IMPORT_ACTIVE_ENERGY);                       
//  data[9]=  sdm.readVal(SDM230_EXPORT_ACTIVE_ENERGY  );                       
//  data[10] = sdm.readVal(SDM230_IMPORT_REACTIVE_ENERGY);                     
//  data[11] = sdm.readVal(SDM230_EXPORT_REACTIVE_ENERGY );                      
//  data[12] = sdm.readVal(SDM230_TOTAL_SYSTEM_POWER_DEMAND);                         
//  data[13]= sdm.readVal(SDM230_MAXIMUM_SYSTEM_POWER_DEMAND );                       
//  data[14] = sdm.readVal( SDM230_CURRENT_POSITIVE_POWER_DEMAND); 
//  data[15]= sdm.readVal( SDM230_MAXIMUM_POSITIVE_POWER_DEMAND );                      
//  data[16]= sdm.readVal( SDM230_CURRENT_REVERSE_POWER_DEMAND );
//  data[17]= sdm.readVal( SDM230_MAXIMUM_REVERSE_POWER_DEMAND );
//  data[18] = sdm.readVal(SDM230_CURRENT_DEMAND);
//  data[19] = sdm.readVal(SDM230_MAXIMUM_CURRENT_DEMAND);
//  data[20]= sdm.readVal( SDM230_TOTAL_ACTIVE_ENERGY ); 
//  data[21] = sdm.readVal(SDM230_TOTAL_REACTIVE_ENERGY);   
//  data[22]= sdm.readVal(SDM230_CURRENT_RESETTABLE_TOTAL_ACTIVE_ENERGY );   
//  data[23] = sdm.readVal(SDM230_CURRENT_RESETTABLE_TOTAL_REACTIVE_ENERGY );                      

 
//  Voltage = sdm.readVal(SDM230_VOLTAGE);
//  Current = sdm.readVal(SDM230_CURRENT); 
//  Active_Power = sdm.readVal(SDM230_POWER);
//  Apparent_Power = sdm.readVal(SDM230_ACTIVE_APPARENT_POWER);
//  Reactive_Power= sdm.readVal(SDM230_REACTIVE_APPARENT_POWER);
//  Power_Factor = sdm.readVal(SDM230_POWER_FACTOR );                                 
//  Phase_Angle = sdm.readVal(SDM230_PHASE_ANGLE );                      
//  Frequency= sdm.readVal(SDM230_FREQUENCY);                     
//  Import_active_energy= sdm.readVal(SDM230_IMPORT_ACTIVE_ENERGY);                       
//  Export_active_energy=  sdm.readVal(SDM230_EXPORT_ACTIVE_ENERGY  );                       
//  Import_reactive_energy = sdm.readVal(SDM230_IMPORT_REACTIVE_ENERGY);                     
//  Export_reactive_energy = sdm.readVal(SDM230_EXPORT_REACTIVE_ENERGY );                      
//  TOTAL_SYSTEM_POWER_DEMAND = sdm.readVal(SDM230_TOTAL_SYSTEM_POWER_DEMAND);                         
//  MAXIMUM_SYSTEM_POWER_DEMAND= sdm.readVal(SDM230_MAXIMUM_SYSTEM_POWER_DEMAND );                       
//  CURRENT_POSITIVE_POWER_DEMAND = sdm.readVal( SDM230_CURRENT_POSITIVE_POWER_DEMAND); 
//  MAXIMUM_POSITIVE_POWER_DEMAND= sdm.readVal( SDM230_MAXIMUM_POSITIVE_POWER_DEMAND );                      
//  CURRENT_REVERSE_POWER_DEMAND= sdm.readVal( SDM230_CURRENT_REVERSE_POWER_DEMAND );
//  MAXIMUM_REVERSE_POWER_DEMAND= sdm.readVal( SDM230_MAXIMUM_REVERSE_POWER_DEMAND );
//  CURRENT_DEMAND = sdm.readVal(SDM230_CURRENT_DEMAND);
//  MAXIMUM_CURRENT_DEMAND = sdm.readVal(SDM230_MAXIMUM_CURRENT_DEMAND);
//  TOTAL_ACTIVE_ENERGY= sdm.readVal( SDM230_TOTAL_ACTIVE_ENERGY ); 
//  TOTAL_REACTIVE_ENERGY = sdm.readVal(SDM230_TOTAL_REACTIVE_ENERGY);   
//  CURRENT_RESETTABLE_TOTAL_ACTIVE_ENERGY= sdm.readVal(SDM230_CURRENT_RESETTABLE_TOTAL_ACTIVE_ENERGY );   
//  CURRENT_RESETTABLE_TOTAL_REACTIVE_ENERGY = sdm.readVal(SDM230_CURRENT_RESETTABLE_TOTAL_REACTIVE_ENERGY );                      
// 



//*********  JSon packaging ********************
      JSONbuffer["Voltage"]                                = String(Voltage) + " V";
      JSONbuffer["Current"]                                = String(Current)+ " A";
      JSONbuffer["Active_Power"]                           = String(Active_Power) + " W";
      JSONbuffer["Apparent_Power"]                         = String(Apparent_Power) + " VA";
      JSONbuffer["Reactive_Power"]                         = String(Reactive_Power) + " VAr";
      JSONbuffer["Power_Factor"]                           = String(Power_Factor) + "";
      JSONbuffer["Frequency"]                              =  String(Frequency) + " º";
//      JSONbuffer["Import_active_energy"]                   = data[8];// =  String(Import_active_energy) + " kWh";
//      JSONbuffer["Export_active_energy"]                   =  String(Export_active_energy) + " kWh";
//      JSONbuffer["Import_reactive_energy"]                 =  String(Import_reactive_energy) + " kVArh";
//      JSONbuffer["Export_reactive_energy"]                 =  String(Export_reactive_energy) + " kVArh";
//      JSONbuffer["TOTAL_SYSTEM_POWER_DEMAND"]              =  String(TOTAL_SYSTEM_POWER_DEMAND) + " W";
//      JSONbuffer["MAXIMUM_SYSTEM_POWER_DEMAND"]            =  String(MAXIMUM_SYSTEM_POWER_DEMAND) + " w";
//      JSONbuffer["CURRENT_POSITIVE_POWER_DEMAND"]          =  String(CURRENT_POSITIVE_POWER_DEMAND) + " W";
//      JSONbuffer["MAXIMUM_POSITIVE_POWER_DEMAND"]          =  String(MAXIMUM_POSITIVE_POWER_DEMAND) + " W";
//      JSONbuffer["CURRENT_REVERSE_POWER_DEMAND"]           =  String(CURRENT_REVERSE_POWER_DEMAND) + " W";
//      JSONbuffer["MAXIMUM_REVERSE_POWER_DEMAND"]           =  String(MAXIMUM_REVERSE_POWER_DEMAND) + " W";
//      JSONbuffer["CURRENT_DEMAND"]                         =  String(CURRENT_DEMAND) + " A";
//      JSONbuffer["MAXIMUM_CURRENT_DEMAND"]                 =  String(MAXIMUM_CURRENT_DEMAND) + " A";
//      JSONbuffer["TOTAL_ACTIVE_ENERGY"]                    =  String(TOTAL_ACTIVE_ENERGY) + " kWh";
//      JSONbuffer["TOTAL_REACTIVE_ENERGY"]                  =  String(TOTAL_REACTIVE_ENERGY) + " kVAr";
//      JSONbuffer["CURRENT_RESETTABLE_TOTAL_ACTIVE_ENERGY"]           =  String(CURRENT_RESETTABLE_TOTAL_ACTIVE_ENERGY) + "Wh";
//      JSONbuffer["CURRENT_RESETTABLE_TOTAL_REACTIVE_ENERGY"]         =  String(CURRENT_RESETTABLE_TOTAL_REACTIVE_ENERGY) + " VArh";

 
}
