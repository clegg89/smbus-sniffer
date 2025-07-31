/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_gpio.h"
#include "stm32g0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "buffer.h"
#include "main.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

	/* USER CODE BEGIN 1 */

static void pushWord(uint16_t data)
{
  buffer[bufferPos] = data;
  if (++bufferPos >= I2C_BUFFER_SIZE)
  {
    bufferPos = 0;
  }
if (bufferPos == bufferStart) printf("ERROR! I2C buffer too small!\r\n"); // Buffer overflow!
}

void EXTI4_15_IRQHandler(void)
{
	static uint16_t pendingData = 0;
	static int dataLeft = 1;

	if (__HAL_GPIO_EXTI_GET_RISING_IT(SCL_IT_Pin) != 0x00u)
	{
		// SCL only configured for rising, and means data should be read
		__HAL_GPIO_EXTI_CLEAR_RISING_IT(SCL_IT_Pin);

		dataLeft--;

		pendingData <<= 1;
		pendingData |= ((SDA_IT_GPIO_Port->IDR & SDA_IT_Pin) != 0x00);

		if (dataLeft == 0)
		{
			// Done collecting data, send it off and start again
      pushWord(pendingData);

			// Start fresh
			pendingData = 0;
			dataLeft = 9;
		}
	} // Unlikely to get both at once, optimize to exit faster
	else if (__HAL_GPIO_EXTI_GET_RISING_IT(SDA_IT_Pin) != 0x00u)
	{
		__HAL_GPIO_EXTI_CLEAR_RISING_IT(SDA_IT_Pin);
		if (((SCL_IT_GPIO_Port->IDR & SCL_IT_Pin) == 0))
		{
			// Nothing interesting
			return;
		}

		// STOP
    pushWord(0x200);
	}
	else // Assume falling SDA (only thing left)
	{
		// Falling
		__HAL_GPIO_EXTI_CLEAR_FALLING_IT(SDA_IT_Pin);
		if ((SCL_IT_GPIO_Port->IDR & SCL_IT_Pin) == 0)
		{
			// Nothing interesting
			return;
		}

		// START
    pushWord(0x400);

    // Start reading data
		dataLeft = 9;
		pendingData = 0;
	}
}

extern SMBUS_HandleTypeDef hsmbus1;

void I2C1_IRQHandler(void)
{
  if (hsmbus1.Instance->ISR & (SMBUS_FLAG_BERR | SMBUS_FLAG_ARLO | SMBUS_FLAG_OVR | SMBUS_FLAG_TIMEOUT | SMBUS_FLAG_ALERT | SMBUS_FLAG_PECERR)) {
    HAL_SMBUS_ER_IRQHandler(&hsmbus1);
  } else {
    HAL_SMBUS_EV_IRQHandler(&hsmbus1);
  }

}

/* USER CODE END 1 */
