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

boolean puerta_alta, puerta_baja, CO2_activado_timer;


///servo
#include <Servo.h> //this adds code to our program that defines what a Servo object is and can do

Servo servo; // create a servo object to control a servo

int mspeed = 1500; // variable to store the motor speed

const int windowPhysicalLength = 120;
const int windowMargin = 4;
const int windowLength = windowPhysicalLength - (2 * windowMargin);

int currentPosition = -1;
int targetPosition = -1;

int previousPosition = -1;

unsigned long onePercentDuration;
unsigned long previousPositionTime;


//// GPIO where the DS18B20 is connected to
//const int oneWireBus = 3;    
//OneWire oneWire(oneWireBus);
//DallasTemperature sensors(&oneWire);
//Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
