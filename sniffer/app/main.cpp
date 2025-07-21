#include "stm32g0xx_hal.h"

#include <array>

#include <cstdint>
#include <cstdio>
#include <cstring>

#define SCL_PIN GPIO_PIN_10
#define SCL_PORT GPIOB
#define SDA_PIN GPIO_PIN_11
#define SDA_PORT GPIOB

static uint8_t buffer[7000];
static uint16_t bufferPos = 0;
static uint16_t bufferStart = 0;

static size_t I2C_BUFFER_SIZE = std::size(buffer);

UART_HandleTypeDef huart2;
void Error_Handler(void);
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

//Stdout print on UART for printf
extern "C" int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)ch, 1, 0xFFFF);

    return ch;
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  char ch = 'a';
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF); // just transmit an 'a'


  printf("\r\n ** I2C Sniffer **\r\n");
  printf("Key: \r\n");
  printf("\e[32m Left [\e[39m: (re)START \r\n");
  printf("\e[31mRight ]\e[39m:     STOP \r\n");
  printf("\r\n");
  printf("\e[36mCyan\e[39m: Address (hex) \r\n");
  printf("\e[33m   R\e[39m:   Read from master \r\n");
  printf("\e[33m   W\e[39m:  Write   to slave \r\n");
  printf("\r\n");
  printf("\e[39mWhite\e[39m: Data (hex) \r\n");
  printf("\e[33m    A\e[39m:   ACK (acknowledged) \r\n");
  printf("\e[33m    N\e[39m:  NACK (not acknowledged -- note: this is not an error) \r\n");
  printf("\r\n Enjoy! \r\n\r\n");

  // Too many variables!
  int16_t dataLeft = 0; // The number of I2C bits left to process
  uint8_t pendingData = 0; // The current byte being processed
  uint8_t pendingACK = 0; // Whether we have received ACK for the current byte
  uint8_t pendingRW = 0; // Whether a register is being Read
  uint8_t pendingExists = 0; // Whether we need to print the data
  uint8_t waitingForRegister = 0; // Whether we have received a (Re)start condition and are expecting a register address
  uint8_t dump = 0; // Whether we are dumping data due to a TIMEOUT

  uint32_t oldTimer = HAL_GetTick();

  setbuf(stdout, NULL); // Disable flushing; This might make the code slower, but makes sure everything is sent without
  	  	  	  	  	  	// having to wait for a newline
  while (1)
  {
	  if ((bufferPos - bufferStart + I2C_BUFFER_SIZE) % I2C_BUFFER_SIZE >= 1) { // positive modulo - distance left to cover
		  if (buffer[bufferStart] == 'A') {
			  // Start condition!!!
			  printf("\e[32m[\e[39m");
			  bufferStart = (bufferStart + 1) % I2C_BUFFER_SIZE;

			  if (dataLeft > 0 && dataLeft < 8) {
				  printf("ERROR! Not enough dat %d.\r\n", dataLeft);
			  }

			  dataLeft = 9;
			  pendingData = 0;
			  waitingForRegister = 1;
		  } else if (buffer[bufferStart] == 'B') {
			  // Stop condition!!!
			  printf("\e[31m]\e[39m\r\n");
			  bufferStart = (bufferStart + 1) % I2C_BUFFER_SIZE;

			  if (dataLeft > 0 && dataLeft < 8) {
				  printf("ERROR! Not got enough dat.\r\n");
				  dataLeft = 0;
			  }
			  pendingData = 0;
		  } else {
			  if (dataLeft <= 0) {
				  printf("ERROR! Got data without start condition.\r\n");
			  }
			  if (dataLeft > 9) {
				  printf("ERROR! Too much data expected\r\n");
			  }

			  dataLeft--;

			  if (dataLeft == 0) {
				  // Read ACK byte
				  pendingACK = !buffer[bufferStart];
				  pendingExists = 1;
			  } else if (dataLeft == 1 && waitingForRegister) {
				  // Read RW byte
				  pendingRW = buffer[bufferStart];
			  } else {
				  // Read regular byte
				  pendingData = pendingData << 1 | buffer[bufferStart];
			  }

			  // Increase the circular buffer position
			  bufferStart = (bufferStart + 1) % I2C_BUFFER_SIZE;
		  }
	  }

	  if (pendingExists) {
		  // Print received data
		  if (waitingForRegister) {
			  printf("\e[36m%2x", pendingData);
		  } else {
			  printf("\e[39m%2x", pendingData);
		  }
		  printf("\e[33m");
		  if (waitingForRegister) {
			  putchar(pendingRW ? 'R' : 'W');
			  waitingForRegister = 0;
		  }
		  putchar(pendingACK ? 'A' : 'N');

		  // Reset all the values
		  pendingExists = 0;
		  pendingData = 0;
		  dataLeft = 9;

		  oldTimer = HAL_GetTick();

		  printf("\e[39m");

		  if (dump) {
			  printf("\r\n");
			  dump = 0;
		  }
	  }

	  if (HAL_GetTick() - oldTimer > 1500) {
		  printf("\r\nNo data found (TIMEOUT), dumping information...\r\n");
		  printf("Received data: 0x%X, %d bits\r\n", pendingData, 9 - dataLeft);

		  printf("SCL line: %s\e[39m\r\n", HAL_GPIO_ReadPin(SCL_PORT, SCL_PIN) ? "\e[32mHI" : "\e[31mLO");
		  printf("SDA line: %s\e[39m\r\n", HAL_GPIO_ReadPin(SDA_PORT, SDA_PIN) ? "\e[32mHI" : "\e[31mLO");

		  pendingExists = dump = 1;
	  }
  }
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = SCL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SCL_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SDA_PORT, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  // Only SDA triggers on Falling
  if (HAL_GPIO_ReadPin(SCL_PORT, SCL_PIN))
  {
    // START condition: SDA went low while SCL high
	buffer[bufferPos] = 'A';
  }
  // Don't care
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  uint8_t datum;

  if (GPIO_Pin == SCL_PIN)
  {
    // Clock triggered, bit received
    datum = HAL_GPIO_ReadPin(SDA_PORT, SDA_PIN);
  }
  else if (HAL_GPIO_ReadPin(SCL_PORT, SCL_PIN))
  {
    // STOP condition: SDA went high while SCL high
	datum = 'B';

	bufferPos = (bufferPos + 1) % 7000;
	if (bufferPos == bufferStart) {
		printf("ERROR! I2C buffer too small!\r\n");
	}
  }
  else
  {
    // else nothing interesting
	return;
  }

  buffer[bufferPos] = datum;

  bufferPos = (bufferPos + 1) % 7000;
  if (bufferPos == bufferStart) {
	printf("ERROR! I2C buffer too small!\r\n");
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

