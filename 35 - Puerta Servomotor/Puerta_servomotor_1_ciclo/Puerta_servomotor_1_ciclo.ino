/*
 * https://github.com/psled/esp8266-window/blob/master/esp8266-window.ino#L294
 */


#define SERVO_PIN D5
#define FULLY_OPEN_SENSOR_PIN D1
#define FULLY_CLOSE_SENSOR_PIN D2


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


void setup() {
    servo.attach(D5); // attaches the servo on pin 8 to the servo object amotor
    Serial.begin(9600);
    pinMode(FULLY_OPEN_SENSOR_PIN, INPUT);
    pinMode(FULLY_CLOSE_SENSOR_PIN, INPUT);

    // Calculate window opening / closing duration
  Serial.println("Calculating open / close duration...");
  calculateDuration();
  Serial.print("Calculated: ");
  Serial.println(onePercentDuration);
  
  
}

void loop(){

//  Serial.println("stop1");
//  servo.detach();
//  servo.write(90); // this means no movement, the servo motor is standing still
//  delay(20000);
//  
//  Serial.println("counterclockwise;SUBE");
//  //servo.write(0); //this means full speed counterclockwise movement  
//  servo.attach(D5);
//   servo.write(80); //this means full speed counterclockwise movement  
//  delay(1500);
//  servo.detach();
//
//   Serial.println("clockwise;BAJA");
// // servo.write(180); //this means full speed clockwise movement 
// servo.attach(D5);
// servo.write(100); //this means full speed clockwise movement 
//  delay(1500);
//  

//    Serial.println("OPEN");
//    bool isFullyOpen = isWindowFullyOpen();
//    delay (5000);
//    Serial.println("CLOSE");
//    bool isFullyClose = isWindowFullyClose();
//     delay (5000);

//            Serial.println("OPEN");
//            if (!isWindowFullyOpen()) {
//                openWindow();
//              }
//              while (!isWindowFullyOpen()) {
//                delay(1);
//              }
//            
//              delay (20000);
//              
//              Serial.println("CLOSE");
//               closeWindow();
//              while (!isWindowFullyClose()) {
//                delay(1);
//              }

 Serial.println("Calculating open / close duration...");
  calculateDuration();
  Serial.print("Calculated: ");
  Serial.println(onePercentDuration);
  Serial.println("Esperando 30s" );            
              delay (30000);   
 
}

void calculateDuration() {
  if (!isWindowFullyOpen()) {
    openWindow();
  }
  while (!isWindowFullyOpen()) {
    delay(1);
  }
  unsigned long calculateStart = millis();
  closeWindow();
  while (!isWindowFullyClose()) {
    delay(1);
  }
  holdWindow();
  unsigned long calculateEnd = millis();
  onePercentDuration = (calculateEnd - calculateStart) / 100;
  currentPosition = 0;
}

void holdWindow() {
  Serial.println("Hold window");
  if (servo.attached()) {
    //servo.write(90);
    servo.write(85);
    //delay(1000);
    delay(250);
    servo.detach();
    delay(1);
  }
}

void openWindow() {
  Serial.println("Opening window");
  if (!servo.attached()) {
    servo.attach(SERVO_PIN);
    delay(1);
  }
  //servo.write(0);
  //servo.write(55);
  servo.write(75);
  delay(1);
}

void closeWindow() {
  Serial.println("Closing window");
  if (!servo.attached()) {
    servo.attach(SERVO_PIN);
    delay(1);
  }
  servo.write(180);
  //servo.write(115);
  //servo.write(95);
  delay(1);
}


bool isPinHigh(int pin) {
  int val = digitalRead(pin);
  delay(1);
  bool result = val == HIGH ? true : false;
  return result;
}

bool isPinLow(int pin) {
  int val = digitalRead(pin);
  delay(1);
  bool result = val == LOW ? true : false;
  return result;
}

bool isWindowFullyOpen() {
  //bool result = !isPinHigh(FULLY_OPEN_SENSOR_PIN);
  bool result = isPinLow(FULLY_OPEN_SENSOR_PIN);
  if (result) Serial.println("Activado Final de Carrera ABIERTA");
  return result;
}

bool isWindowFullyClose() {
  //bool result = !isPinHigh(FULLY_CLOSE_SENSOR_PIN);
  bool result = isPinLow(FULLY_CLOSE_SENSOR_PIN);
  if (result) Serial.println("Activado Final de Carrera CERRADA");
  return result;
}
