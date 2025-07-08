#include <Arduino_FreeRTOS.h>
#include <semphr.h>


SemaphoreHandle_t LDRSemaphore;

void TaskDigitalRead(); // tâche de lesture du digital pin
void TaskAnalogRead(); // tâche pour lire le pin analog


void setup() {
  Serial.begin(9600);
 
 
}
void loop() {
  // empty

}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/


void TaskAnalogRead1(){ // c'est une tâche
  for(;;)
  {
    int sensorValue = analogRead(A0);
    if(xSemaphoreTake(LDRSemaphore,(TickType_t)5) == pdTRUE){
      Serial.print(("Valeur de LDR1 = "));
      Serial.println(sensorValue);

      xSemaphoreGive(LDRSemaphore);
    }
    vTaskDelay(100);// A tick delay >>15ms entre les lectures pour la sensibilité 
  }
}
void TaskAnalogRead2(){ // c'est une tâche
  for(;;)
  {
    int sensorValue1 = analogRead(A1);
    if(xSemaphoreTake(LDRSemaphore,(TickType_t)5) == pdTRUE){
      Serial.print(("Valeur de LDR2 = "));
      Serial.println(sensorValue1);

      xSemaphoreGive(LDRSemaphore);
    }
    vTaskDelay(100);// A tick delay >>15ms entre les lectures pour la sensibilité 
  }
}
