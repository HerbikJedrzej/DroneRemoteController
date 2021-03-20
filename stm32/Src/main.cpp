#include "main.h"
#include "stm32f1xx_hal.h"
#include "ServoStructure.hh"
#include <AT24C.hh>
#include <LDC.hh>
#include "ADC.hh"
#include "Visualizer.hh"
#include <BatteryObserver.hh>
#include <MeasurementsPID.hh>
#include <nRF24HalfDuplex.hh>
#include <SPI.hh>
#include <I2C.hh>
#include <Directory.hh>
#include <BatteryTuner.hh>
#include <PrintRadioData.hh>
#include <SelectMode.hh>
#include <SetPIDparams.hh>
#include <SetAngleOffset.hh>
#include <SetPercentValue.hh>
#include <MeasurementsAltitude.hh>

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_tx;
DMA_HandleTypeDef hdma_i2c1_rx;
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi2_rx;
DMA_HandleTypeDef hdma_spi2_tx;
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim6;
UART_HandleTypeDef huart1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM6_Init(void);

#include "InterruptFunctions.hh"

uint16_t adcData[5];

void functionCalledWhenExceptionIsThrowed(const char* text, unsigned int size){
  Drivers::gpio.pin(OutputList::LED_Red2, false);
  HAL_Delay(500);
  Drivers::gpio.pin(OutputList::LED_Red2, true);
  HAL_Delay(500);
  HAL_UART_Transmit(&huart1, (uint8_t*)"\n\r", 2, 10);
  HAL_UART_Transmit(&huart1, (uint8_t*)text, size, 100);
}

int main(void)
{
  Drivers::DriverIfc::loopsToWait = 16; // 10us of delay
  // Drivers::DriverIfc::loopsToWait = 8; // when togle pin with delay edges fall and rise every exaclly 10us (10us = sleep + gpio operation)

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_TIM6_Init();

  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
  LDC lcd(Drivers::gpio, OutputList::LDC_RS, OutputList::LDC_RW, OutputList::LDC_E, (GPIOC->ODR), HAL_Delay);
  lcd(1, 0);lcd.write((uint8_t*)" Remote control ", 16);
  lcd(2, 0);lcd.write((uint8_t*)"   created by   ", 16);
  lcd(3, 0);lcd.write((uint8_t*)" Jedrzej Herbik ", 16);
  TreeCell<ScreenCellIfc>* root = new TreeCell<ScreenCellIfc>(nullptr, nullptr);
  const bool initMode = Drivers::gpio.pin(InputList::OptFly);
  bool altitudeMeasurementsTrigger = false;
  Drivers::nRF24HalfDuplex radio(7, 0b10101110, Drivers::time, initMode, &Drivers::spi2, &Drivers::gpio, OutputList::RadioCE, OutputList::RadioCSN, InterruptInputList::RadioIRQ, HAL_Delay);
  Drivers::Memory memory(&Drivers::i2c1, &Drivers::gpio, 0xA0, OutputList::MemoryWriteProtect, HAL_Delay);
  BatteryObserver battery(&memory, adcData[ADClist::battery], Drivers::time, HAL_Delay);
  Drivers::RadioParser radioParser(&radio, Drivers::RadioMode::RemoteControl);
  {
    root->add(CreateBatteryScreenDirectory(root, (LCD_ifc*)&lcd, &battery, &radioParser));
    root->add(CreateSetPIDsParams(root, (LCD_ifc*)&lcd, &radioParser, &memory));
    root->add(new SelectMode((LCD_ifc*)&lcd, &radioParser, &memory));
    root->add(CreateMeasurementsAltitudeScreenDirectory(root, (LCD_ifc*)&lcd, &radioParser, altitudeMeasurementsTrigger, Drivers::time));
    root->add(CreateMeasurementsPIDScreenDirectory(root, (LCD_ifc*)&lcd, &radioParser, &memory, Drivers::time));
    root->add(new PrintRadioData((LCD_ifc*)&lcd, &radio, &radioParser));
    root->add(new SetAngleOffset((LCD_ifc*)&lcd, &radioParser, &memory));
    root->add(new SetPercentVal((LCD_ifc*)&lcd, &radioParser));
  }
  radio.init();
  Visualizer visual((LCD_ifc*)&lcd, root, HAL_Delay);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcData, 5);
  uint32_t pastCNT = TIM1->CNT;
	HAL_Delay(75);
  Drivers::gpio.pin(OutputList::LED_Green, true);
	HAL_Delay(75);
  Drivers::gpio.pin(OutputList::LED_Blue1, true);
	HAL_Delay(75);
  Drivers::gpio.pin(OutputList::LED_Blue2, true);
	HAL_Delay(75);
  Drivers::gpio.pin(OutputList::LED_Yellow1, true);
	HAL_Delay(75);
  Drivers::gpio.pin(OutputList::LED_Yellow2, true);
	HAL_Delay(75);
  Drivers::gpio.pin(OutputList::LED_Yellow3, true);
	HAL_Delay(75);
  Drivers::gpio.pin(OutputList::LED_Red1, true);
	HAL_Delay(75);
  Drivers::gpio.pin(OutputList::LED_Red2, true);
	HAL_Delay(75);
  // battery.clearMemory();
  Drivers::gpio.pin(OutputList::LED_Red2, battery.init());
  visual.init();
	memory.unlockMemory();
  while (1)
  {
	  if(Drivers::doCircle){
		  Drivers::doCircle = false;
      if(Drivers::time % 5 == 0)
        Drivers::gpio.pin(OutputList::LED_Green, !Drivers::gpio.pin(OutputList::LED_Green));
      Drivers::gpio.pin(OutputList::LED_Yellow1, !radio.isAckCorrect());
      Drivers::gpio.pin(OutputList::LED_Yellow2, !radio.isKyeCorrect());
      Drivers::gpio.pin(OutputList::LED_Yellow3, !radio.isComunicationCorrect());
      radioParser.run();
      // mainExecutedWithFrequency(Drivers::time, Drivers::gpio, &radio, &radioParser, lcd, adc, );
      // if(! Drivers::gpio.pin(InputList::JoyRight))
      //   battery.correctResistor1whenPowerSupplayis12V();
      // if(! Drivers::gpio.pin(InputList::SwitchLeft))
      //   battery.saveMinBat();
      if(! Drivers::gpio.pin(InputList::JoyLeft))
        radioParser.setJoyLeftButtonOption();
      if(! Drivers::gpio.pin(InputList::JoyRight)){
        radioParser.setJoyRightButtonOption();
        altitudeMeasurementsTrigger = true;
      }else{
        altitudeMeasurementsTrigger = false;
      }
      if(! Drivers::gpio.pin(InputList::OptSpec))
        radioParser.setSpecialButtonOption();
      if(! Drivers::gpio.pin(InputList::OptStart))
        radioParser.setEngineOnOption(true);
      if(! Drivers::gpio.pin(InputList::OptFly))
        radioParser.setFlyOnOption(true);
      if(! Drivers::gpio.pin(InputList::OptStop))
        radioParser.setEngineOnOption(false);
      radioParser.setRollValue(int(adcData[ADClist::axisRX] >> 4) -126);
      radioParser.setPitchValue(int(adcData[ADClist::axisRY] >> 4) -126);
      radioParser.setYawlValue(int(adcData[ADClist::axisLX] >> 4) -126);
      radioParser.setAltitudeIncremetionValue(int(adcData[ADClist::axisLY] >> 4) -126);

      if(! Drivers::gpio.pin(InputList::SwitchLeft))
        visual.Ok();
      if(! Drivers::gpio.pin(InputList::SwitchRight))
        visual.Change();
      if(! Drivers::gpio.pin(InputList::Encoder))
        visual.Cancel();
      if( ((int)pastCNT - (int)TIM1->CNT) > 0 )
        visual.Up();
      else if( ((int)pastCNT - (int)TIM1->CNT) < 0 )
        visual.Down();
      visual.run(Visualizer::StateOfOperation::waiting, radio.isAckCorrect(), radio.isKyeCorrect(), battery.batteryPercentVal(), radio.getRx(1));

      HAL_ADC_Stop_DMA(&hadc1);
      HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcData, 5);
		  pastCNT = TIM1->CNT;
	  }
	  HAL_Delay(1);
  }
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(true){}
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    while(true){}
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    while(true){}
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = ENABLE;
  hadc1.Init.NbrOfDiscConversion = 5;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 5;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    while(true){}
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    while(true){}
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    while(true){}
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    while(true){}
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    while(true){}
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    while(true){}
  }

}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    while(true){}
  }

}

