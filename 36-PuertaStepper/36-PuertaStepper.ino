/**************************************************************************
 * 
 * ESP8266 NodeMCU stepper motor control with rotary encoder.
 * This is a free software with NO WARRANTY.
 * https://simple-circuit.com/
 *
 *************************************************************************/
 
// include Arduino stepper motor library
#include <Stepper.h>
 
// number of steps per one revolution is 2048 ( = 4096 half steps)
#define STEPS  2048
 
// stepper motor control pins
#define IN1   D0   // IN1 is connected to NodeMCU pin D1 (GPIO5)
#define IN2   D3   // IN2 is connected to NodeMCU pin D2 (GPIO4)
#define IN3   D5   // IN3 is connected to NodeMCU pin D3 (GPIO0)
#define IN4   D6   // IN4 is connected to NodeMCU pin D4 (GPIO2)


// #define IN4   D6   // IN4 is connected to NodeMCU pin D4 (GPIO2)
// #define IN4   D6   // IN4 is connected to NodeMCU pin D4 (GPIO2)

// initialize stepper library
Stepper stepper(STEPS, IN4, IN2, IN3, IN1);
  int dir=200; 
  int stepsTaken;
  int steps = 7;
  boolean FC_abierta;
  boolean FC_cerrada;
  
void setup(void)
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  stepper.setSpeed(15);  // set stepper motor speed to 10 rpm
 Serial.begin(9600);
 }
 

// main loop
void loop()
{
 yield();

      
      Serial.println("clockwise");
       while(stepsTaken < steps){
          yield(); // Let other things run
          stepper.step(dir * 1);
          stepsTaken++;
      }
         stepsTaken=0;
       delay(2500); 
          
 Serial.println("counterclockwise");         
      while(stepsTaken < steps){
          yield(); // Let other things run
          stepper.step(-dir * 1);
          stepsTaken++;
      }
         stepsTaken=0;
           delay(2500);
           
//   // stepper.step( 20 * dir );
//   // step one revolution  in one direction:
//  Serial.println("clockwise");
//  stepper.step(dir);
//  delay(2500);
//
//  // step one revolution in the other direction:
//  Serial.println("counterclockwise");
//  stepper.step(-dir);
// 
 
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
 
  delay(2000);
}

void abrir_puerta(){
  
       Serial.println("clockwise");
       int stepsTaken=0;
       while(stepsTaken < steps){
          yield(); // Let other things run
          FC_abierta= digitalRead(D7);
          if(FC_abierta){
            stepper.step(dir * 1);
          }
          stepsTaken++;
        }
         stepsTaken=0;
         digitalWrite(IN1, LOW);
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, LOW);
  
}
 
// end of code.
