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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
	False = 0,
	True = 1
}type_bool;

typedef enum
{
	inactive = 0,
	active   = 1
}type_active_inactive;

typedef struct
{
	uint32_t t_initial;
	uint32_t t_atual;
	uint32_t t_lapse;
}type_Soft_Timer;

typedef struct
{
	type_Soft_Timer timer;
	float duty_cycle;
	uint32_t t_period;
	uint32_t t_active;
	uint32_t t_inactive;
	type_active_inactive state;
	GPIO_TypeDef* port;
	uint16_t pin;
}type_PWM;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/*uint32_t t_initial;
uint32_t t_atual;
uint32_t t_lapse;
uint32_t t_lapse2;
uint32_t t_initial2;
uint32_t t_atual2;*/

type_Soft_Timer timer_RED, timer_BLUE;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

type_bool ST_run(type_Soft_Timer* ptimer);
void ST_initialize(type_Soft_Timer* ptimer, uint32_t tlapse);
void ST_lapse(type_Soft_Timer* ptimer);
void ST_def_lapse(type_Soft_Timer* ptimer, uint32_t tlapse);

type_PWM pwm_orange; 

type_Soft_Timer timer_PWM_update;

void PWM_run(type_PWM* pwm);

void PWM_update(type_PWM* pwm)
{
	pwm->t_active = (uint32_t)(pwm->duty_cycle * pwm->t_period);
	pwm->t_inactive = pwm->t_period - pwm->t_active;
}
	///////PWM Professor///////
	void PWM_initialize(type_PWM* pwm, GPIO_TypeDef* port, uint16_t pin, uint32_t t_period, float duty_cycle);

	void PWM_def_parameters(type_PWM* pwm, uint32_t t_period, float duty_cycle);
	///////PWM Professor///////

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
		/* USER CODE BEGIN 2 */
	
		  //HAL_TIM_Base_Start_IT(&htim2);
		  //HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
		ST_initialize(&timer_RED, 500);
		//HAL_GPIO_TogglePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin);
		ST_initialize(&timer_BLUE, 625);
		/*t_lapse = 500;
		t_lapse2 = 400;
		t_initial = HAL_GetTick();*/
	
		//////////// PWM PROFESSOR ///////////////
	
		PWM_initialize(&pwm_orange, ORANGE_LED_GPIO_Port, ORANGE_LED_Pin, 1000, 1.0f);
	
		//////////// PWM PROFESSOR ///////////////
	
		//HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_SET);

	  /* USER CODE END 2 */

	  /* Infinite loop */
	  /* USER CODE BEGIN WHILE */
		while (1)
		{
			/* USER CODE END WHILE */

			/* USER CODE BEGIN 3 */
	  
			if (ST_run(&timer_BLUE))
			{
				HAL_GPIO_TogglePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin);
				ST_lapse(&timer_BLUE);
			}
	  
			if (ST_run(&timer_RED))
			{
				HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
				ST_lapse(&timer_RED);
			}
	 
			PWM_run(&pwm_orange);
	    
		}
		/* USER CODE END 3 */
	}

	/**
	  * @brief System Clock Configuration
	  * @retval None
	  */
	void SystemClock_Config(void)
	{
		RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
		RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

		/** Configure the main internal regulator output voltage
		*/
		__HAL_RCC_PWR_CLK_ENABLE();
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

		/** Initializes the RCC Oscillators according to the specified parameters
		* in the RCC_OscInitTypeDef structure.
		*/
		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
		RCC_OscInitStruct.HSEState = RCC_HSE_ON;
		RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
		RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
		RCC_OscInitStruct.PLL.PLLM = 4;
		RCC_OscInitStruct.PLL.PLLN = 168;
		RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
		RCC_OscInitStruct.PLL.PLLQ = 4;
		if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
		{
			Error_Handler();
		}

		/** Initializes the CPU, AHB and APB buses clocks
		*/
		RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
		                            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
		RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
		RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

		if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
		{
			Error_Handler();
		}
	}

	/* USER CODE BEGIN 4 */

	type_bool ST_run(type_Soft_Timer* ptimer)
	{
		ptimer->t_atual = HAL_GetTick();
		if ((ptimer->t_atual - ptimer->t_initial) >= ptimer->t_lapse)
			return True;
		else
			return False;
	}
	void ST_initialize(type_Soft_Timer* ptimer, uint32_t tlapse)
	{
		ptimer->t_atual = HAL_GetTick();
		ptimer->t_initial = ptimer->t_atual;
		ptimer->t_lapse = tlapse;
	}
	void ST_lapse(type_Soft_Timer* ptimer)
	{
		ptimer->t_initial += ptimer->t_lapse;
	}

	void ST_def_lapse(type_Soft_Timer* ptimer, uint32_t tlapse)
	{
		ptimer->t_lapse = tlapse;
	}


	///////PWM Professor///////
	void PWM_run(type_PWM* pwm)
	{
		switch (pwm->state)
		{
		case active:
			if (ST_run(&pwm->timer))
			{
				HAL_GPIO_WritePin(pwm->port, pwm->pin, GPIO_PIN_RESET);
				ST_def_lapse(&pwm->timer, pwm->t_inactive);
				pwm->state = inactive;
			}
			break;
			pwm->timer.t_initial = HAL_GetTick();
		case inactive:
			if (ST_run(&pwm->timer))
			{
				HAL_GPIO_WritePin(pwm->port, pwm->pin, GPIO_PIN_SET);
				ST_def_lapse(&pwm->timer, pwm->t_active);
				pwm->state = active;
			}
			break;
			pwm->timer.t_initial = HAL_GetTick();
		default:
			break;
			pwm->timer.t_initial = HAL_GetTick();
		}
	}
	void PWM_initialize(type_PWM* pwm, GPIO_TypeDef* port, uint16_t pin, uint32_t t_period, float duty_cycle)
	{
		PWM_def_parameters(pwm, t_period, duty_cycle);
		pwm->port = port;
		pwm->pin = pin;
		pwm->state = active;
		HAL_GPIO_WritePin(pwm->port, pwm->pin, GPIO_PIN_SET);
		ST_initialize(&pwm->timer, pwm->t_active);
	}

	void PWM_def_parameters(type_PWM* pwm, uint32_t t_period, float duty_cycle)
	{
		if (duty_cycle <= 0.0f)
		{
			pwm->duty_cycle = 0.0f;
		}
		else if (duty_cycle >= 1.0f)
		{
			pwm->duty_cycle = 1.0f;
		}
		else
		{
			pwm->duty_cycle = duty_cycle;
		}
		pwm->t_period = t_period;
		pwm->t_active = (uint32_t)(pwm->duty_cycle * pwm->t_period);
		pwm->t_inactive = pwm->t_period - pwm->t_active;
	
	}
	///////PWM Professor///////

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
	#ifdef USE_FULL_ASSERT
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