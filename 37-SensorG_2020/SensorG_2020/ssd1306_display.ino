#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int flip;
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
  display.println("Sensor Irradiancia 2020!");
  display.println("2020");
  display.println("3 RELES");
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
      display.print("G :");display.println(G);
      display.print("mV_G : ");display.println(mV_G);
      display.print("G2 :");display.println(G2);
      display.println(Hora);
      display.println(Fecha);
  
  }
  if (flip==1) {
       display.print("Estado D5 :");display.println(estados[0]);
      display.print("Estado D6 : ");display.println(estados[1]);
      display.print("Estado D7 :");display.println(estados[2]);
      display.print("Estado D8:");display.println(estados[3]);
      display.println(Hora);
      display.println(Fecha);

  }

  if (flip==2) {
      display.setTextSize(2);
  display.println("G :");display.println(G);    
  
  }
  if (flip==3) {
        display.setTextSize(2);
       display.println("G2 :");display.println(G2);
  
  }

  display.display(); 
}
