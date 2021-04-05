#include <Adafruit_ADS1015.h>
#include <OneWire.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
float Cenit;
float mV_PH;
float To;
float mV_To;
float PH_ant;
float mV_PH_ant;
float To_ant;
boolean puerta_alta, puerta_baja, CO2_activado_timer;
boolean FC_abierta;
boolean FC_cerrada;
  