/* SPI2 init function */
static void MX_SPI2_Init(void)
{

  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    while(true){}
  }

}

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_Encoder_InitTypeDef sConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 15;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 15;
  if (HAL_TIM_Encoder_Init(&htim1, &sConfig) != HAL_OK)
  {
    while(true){}
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    while(true){}
  }

}

/* TIM6 init function */
static void MX_TIM6_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 4;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 47999;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    while(true){}
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    while(true){}
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

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
    while(true){}
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LDC_RS_Pin|LDC_RW_Pin|LDC_E_Pin|LDC_0_Pin 
                          |LDC_1_Pin|LDC_2_Pin|LDC_3_Pin|LDC_4_Pin 
                          |LDC_5_Pin|LDC_6_Pin|LDC_7_Pin|LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RADIO_CSN_Pin|LED7_Pin|LED6_Pin|RADIO_CE_Pin 
                          |MEMORY_WP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED5_Pin|LED4_Pin|LED3_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED8_GPIO_Port, LED8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LDC_RS_Pin LDC_RW_Pin LDC_E_Pin LDC_0_Pin 
                           LDC_1_Pin LDC_2_Pin LDC_3_Pin LDC_4_Pin 
                           LDC_5_Pin LDC_6_Pin LDC_7_Pin LED1_Pin */
  GPIO_InitStruct.Pin = LDC_RS_Pin|LDC_RW_Pin|LDC_E_Pin|LDC_0_Pin 
                          |LDC_1_Pin|LDC_2_Pin|LDC_3_Pin|LDC_4_Pin 
                          |LDC_5_Pin|LDC_6_Pin|LDC_7_Pin|LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : JOY2SW_Pin SW2_Pin SW3_Pin */
  GPIO_InitStruct.Pin = JOY2SW_Pin|SW2_Pin|SW3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RADIO_CSN_Pin LED7_Pin LED6_Pin RADIO_CE_Pin 
                           MEMORY_WP_Pin */
  GPIO_InitStruct.Pin = RADIO_CSN_Pin|LED7_Pin|LED6_Pin|RADIO_CE_Pin 
                          |MEMORY_WP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : SW6_Pin SW4_Pin SW5_Pin */
  GPIO_InitStruct.Pin = SW6_Pin|SW4_Pin|SW5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : RADIO_IRQ_Pin */
  GPIO_InitStruct.Pin = RADIO_IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RADIO_IRQ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ENCODER_SW_Pin JOY1SW_Pin SW1_Pin */
  GPIO_InitStruct.Pin = ENCODER_SW_Pin|JOY1SW_Pin|SW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LED5_Pin LED4_Pin LED3_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED5_Pin|LED4_Pin|LED3_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LED8_Pin */
  GPIO_InitStruct.Pin = LED8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED8_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char *, int)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
