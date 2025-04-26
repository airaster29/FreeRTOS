/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "gpio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    normalDelay=0,
    absDelay
} delayMethod;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
TaskHandle_t taskA_handle=NULL;
TaskHandle_t taskB_handle=NULL;
TaskHandle_t taskC_handle=NULL;

uint16_t g_normalDelayTime = 1000;
uint16_t g_absDelayTime = 2000;
delayMethod g_delayMethod=normalDelay;
extern uint8_t rxBuffer[15];
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void TaskA(void *args);
void TaskB(void *args);
void TaskC(void *args);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
    vTaskSuspendAll();
    xTaskCreate((TaskFunction_t)TaskA,
                "TaskA",
                (configSTACK_DEPTH_TYPE)128,
                NULL,
                //&g_normalDelayTime,
                (UBaseType_t)1,
                &taskA_handle
               );

    xTaskCreate((TaskFunction_t)TaskB,
                "TaskB",
                (configSTACK_DEPTH_TYPE)128,
                NULL,
                (UBaseType_t)1,
                &taskB_handle
               );

    xTaskCreate((TaskFunction_t)TaskC,
                "TaskC",
                (configSTACK_DEPTH_TYPE)128,
                NULL,
                (UBaseType_t)1,
                &taskC_handle
               );
    xTaskResumeAll();
    vTaskDelete(NULL);
    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void TaskA(void *args)
{
    TickType_t xLastWakeTime;
    for(;;) {
				vTaskSuspendAll();
        printf("TaskA is running\r\n");
        if(g_delayMethod == normalDelay) {
					HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
					printf("normalDelay\r\n");
					xTaskResumeAll();
          vTaskDelay(g_normalDelayTime);
        }
        else if(g_delayMethod == absDelay){
					HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
					xLastWakeTime = xTaskGetTickCount();	//初始化
					printf("absDelay\r\n");
					xTaskResumeAll();
					vTaskDelayUntil(&xLastWakeTime,g_absDelayTime);	//等待下一个周期
				}
				//xTaskResumeAll();
    }
}

void TaskB(void *args)
{
    for(;;) {
				if(!strcmp("normalDelay",(const char*)rxBuffer)){
					vTaskSuspendAll();
					g_delayMethod = normalDelay;
					printf("TaskB normalDelay\r\n");
					xTaskResumeAll();
				} 
    }
}

void TaskC(void *args)
{
    for(;;) { 
				if(!strcmp("absDelay",(const char*)rxBuffer)){
						vTaskSuspendAll();
						g_delayMethod = absDelay;
						printf("TaskC absDelay\r\n");
						xTaskResumeAll();
				}
    }
}
/* USER CODE END Application */

