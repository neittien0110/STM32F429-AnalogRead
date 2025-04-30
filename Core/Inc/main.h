/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"  // Thư viện string.h để sử dụng hàm strlen()
#include "stdio.h"   // Thư viện stdio.h để sử dụng hàm sprintf()
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
//#define MY_ADC_POLLING     //Phương pháp Polling. Để sử dụng, bỏ comment dòng lệnh này và recomment các define tương tự
//#define MY_ADC_INTERRUPT     // Phương pháp Interrupt. Để sử dụng, bỏ comment dòng lệnh này và recomment các define tương tự
#define MY_ADC_DMA           // Phương pháp DMA. Để sử dụng, bỏ comment dòng lệnh này và recomment các define tương tự
                             // Tham khảo thêm: https://www.st.com/resource/en/application_note/an4031-using-the-stm32f2-stm32f4-and-stm32f7-series-dma-controller-stmicroelectronics.pdf

#if defined(MY_ADC_POLLING)
	/// Nội dung UART giúp phân biệt chương trình đang sử dụng cách thăm dò nào
	#define UART_PROMT "Polling"
#elif defined(MY_ADC_INTERRUPT)
	#define UART_PROMT "Interrupt-NoDMA"
#elif defined(MY_ADC_DMA)
	#define UART_PROMT "Interrupt-DMA"
#endif

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
