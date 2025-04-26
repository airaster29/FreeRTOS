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
#include "multi_button.h"
#include "buttonHandler.h"
#include "gpio.h"
#include "stdio.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern uint8_t read_button_GPIO(uint8_t button_id);
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
BaseType_t ret;
TaskHandle_t Task1_Handle = NULL;
TaskHandle_t Task2_Handle = NULL;
TaskHandle_t Task3_Handle = NULL;
TaskHandle_t stackoverflowTask_Handle = NULL;
TaskHandle_t KEY_TASK_Handle = NULL;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void Task1(void *args);
static void Task2(void *args);
static void Task3(void *args);
static void stackoverflowTask(void *args);
static void KeyTask(void *args);

uint8_t read_button_GPIO(uint8_t button_id);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
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
//		ret = xTaskCreate((TaskFunction_t)Task1,
//              (const char *)"TASK1",
//              (configSTACK_DEPTH_TYPE)128,
//              (void *)NULL,
//              (UBaseType_t)osPriorityNormal,
//              (TaskHandle_t *)&Task1_Handle);
//		if(ret == pdPASS)printf("task1 create success.\r\n");
//		else printf("task1 create fail.\r\n");

//		xTaskCreate((TaskFunction_t)Task2,
//              (const char *)"TASK2",
//              (configSTACK_DEPTH_TYPE)128,
//              (void *)NULL,
//              (UBaseType_t)osPriorityNormal1,
//              (TaskHandle_t *)&Task2_Handle);
//			if(ret == pdPASS)printf("task2 create success.\r\n");
//		else printf("task2 create fail.\r\n");

//		xTaskCreate((TaskFunction_t)Task3,
//              (const char *)"TASK3",
//              (configSTACK_DEPTH_TYPE)128,
//              (void *)NULL,
//              (UBaseType_t)osPriorityNormal,
//              (TaskHandle_t *)&Task3_Handle);
//		if(ret == pdPASS)printf("task3 create success.\r\n");
//		else printf("task3 create fail.\r\n");

    xTaskCreate((TaskFunction_t)stackoverflowTask,
                (const char *)"stackoverflowTask",
                (configSTACK_DEPTH_TYPE)16,
                (void *)NULL,
                (UBaseType_t)osPriorityNormal1,
                (TaskHandle_t *)&stackoverflowTask_Handle);

//		xTaskCreate((TaskFunction_t)KeyTask,
//              (const char *)"TASK2",
//              (configSTACK_DEPTH_TYPE)128,
//              (void *)NULL,
//              (UBaseType_t)osPriorityNormal2,
//              (TaskHandle_t *)&KEY_TASK_Handle);
//    vTaskDelete(NULL);
    /* Infinite loop */
    while (1)
    {
    }
}

/* USER CODE END StartDefaultTask */


/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
static void Task1(void *args)
{
    while (1)
    {
        taskENTER_CRITICAL();
        printf("Task1 running,Priority is osPriorityNormal\r\n");
        taskEXIT_CRITICAL();
        vTaskDelay(500);
    }
}

static void Task2(void *args)
{
    while (1)
    {
        taskENTER_CRITICAL();
        printf("Task2 running,Priority is osPriorityNormal1\r\n");
        taskEXIT_CRITICAL();
        vTaskDelay(500);
    }
}

static void Task3(void *args)
{
    while (1)
    {
        taskENTER_CRITICAL();
        printf("Task3 running,Priority is osPriorityNormal\r\n");
        taskEXIT_CRITICAL();
        vTaskDelay(500);
    }
}

static void stackoverflowTask(void *args)
{
		taskENTER_CRITICAL();
    printf("stackoverflow Task running\r\n");
		taskEXIT_CRITICAL();
    int arr[100000]= {0};
    while(1) {
		printf("stack safe\r\n");
		vTaskDelay(500);
    }
}

static void KeyTask(void *args)
{
    /*1.申请按键结构*/
    taskENTER_CRITICAL();
    struct Button btn1;
    struct Button btn2;
    uint8_t btn1_id = 0;
    uint8_t btn2_id = 1;
    /*2.初始化按键*/
    button_init(&btn1, read_button_GPIO, 0, btn1_id);
    button_init(&btn2, read_button_GPIO, 0, btn2_id);
    /*3.注册按键事件*/
    button_attach(&btn1, PRESS_DOWN, Callback_SINGLE_CLICK_btn1Handler);
    button_attach(&btn2, PRESS_DOWN, Callback_SINGLE_CLICK_btn2Handler);
    /*4.启动按键*/
    button_start(&btn1);
    button_start(&btn2);
    taskEXIT_CRITICAL();
    while (1)
    {
        /*5.设置5ms间隔的定时器循环调用后台处理函数*/
        button_ticks();
        vTaskDelay(5);
    }
}

/* USER CODE END Application */
