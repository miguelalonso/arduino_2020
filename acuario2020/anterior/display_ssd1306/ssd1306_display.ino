#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

boolean flip;
long time_display=2000;
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
          
  
  flip=!flip;
  
  display.clearDisplay();
 display.setTextSize(1);
  if (flip) {
  display.setTextColor(WHITE);
  }else{
    display.setTextColor(INVERSE);
  }
  
  PH=random(0,12);
  mV_PH=random(0,3300);
  mV_To=random(0,3300);
  To=random(-12,45);
  
  display.setCursor(0, 0);
  display.println("192.168.1.180");
  display.print("Millis :");display.println(millis());
  display.print("D4 :");display.println("ON");
  display.print("Millis : ");display.println(millis());
  display.print("PH :");display.println(PH);
  display.print("mV_PH :");display.println(mV_PH);
  display.print("mV_To :");display.println(mV_To);
  display.print("T0 :");display.println(To);
  display.display(); 
}
