#include <Arduino_FreeRTOS.h>
#define soundPin A1
#include<semphr.h>
// define two tasks for Blink & AnalogRead
void Task1( void *pvParameters );
void Task2( void *pvParameters );
void Task3( void *pvParameters );
//void TaskAnalogRead( void *pvParameters );
TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;


int light;
int sound;
// xSemaphore = NULL;
SemaphoreHandle_t xSemaphore= xSemaphoreCreateMutex();


// the setup function runs once when you press reset or power the board
void setup() {
   pinMode(13, OUTPUT);
   
  // Now set up two tasks to run independently.
  xTaskCreate(
    Task1
    ,  (const portCHAR *)"Blink1"   // A name just for humans
    ,  128  // Stack size
    ,  NULL
    ,  3  // priority
    ,  TaskHandle_1 );

      // Now set up two tasks to run independently.
  xTaskCreate(
    Task2
    ,  (const portCHAR *)"Blink1"   // A name just for humans
    ,  128  // Stack size
    ,  NULL
    ,  1  // priority
    ,  TaskHandle_2 );


      // Now set up two tasks to run independently.
  xTaskCreate(
    Task3
    ,  (const portCHAR *)"Blink1"   // A name just for humans
    ,  128  // Stack size
    ,  NULL
    ,  2  // priority
    ,  TaskHandle_3 );

//  xTaskCreate(
//    TaskAnalogRead
//    ,  (const portCHAR *) "AnalogRead"
//    ,  128 // This stack size can be checked & adjusted by reading Highwater
//    ,  NULL
//    ,  3  // priority
//    ,  TaskHandle_1 );
//
    Serial.begin(9600);

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void Task1(void *pvParameters)  // This is a task.
{
    while(1){
    (void) pvParameters;
    if(xSemaphoreTake(xSemaphore, (TickType_t)10)==pdTRUE){
    Serial.println("Start task 1");
    sound = analogRead(A1);
    Serial.print("sound: ");
    Serial.println(sound);
    Serial.println("");
      for(int i=0;i<4;i++){
      digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(10);
      digitalWrite(13, LOW);
      delay(10);// turn the LED off by making the voltage LOW
      //vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
     }
    
    
      
    xSemaphoreGive(xSemaphore);
    vTaskDelay(60);
    }
      
    }

}


void Task2(void *pvParameters){

  if(xSemaphoreTake(xSemaphore, (TickType_t)10)==pdTRUE){

    (void) pvParameters;
    Serial.println("Start task 2");
    Serial.println("");
    for(int i=0;i<2;i++){
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(10);
    digitalWrite(13, LOW);
    delay(10);// turn the LED off by making the voltage LOW
    //vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    }

    

    xSemaphoreGive(xSemaphore);
    vTaskDelay(60);
  }
  
  
  
  }

void Task3(void *pvParameters){

  while(1){
  if(xSemaphoreTake(xSemaphore, (TickType_t)10)==pdTRUE){

    (void) pvParameters;
    Serial.println("Start task 3");
    light = analogRead(A0);
    Serial.print("light: ");
    Serial.println(light);
    Serial.println("");
    for(int i=0;i<3;i++){
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);
    digitalWrite(13, LOW);
    delay(100);// turn the LED off by making the voltage LOW
    //vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    }
    


    xSemaphoreGive(xSemaphore);
    vTaskDelay(60);
  }
  }
  
  
  }

