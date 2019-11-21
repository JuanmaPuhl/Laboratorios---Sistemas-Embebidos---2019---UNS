#include "Arduino_FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

static void prenderLED(void* pvParameters);
static void apagarLED(void* pvParameters);
static TaskHandle_t xTask1 = NULL, xTask2 = NULL;

void setup()
{
  xTaskCreate(prenderLED , (const portCHAR*)"prenderLED" , 256 , NULL , 3 , &xTask1);
  xTaskCreate(apagarLED , (const portCHAR*)"apagarLED" , 256 , NULL , 3 , &xTask2);
  DDRB |= (1 << DDB5);
  xTaskNotifyGive(xTask1);
  vTaskStartScheduler();
}

void loop()
{
  // put your main code here, to run repeatedly:
}

static void prenderLED(void* pvParameters)
{
  TickType_t xLastWakeTime;
  while(1)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xLastWakeTime = xTaskGetTickCount();
    PORTB |= (1 << DDB5);
    vTaskDelayUntil(&xLastWakeTime, (1000 / portTICK_PERIOD_MS));
    xTaskNotifyGive(xTask2);
  }
}

static void apagarLED(void* pvParameters)
{
  TickType_t xLastWakeTime;
  while(1)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xLastWakeTime = xTaskGetTickCount();
    PORTB &= ~(1 << DDB5);
    vTaskDelayUntil(&xLastWakeTime, (1000 / portTICK_PERIOD_MS));
    xTaskNotifyGive(xTask1);
  }
}
