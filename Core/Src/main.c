/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "gpio.h"
#include "freertos.h"
#include "task.h"
#include "usart.h"
#include "queue.h"
#include "sys.h"
#include "delay.h"
#include "oled.h"
#include "string.h"
#include "stdio.h"
#include "bsp_raindrop.h"
#include "dht22.h"
#include "bh1750.h"
#include "my_iic.h"
#include "bmp280.h"
#include "dwt_delay.h"
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
// LED ��ʼ������
TaskHandle_t task1handle;
TaskHandle_t task2handle;
TaskHandle_t task3handle;
TaskHandle_t task4handle;
QueueHandle_t queue;

void rain_task(void *arg)
{
   while (1)
   {
        bsp_raindrop_update_display();//雨滴

        bsp_raindrop_get_percent();
     vTaskDelay(pdMS_TO_TICKS(500)); // ��ʱ500ms
   }
}

void dht22_task(void *arg)
{
   while (1)
   {
			dht22_update_display();//温湿度 
       vTaskDelay(pdMS_TO_TICKS(500)); // 延时500ms
   }
}

void bh1750_task(void *arg)
{
   while (1)
   {
       bh1750_update_display();//光照
       vTaskDelay(pdMS_TO_TICKS(500)); // 延时500ms
   }
}

void bmp280_task(void *arg)
{
   while (1)
   {
       bmp280_update_display();//气压
       vTaskDelay(pdMS_TO_TICKS(500)); // 延时500ms
   }
}

int main(void)
{
    HAL_Init(); // ��ʼ��HAL��
    // led_init(); // ��ʼ��LED
    SystemClock_Config();
    MX_USART2_UART_Init();
    
    // 初始化DWT计数器，用于精确延时
    dwt_delay_init();
    
    oled_init();
    oled_fill(0x00);

     /* DHT22 init */
    dht22_init();
    my_iic_init();
    bmp280_init();
    bsp_raindrop_analog_init();
    bh1750_init();
    delay_ms(100); // 确保其他传感器初始化完成//   
		xTaskCreate(rain_task, "rain_task", 256, NULL, 1, &task1handle);
    xTaskCreate(dht22_task, "dht22_task", 256, NULL, 1, &task2handle);
	
    xTaskCreate(bh1750_task, "bh1750_task", 128, NULL, 1, &task3handle);
    xTaskCreate(bmp280_task, "bmp280_task", 128, NULL, 1, &task4handle);

    vTaskStartScheduler();

    while (1)
    {
    }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }

  if(htim->Instance == TIM3)
    {
      print("TIM3 interrupt: setting dht22_sample_flag\n");
      dht22_sample_flag=1; // 设置采样标志
    }

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
