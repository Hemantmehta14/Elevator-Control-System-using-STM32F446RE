/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
typedef enum
{
    IDLE,
    MOVE_UP,
    MOVE_DOWN
} ElevatorState;

typedef enum
{
    DIR_NONE,
    DIR_UP,
    DIR_DOWN
} Direction;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
ElevatorState state = IDLE;
Direction direction = DIR_NONE;

uint8_t currentFloor = 1;
uint8_t targetFloor = 1;

uint8_t request[3] = {0,0,0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void ShowFloor(uint8_t floor);
void ScanButtons(void);
uint8_t HasRequest(void);
int GetNextFloor(void);
void ElevatorFSM(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void ShowFloor(uint8_t floor)
{
    HAL_GPIO_WritePin(F1_LED_GPIO_Port, F1_LED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(F2_LED_GPIO_Port, F2_LED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(F3_LED_GPIO_Port, F3_LED_Pin, GPIO_PIN_RESET);

    switch(floor)
    {
        case 1:
            HAL_GPIO_WritePin(F1_LED_GPIO_Port, F1_LED_Pin, GPIO_PIN_SET);
            break;

        case 2:
            HAL_GPIO_WritePin(F2_LED_GPIO_Port, F2_LED_Pin, GPIO_PIN_SET);
            break;

        case 3:
            HAL_GPIO_WritePin(F3_LED_GPIO_Port, F3_LED_Pin, GPIO_PIN_SET);
            break;
    }
}

void ScanButtons(void)
{
    if(HAL_GPIO_ReadPin(F1_PB_GPIO_Port, F1_PB_Pin) == GPIO_PIN_RESET)
    {
        request[0] = 1;
        HAL_Delay(200);
    }

    if(HAL_GPIO_ReadPin(F2_PB_GPIO_Port, F2_PB_Pin) == GPIO_PIN_RESET)
    {
        request[1] = 1;
        HAL_Delay(200);
    }

    if(HAL_GPIO_ReadPin(F3_PB_GPIO_Port, F3_PB_Pin) == GPIO_PIN_RESET)
    {
        request[2] = 1;
        HAL_Delay(200);
    }
}

uint8_t HasRequest(void)
{
    return (request[0] || request[1] || request[2]);
}

int GetNextFloor(void)
{
    int i;

    if(direction == DIR_UP)
    {
        for(i=currentFloor+1; i<=3; i++)
        {
            if(request[i-1])
                return i;
        }

        direction = DIR_DOWN;

        for(i=currentFloor-1; i>=1; i--)
        {
            if(request[i-1])
                return i;
        }
    }

    else if(direction == DIR_DOWN)
    {
        for(i=currentFloor-1; i>=1; i--)
        {
            if(request[i-1])
                return i;
        }

        direction = DIR_UP;

        for(i=currentFloor+1; i<=3; i++)
        {
            if(request[i-1])
                return i;
        }
    }

    else
    {
        for(i=currentFloor+1; i<=3; i++)
        {
            if(request[i-1])
            {
                direction = DIR_UP;
                return i;
            }
        }

        for(i=currentFloor-1; i>=1; i--)
        {
            if(request[i-1])
            {
                direction = DIR_DOWN;
                return i;
            }
        }
    }

    return currentFloor;
}

void ElevatorFSM(void)
{
    switch(state)
    {
        case IDLE:

            if(HasRequest())
            {
                targetFloor = GetNextFloor();

                if(targetFloor > currentFloor)
                    state = MOVE_UP;

                else if(targetFloor < currentFloor)
                    state = MOVE_DOWN;
            }

            break;

        case MOVE_UP:

            HAL_GPIO_WritePin(UP_LED_GPIO_Port, UP_LED_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(DOWN_LED_GPIO_Port, DOWN_LED_Pin, GPIO_PIN_RESET);

            HAL_Delay(1000);

            currentFloor++;

            ShowFloor(currentFloor);

            if(currentFloor == targetFloor)
            {
                request[targetFloor-1] = 0;
                HAL_GPIO_WritePin(UP_LED_GPIO_Port, UP_LED_Pin, GPIO_PIN_RESET);
                state = IDLE;
            }

            break;

        case MOVE_DOWN:

            HAL_GPIO_WritePin(UP_LED_GPIO_Port, UP_LED_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(DOWN_LED_GPIO_Port, DOWN_LED_Pin, GPIO_PIN_SET);

            HAL_Delay(1000);

            currentFloor--;

            ShowFloor(currentFloor);

            if(currentFloor == targetFloor)
            {
                request[targetFloor-1] = 0;
                HAL_GPIO_WritePin(DOWN_LED_GPIO_Port, DOWN_LED_Pin, GPIO_PIN_RESET);
                state = IDLE;
            }

            break;
    }
}
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
  /* USER CODE BEGIN 2 */
 // ShowFloor(currentFloor);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  ScanButtons();

	  ElevatorFSM();
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
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

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, F1_LED_Pin|F2_LED_Pin|UP_LED_Pin|DOWN_LED_Pin
                          |F3_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : F1_PB_Pin */
  GPIO_InitStruct.Pin = F1_PB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(F1_PB_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : F1_LED_Pin F2_LED_Pin UP_LED_Pin DOWN_LED_Pin
                           F3_LED_Pin */
  GPIO_InitStruct.Pin = F1_LED_Pin|F2_LED_Pin|UP_LED_Pin|DOWN_LED_Pin
                          |F3_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : F2_PB_Pin F3_PB_Pin */
  GPIO_InitStruct.Pin = F2_PB_Pin|F3_PB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
