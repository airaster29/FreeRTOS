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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
TaskHandle_t task1_handle=NULL;
TaskHandle_t task2_handle=NULL;
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
static void task1(void *args);
static void task2(void *args);
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
    xTaskCreate((TaskFunction_t)task1,
                "task1",
                128,
                NULL,
                (UBaseType_t)1,
                (TaskHandle_t *)&task1_handle
               );
    xTaskCreate((TaskFunction_t)task2,
                "task2",
                128,
                NULL,
                (UBaseType_t)1,
                (TaskHandle_t *)&task2_handle
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
static void task1(void *args)
{
    uint8_t num1=0;
    for(;;) {
        vTaskSuspendAll();
        num1++;
        if(num1>3) {
            printf("task1 is suspend and num1=%d\r\n",num1);
            num1=0;
            xTaskResumeAll();
            vTaskSuspend(task1_handle);
        } else {
            xTaskResumeAll();
        }
        printf("task1 is running and num1=%d\r\n",num1);
        vTaskDelay(500);
    }
}

static void task2(void *args)
{
    uint8_t num2=0;
    eTaskState Task1State;
    for(;;) {
        vTaskSuspendAll();
        printf("task2 is running and num2=%d\r\n",num2);
        Task1State = eTaskGetState(task1_handle);

        if(Task1State == eSuspended) {
            num2++;
            if(num2>3) {
                num2=0;
                printf("wake up task1 and num2=%d\r\n",num2);
                //xTaskResumeAll();
                vTaskResume(task1_handle);
            }

        }
        xTaskResumeAll();
        vTaskDelay(500);
    }
}
/* USER CODE END Application */

