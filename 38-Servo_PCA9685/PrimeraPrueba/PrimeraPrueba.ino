
//https://www.tutorialspoint.com/arduino/arduino_servo_motor.htm#:~:text=A%20Servo%20Motor%20is%20a,angular%20position%20of%20the%20shaft.

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
//#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
//#define SERVOMAX  2400 // This is the 'maximum' pulse length count (out of 4096)
//#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
//#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  delay(10);
}


// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
  //Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  //Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000000;  // convert input seconds to us
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void loop() {
      //  1.5 ms neutral
      //  1.25 ms 0 degrees
      //  1.75 ms 180 degress
      //pwm.setPWM(servonum, 0, SERVOMIN);
  
  
  Serial.println(servonum);
  Serial.println("0.5ms"); //mas rápido
  setServoPulse(0, 0.0005); 
  delay(10000);

  Serial.println(servonum);
  Serial.println("1.2ms");
  setServoPulse(0, 0.0012); 
  delay(10000);

  
  Serial.println("1.8ms"); //mas lento
   setServoPulse(0, 0.0018); 

  delay(5000);
  Serial.println("parado");
  setServoPulse(0, 0.0); 
  Serial.println("fin ciclo, espera 10 s");
  delay(10000);

}
