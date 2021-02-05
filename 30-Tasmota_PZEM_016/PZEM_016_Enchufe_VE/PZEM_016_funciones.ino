



void leer_PZEM016(){

  if((millis()-timeOld3)<10000){
    return;
  }

  //demo
    //pzem.voltage       = random(0, 100);
    //pzem.current       = random(0, 100);
    //pzem.power         = random(0, 100);
    //pzem.power2         = pzem.voltage*pzem.current;
    //pzem.energy        = random(0, 100);
    //pzem.frequency     = random(0, 100);
    //pzem.power_factor  = random(0, 100);
    //return;


  
  timeOld3=millis();
  
  uint8_t result;
  uint16_t data[6];
  yield();
  node.clearResponseBuffer();
  ESP.wdtDisable();
  result = node.readInputRegisters(0x0000, 9);
  ESP.wdtEnable(1);
  DPRINTLN("Nueva Medida:");
  DPRINTLN(result);
  if (result == node.ku8MBSuccess)
  {
    pzem.voltage       = (node.getResponseBuffer(0x00) / 10.0f);
    pzem.current       = (node.getResponseBuffer(0x01) / 1000.000f);
    pzem.power         = (node.getResponseBuffer(0x03) / 10.0f);
    pzem.power2         = pzem.voltage*pzem.current;
    pzem.energy        = (node.getResponseBuffer(0x05) / 1000.0f);
    pzem.frequency     = (node.getResponseBuffer(0x07) / 10.0f);
    pzem.power_factor  = (node.getResponseBuffer(0x08) / 100.0f);
    pzem_lect_valid=true;
    DPRINT("VOLTAGE:           ");   DPRINTLN(pzem.voltage);       // V
    DPRINT("CURRENT_USAGE:     ");   DPRINTLN(pzem.current, 3);    // A
    DPRINT("ACTIVE_POWER:      ");   DPRINTLN(pzem.power);        // W
    //DPRINT("ACTIVE_ENERGY:     ");   //DPRINTLN(pzem.energy, 3);    // kWh
    //DPRINT("FREQUENCY:         ");   //DPRINTLN(pzem.frequency);           // Hz
    //DPRINT("POWER_FACTOR:      ");   //DPRINTLN(pzem.power_factor);
    //DPRINTLN("====================================================");
  }
  else {
    DPRINTLN("fallo en lectura PZEM-016");
    pzem.voltage       = 0;
    pzem.current       = 0;
    pzem.power         = -9.99;
    pzem.power2         = -9.99;
    pzem.energy        = 0;
    pzem.frequency     = 0;
    pzem.power_factor  = 0;
    pzem_lect_valid=false;
     }  
  yield();
  
}

void resetEnergy(uint8_t slaveAddr){
  //The command to reset the slave's energy is (total 4 bytes):
  //Slave address + 0x42 + CRC check high byte + CRC check low byte.
  uint16_t u16CRC = 0xFFFF;
  static uint8_t resetCommand = 0x42;
  u16CRC = crc16_update(u16CRC, slaveAddr);
  u16CRC = crc16_update(u16CRC, resetCommand);
  DPRINTLN("Resetting Energy");
  mySerial.write(slaveAddr);
  mySerial.write(resetCommand);
  mySerial.write(lowByte(u16CRC));
  mySerial.write(highByte(u16CRC));
  delay(1000);
}




void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
  delay(0);
}

void postTransmission()
{
  delay(0);
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}
