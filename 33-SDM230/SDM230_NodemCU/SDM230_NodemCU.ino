/*http://192.168.1.53:80
 *Equivalencia de pines del NodeMCU
#define D0 16
#define D1 5 // I2C Bus SCL (clock)
#define D2 4 // I2C Bus SDA (data)
#define D3 0
#define D4 2 // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO 
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3 // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)


  RS485_HalfDuplex.pde - example using ModbusMaster library to communicate
  with EPSolar LS2024B controller using a half-duplex RS485 transceiver.

  This example is tested against an EPSolar LS2024B solar charge controller.
  See here for protocol specs:
  http://www.solar-elektro.cz/data/dokumenty/1733_modbus_protocol.pdf

  Library:: ModbusMaster
  Author:: Marius Kintel <marius at kintel dot net>

  Copyright:: 2009-2016 Doc Walker

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

*/

#include <FS.h>
#include <ModbusMaster.h>
#include <SoftwareSerial.h> 


ModbusMaster node;
#define MAX485_DE      D1
#define MAX485_RE_NEG  D2
SoftwareSerial mySerial(D5, D6); // RX, TX

double voltage            = 0;
double current            = 0;
double power              = 0;
double energy             = 0;
double frequency          = 0;
double power_factor       = 0;


//SDM 230 registers
#define SDM230_VOLTAGE                      0x0000                              //V
#define SDM230_CURRENT                      0x0006                              //A
#define SDM230_POWER                        0x000C                              //W
#define SDM230_ACTIVE_APPARENT_POWER        0x0012                              //VA
#define SDM230_REACTIVE_APPARENT_POWER      0x0018                              //VAR
#define SDM230_POWER_FACTOR                 0x001E                              //
#define SDM230_PHASE_ANGLE                  0x0024                              //DEGREE
#define SDM230_FREQUENCY                    0x0046                              //Hz
#define SDM230_IMPORT_ACTIVE_ENERGY         0x0048                              //Wh
#define SDM230_EXPORT_ACTIVE_ENERGY         0x004A                              //Wh
#define SDM230_IMPORT_REACTIVE_ENERGY       0x004C                              //VARh
#define SDM230_EXPORT_REACTIVE_ENERGY       0x004E                              //VARh
#define SDM230_TOTAL_SYSTEM_POWER_DEMAND    0x0054                              //W
#define SDM230_MAXIMUM_SYSTEM_POWER_DEMAND  0x0056                              //W
#define SDM230_CURRENT_POSITIVE_POWER_DEMAND 0x0058                             //W
#define SDM230_MAXIMUM_POSITIVE_POWER_DEMAND 0x005A                             //W
#define SDM230_CURRENT_REVERSE_POWER_DEMAND 0x005C                              //W
#define SDM230_MAXIMUM_REVERSE_POWER_DEMAND 0x005E                              //W
#define SDM230_CURRENT_DEMAND               0x0102                              //Amps
#define SDM230_MAXIMUM_CURRENT_DEMAND       0x0108                              //Amps
#define SDM230_TOTAL_ACTIVE_ENERGY          0x0156                              //kwh
#define SDM230_TOTAL_REACTIVE_ENERGY        0x0158                              //kvarh
#define SDM230_CURRENT_RESETTABLE_TOTAL_ACTIVE_ENERGY   0x0180                  //Wh
#define SDM230_CURRENT_RESETTABLE_TOTAL_REACTIVE_ENERGY 0x0182                  //VARh




void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
  delay(5);
}

void postTransmission()
{
   delay(5);
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup()
{
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
  Serial.begin(9600);
  mySerial.begin(9600);
  node.begin(2, mySerial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  delay(1000);
}

bool state = true;

void loop()
{
  uint8_t result;
  uint16_t data[6];
//  yield();
//  node.clearResponseBuffer();
//  ESP.wdtDisable();
//  result = node.readInputRegisters(0x0000, 80);
//  ESP.wdtEnable(1);
//  Serial.println("Nueva Medida:");
//  Serial.println(result);
//  
    float v = readVal(SDM230_VOLTAGE);
    float f = readVal(SDM230_FREQUENCY);
    float pf = readVal(SDM230_POWER_FACTOR);
    float PHASE_ANGLE = readVal(SDM230_PHASE_ANGLE);
    
    
  Serial.print("VOLTAGE : ");Serial.println(v);
  Serial.print("FREQUENCY : ");Serial.println(f);
  Serial.print("POWER_FACTOR  : ");Serial.println(pf);
  Serial.print("PHASE_ANGLE  : ");Serial.println(PHASE_ANGLE);
  
  
  delay(2000);
  yield();
}


float readVal(uint16_t reg) {
   uint16_t arr[2];
   uint16_t registros[2];
   registros[0]=reg;
   registros[1]=reg+0x01;
   
    ESP.wdtDisable();
    uint8_t result = node.readInputRegisters(reg, 2);
    ESP.wdtEnable(10);
    
  for (int i = 0; i < 2; i++) {
    uint16_t v = node.getResponseBuffer(registros[i]);
    Serial.println(v);
    int a = 1;
    if (i == 1) {
      a = 0;
    }
    arr[a] = v;
  }
  uint32_t *val = (uint32_t *)arr;
  return convert(*val);
}

float convert(uint32_t x) {
  float y = *(float*)&x;
  return y;
}
