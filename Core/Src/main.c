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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/**
 * @brief Giá trị 12-bit đọc được từ cảm biến, thông qua ADC. Vậy giá trị trong khoảng [0,4095]
 * @note  - giá trị số 12-bit, trong phạm vi [0,4095]
 *        - Vẫn áp dụng được với DMA, vì DMA sẽ tự động align về 32-bit.
 * @see   HAL_ADC_Start(), HAL_ADC_PollForConversion(), HAL_ADC_GetValue()
 */
uint16_t sensor_value;

/**
 * @brief Bộ đệm dữ liệu để soạn thông diệp và gửi về máy tinh, thông qua UART
 * @note  The buffer, used to draft the message and then, send it to PC by UART
 * @see   huart1
 */
char uart_buffer[50];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
#ifdef MY_ADC_DMA
  /// Kích hoạt ADC DMA request sau lần truyền cuối cùng (Chế độ Single-ADC) và kích hoạt thiết bị ngoại vi ADC,
  /// trong đó sensor_value là địa chỉ đích, sẽ được DMA tự động copy dữ liệu từ thiết bị ngoại vi vào đó.
  /// CHÚ Ý:
  ///   - Tham số 2: là ĐỊA CHỈ của buffer chứa kết quả, do ADC chuyển đổi và được DMAC copy vào.
  ///   - Tham số 3: là số phần tử của buffer, không phải là số byte. Ví dụ uint16_t buffer[20] thì tham số này là 20.
  HAL_ADC_Start_DMA(&hadc1, &sensor_value, 1);
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

#ifdef MY_ADC_INTERRUPT
	  /// Yêu cầu ADC thực hiện chuyển đổi dữ liệu Anglog --> Digital ở kênh đã chỉ định, và dùng Interrupt.
  	  /// Lưu ý: Với thăm dò polling thì dùng hàm HAL_ADC_Start(), với thăm dò bằng ngắt thì dùng HAL_ADC_Start_IT()
	  HAL_ADC_Start_IT(&hadc1);
#endif

  while (1)
  {

#ifdef MY_ADC_POLLING    
	  /// Yêu cầu ADC thực hiện chuyển đổi dữ liệu Anglog --> Digital ở kênh đã chỉ định
	  /// Lưu ý: Với thăm dò polling thì dùng hàm HAL_ADC_Start(), với thăm dò bằng ngắt thì dùng HAL_ADC_Start_IT()
	  HAL_ADC_Start(&hadc1);

	  /// Quá trình chuyển đổi đòi hòi thời gian (tốc độ lấy mẫu).
	  /// Bởi vậy, cần đợi một chút cho tới khi thực hiện xong, nhưng không đợi quá Timeout 20ms.
	  /// ==> hàm Blocking, thực hiện vòng lặp đợi dữ liệu kiểu Polling.
	  HAL_ADC_PollForConversion(&hadc1, 20);

	  /// Lúc này dữ liệu đã sẵn sàng trong bộ ADC. Lấy về và lưu vào biến chỉ định.
	  sensor_value = HAL_ADC_GetValue(&hadc1);
#endif /* MY_ADC_POLLING */
#ifdef MY_ADC_INTERRUPT
	  /// Note: Không phải làm gì cả. Ngắt ADC sẽ tự kích hoạt chương trình con ngắt ADC_IRQHandler() để đọc dữ liệu và ghi vào biến sensor_value

	  /// Truyền về máy tính để tiện giám sát số liệu
	  sprintf(uart_buffer, "%s: %d\r\n", UART_PROMT, sensor_value);
	  HAL_UART_Transmit(&huart1, (uint8_t *)uart_buffer, strlen(uart_buffer), HAL_MAX_DELAY);
#endif

	  /// Đợi một chút
	  HAL_Delay(200);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
#if defined(MY_ADC_DMA)
/**
 * @brief Hàm sự kiện được gọi sau khi ADC chuyển đổi thành công dữ liệu
 *        và DMA hoàn tất copy dữ liệu vào đich
 * @note  ADC thực hiện với đồng hồ 48MHz, nhanh hơn nhiều so với core 12Mhz,
 * @remark Cách để tạo khung của hàm này là:
 * 		   - Ở cửa sổ Project Explorer, mở thư mục Drivers\STM32F4xx_HAL_Driver\Src
 *         - Mở file stm32f4xx_hal_adc.c
 *         - Tìm file hoặc trong cửa sổ Outline và copy khai báo hàm vào đây
 *         - Xong. Đây là dạng hàm abtract, nên chỉ cần viết đè là xong.
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){
	/// Truyền về máy tính để tiện giám sát số liệu
	sprintf(uart_buffer, "%s:: %d\r\n", UART_PROMT, sensor_value);
	HAL_UART_Transmit(&huart1, (uint8_t *)uart_buffer, strlen(uart_buffer), HAL_MAX_DELAY);
}
#endif
/* USER CODE END 4 */

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
