#include <Adafruit_ADS1015.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 3;    
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

float PH;
float mV_PH;
float To;
float mV_To;

float PH_ant;
float mV_PH_ant;
float To_ant;


void setup() {
  Serial.begin(9600);
  setup_PH_45020();
  init_display();
}

void loop() {
  PH_45020();
   display_ssd1306();
}

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

void PH_45020(){
  //float multiplier = 0.1875F; /* ADS1115  @ +/- 6.144V gain (16-bit results) */
  float multiplier = 0.125F;  // ganancia 1
  int16_t results = ads.readADC_Differential_0_1();  
  mV_PH_ant=mV_PH;
  mV_PH=results * multiplier;
  PH_ant=PH;
  PH=1*mV_PH+0;

  //calibración el 22 de julio de 2020
  //y=-0.0058x+22.035
  

  int16_t results2 = ads.readADC_Differential_2_3();
 // mV_To_ant=mV_To;
  To_ant=To;
  mV_To=results2 * multiplier;
  To=mV_To;

 
}
