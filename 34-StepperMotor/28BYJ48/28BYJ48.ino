/*
  Creado: Luis del Valle (ldelvalleh@programarfacil.com
  Utilización librería Steper con motor 28BYJ-48
  https://programarfacil.com
*/
 
// Incluímos la librería para poder utilizarla
#include <Stepper.h>
 
// Esto es el número de pasos por revolución
#define STEPS 4096 
// Número de pasos que queremos que de
#define NUMSTEPS 512
 
// Constructor, pasamos STEPS y los pines donde tengamos conectado el motor
Stepper stepper(STEPS, D1, D2, D5, D6);
 
void setup() {
  // Asignamos la velocidad en RPM (Revoluciones por Minuto)
  stepper.setSpeed(1);
  Serial.begin(9600);
}
 
void loop() {
  // Movemos el motor un número determinado de pasos
  Serial.println("Avanza y espera 5s:");
  stepper.step(NUMSTEPS);
  
  delay(5000);
  Serial.println("Retrocede y espera 5s:");
  stepper.step(-NUMSTEPS);
  delay(5000);
  yield();
}
