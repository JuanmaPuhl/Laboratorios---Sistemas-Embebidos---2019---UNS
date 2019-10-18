#include "Arduino_FreeRTOS.h"
#include "semphr.h"

// definimos las tareas
static void TaskProductor(void* pvParameters);
static void TaskConsumidor(void* pvParameters);

// definimos los semaforo contadores
SemaphoreHandle_t xFillCount = NULL;
SemaphoreHandle_t xEmptyCount = NULL;
// definimos el mutex utilizado para bloquear al recurso (puerto serie en este caso)
SemaphoreHandle_t xMutex = NULL;

void setup() {

    // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);

    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
    }

    // creamos la tarea del primer productor
    xTaskCreate(
        TaskProductor
        ,  (const portCHAR *)"ProductorUno"   // A name just for humans
        ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
        ,  NULL
        ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,  NULL );

    // creamos la tarea del segundo productor
    xTaskCreate(
        TaskProductor
        ,  (const portCHAR *)"ProductorDos"   // A name just for humans
        ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
        ,  NULL
        ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,  NULL );

    // creamos la tarea del consumidor
    xTaskCreate(
        TaskConsumidor
        ,  (const portCHAR *)"Consumidor"   // A name just for humans
        ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
        ,  NULL
        ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,  NULL );

    // creamos los semaforos y el mutex
    xFillCount = xSemaphoreCreateCounting(32, 0); //inicialmente no hay elementos producidos, fill=0
    xEmptyCount = xSemaphoreCreateCounting(32, 32); //inicialmente no hay elementos producidos, empty=32
    xMutex = xSemaphoreCreateMutex();
}

void loop() {

}

// task productor
static void TaskProductor(void* pvParameters) {

    // recuperamos el nombre del task
    char* pcName = pcTaskGetName(NULL);

    while (1) {
        // si hay lugar para producir
        if (xSemaphoreTake(xEmptyCount, portMAX_DELAY) == pdTRUE)
        {
            // si podemos acceder al recurso (puerto serie)
            if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
                Serial.println(pcName);
                xSemaphoreGive(xMutex);
            }
            // aumentamos la cantidad producida
            xSemaphoreGive(xFillCount);
        }
        else
        {
            // no hay lugar para producir
        }
    }
}

// task consumidor
static void TaskConsumidor(void* pvParameters) {

    // recuperamos el nombre del task
    char* pcName = pcTaskGetName(NULL);

    while (1) {
        // si hay elementos para consumir
        if (xSemaphoreTake(xFillCount, portMAX_DELAY) == pdTRUE)
        {
            // si podemos acceder al recurso (puerto serie)
            if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
                Serial.println(pcName);
                xSemaphoreGive(xMutex);
            }

            // decrementamos la cantidad de elementos
            xSemaphoreGive(xEmptyCount);
        }
        else
        {
            // no hay nada que consumir
        }
    }
}
