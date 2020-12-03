
/*
  Application:
  - Interface water flow sensor with ESP32 board.
  
  Board:
  - ESP32 Dev Module
    https://my.cytron.io/p-node32-lite-wifi-and-bluetooth-development-kit
  Sensor:
  - G 1/2 Water Flow Sensor
    https://my.cytron.io/p-g-1-2-water-flow-sensor
 */



void IRAM_ATTR  pulseCounter()
{
  pulseCount++;
}

void setup_flowmeter()
{
 

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SENSOR, INPUT_PULLUP);
  digitalWrite(SENSOR, HIGH);
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  //attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, RISING);
   sei(); // Enable interrupts
}

void loop_flowmeter()
{
  if (conf.calibracion==0){
    conf.calibracion=calibrationFactor;
  }else{
    calibrationFactor=conf.calibracion;
  }
  
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    //detachInterrupt(sensorInterrupt);
    pulse1Sec = pulseCount;
    pulseCount = 0;

    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();

    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;
    //flowMilliLitres=random(0,10);  //modo demo

    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;

    if (totalMilliLitres_ant!=totalMilliLitres){
          // Print the flow rate for this second in litres / minute
          DPRINT("Flow rate: ");
          DPRINT(int(flowRate));  // Print the integer part of the variable
          DPRINT("L/min");
          DPRINT("\t");       // Print tab space
      
          // Print the cumulative total of litres flowed since starting
          DPRINT("Output Liquid Quantity: ");
          DPRINT(totalMilliLitres);
          DPRINT("mL / ");
          DPRINT(totalMilliLitres / 1000);
          DPRINTLN("L");
          conf.totalMilliLitres=totalMilliLitres;
          totalMilliLitres_ant=totalMilliLitres;
          int doy = calculateDayOfYear(day(), month(), year());
          TotalDia[doy]=totalMilliLitres;
          DPRINT("Totaldia : ");
          DPRINTLN(TotalDia[doy]);
          }
    //attachInterrupt(sensorInterrupt, pulseCounter, RISING);
    
  }

  if(dia_anterior!=day()){
    dia_anterior=day();
    savecaudales();
    totalMilliLitres=0;
    }
  
}

void loadcaudales() {
       if(!SPIFFS.begin()){
        DPRINTLN("Card Mount Failed");
        return;
        }
        File myFile = SPIFFS.open("/caudales.txt", "r");
        myFile.read((byte *)&TotalDia, sizeof(TotalDia));
       
        myFile.close();
        DPRINTLN("leyendo caudales");
 }


void savecaudales(){
   DPRINTLN("saving caudales");
   if(!SPIFFS.begin()){
        DPRINTLN("Card Mount Failed");
        return;
    }
    File myFile = SPIFFS.open("/caudales.txt", "w");
    myFile.write((byte *)&TotalDia, sizeof(TotalDia));
    myFile.close(); 
    DPRINTLN("guardando caudales");
   
  }
