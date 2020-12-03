#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int flip=0;
long time_display=8000;
long time_display_ant;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void init_display(){
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(5000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Hello, world!");
  display.display(); 
}

void display_ssd1306(){
  
     if((millis()-time_display_ant )<time_display){
          return;
         }
      time_display_ant =millis();
      display.setTextColor(WHITE); 
         
      flip++;if(flip>3){flip=0;}
      
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println(WiFi.localIP());
      display.print("Millis :");display.println(millis());
 
  if (flip==0) {
      display.print("Act_Power :");display.println(sdm230.Active_Power);
      display.print("Inv : ");display.println(inversor.Power);
      display.print("Bomb :");display.println(bombeo.Power);
      display.print("aire.Power:");display.println(aire.Power);
      display.print("PowFact :");display.println(sdm230.PowerFactor);
      display.print("Sunrise :");display.println(conf.sunrise);
      display.print("Sunset :");display.println(conf.sunset);
      display.println(hora);
      display.println(Fecha);
  
  }
  if (flip==1) {
      display.print("Act_Power :");display.println(sdm230.Active_Power);
      display.print("Inv : ");display.println(inversor.Power);
      display.print("Sunrise :");display.println(conf.sunrise);
      display.print("Sunset :");display.println(conf.sunset);
      display.println(hora);
      display.println(Fecha);
  
  }

  if (flip==2) {
      display.setTextSize(2);
      if (lect_valid){
        display.print("Act_Power :");display.println(sdm230.Active_Power);
     
      }else{
        display.println("Error");
      }
  
  }
  if (flip==3) {
      display.setTextSize(2);
      
      display.print("Inv : ");display.println(inversor.Power);
      display.print("Bomb :");display.println(bombeo.Power);
 
  }

  display.display(); 
}
