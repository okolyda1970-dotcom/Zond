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
#include <regEdit.h>
#include  "math.h"
#include "string.h"
#include "usart_ring.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SIZE_BF 8
#define SIZE 9
#define FPD 25
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
char strok [9] = {0};
uint8_t ld = 0;
uint8_t pData0 [4] = {0};
uint8_t pData1 [4] = {0};
uint8_t pData2 [4] = {0};
uint8_t pData3 [4] = {0};
uint8_t pData4 [4] = {0};
uint8_t pData5 [4] = {0};
uint8_t pData6 [4] = {0};


//adfStruct  tipDef_0;

extern volatile uint16_t rx_buffer_head;
extern volatile uint16_t rx_buffer_tail;
extern unsigned char rx_buffer[UART_RX_BUFFER_SIZE];


extern structR0  tipDef_0;
extern structR1  tipDef_1;
extern structR2  tipDef_2;
extern structR3  tipDef_3;
extern structR4  tipDef_4;
extern structR5  tipDef_5;
extern remote2 txRem2;
uint32_t registr0 = 0x005980C8;
uint32_t registr1 = 0x200303E9;
uint32_t registr2 = 0x80004A42;
uint32_t registr3 = 0x00000133;
uint32_t registr4 = 0x63DFF2FC;
uint32_t registr5 = 0x01C00005;
uint32_t registr6 = 0x00000006;

volatile uint32_t regRx1 = 0;
volatile uint32_t regRx2 = 0;

uint32_t registrFB = 0;
uint8_t rxData [SIZE] = {0};
 uint8_t txData [SIZE] = {0};

volatile float adcData [128] = {0.0};
uint8_t range = 0;
uint8_t rising = 0;
uint8_t flagAdc = 0;
uint16_t intValue = 0;
uint16_t fracValue = 0;
uint16_t modValue = 0;

uint8_t flag = 0;
uint16_t tick = 0;
uint8_t xPosision = 0;
uint8_t yPosision = 0;
uint8_t testCod= 0;
float fractionValue = 0.0;
float integerValue = 0.0;
float modulValue = 1000.0;
float rfdivider = 32.0;
float fwc = 0.0;
FlagStatus flagTxUart = RESET;
FlagStatus flagRxUart = RESET;
FlagStatus errorRx = RESET;
uint8_t tickTx = 0;
uint8_t tickRx = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void shift (void)
{
	registr0 = 0x330010;
	registr1 = 0x8008029;
	registr2 = 0x4E42;
	registr3 = 0x4B3;
	registr4 = 0xEC802C;
	registr5 = 0xD80005;

	registr0 = initReg0();
	registr1 = initReg1();
	registr2 = initReg2();
	registr3 = initReg3();
	registr4 = initReg4();
	registr5 = initReg5();

	  pData0[0] = 0x000000FF & (registr0 >> 24);
	  pData0[1] = 0x000000FF & (registr0 >> 16);
	  pData0[2] = 0x000000FF & (registr0 >> 8);
	  pData0[3] = 0x000000FF & (registr0);

	  pData1[0] = 0x000000FF & (registr1 >> 24);
	  pData1[1] = 0x000000FF & (registr1 >> 16);
	  pData1[2] = 0x000000FF & (registr1 >> 8);
	  pData1[3] = 0x000000FF & (registr1);

	  pData2[0] = 0x000000FF & (registr2 >> 24);
	  pData2[1] = 0x000000FF & (registr2 >> 16);
	  pData2[2] = 0x000000FF & (registr2 >> 8);
	  pData2[3] = 0x000000FF & (registr2);

	  pData3[0] = 0x000000FF & (registr3 >> 24);
	  pData3[1] = 0x000000FF & (registr3 >> 16);
	  pData3[2] = 0x000000FF & (registr3 >> 8);
	  pData3[3] = 0x000000FF & (registr3);

	  pData4[0] = 0x000000FF & (registr4 >> 24);
	  pData4[1] = 0x000000FF & (registr4 >> 16);
	  pData4[2] = 0x000000FF & (registr4 >> 8);
	  pData4[3] = 0x000000FF & (registr4);

	  pData5[0] = 0x000000FF & (registr5 >> 24);
	  pData5[1] = 0x000000FF & (registr5 >> 16);
	  pData5[2] = 0x000000FF & (registr5 >> 8);
	  pData5[3] = 0x000000FF & (registr5);

}

void shiftRx (void)
{

/*	  regRx1  = 0x000000FF & (rxData[0] << 24);
	  regRx1 = 0x000000FF & (rxData[1] >> 16);
	  regRx1 = 0x000000FF & (rxData[2] >> 8);
	  regRx1 = 0x000000FF & (rxData[3]);*/

	  regRx1 = 0;
	  regRx2 = 0;
	  regRx1 |= (rxData[0] << 24);
	  regRx1 |= (rxData[1] << 16);
	  regRx1 |= (rxData[2] << 8);
	  regRx1 |= (rxData[3]);

	  regRx2 |= (rxData[4] << 24);
	  regRx2 |= (rxData[5] << 16);
	  regRx2 |= (rxData[6] << 8);
	  regRx2 |= (rxData[7]);

}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	 flagTxUart = RESET;
}

void resetUartReceiver(void) {
    // === 1. Останавливаем DMA ===
    HAL_UART_DMAStop(&huart1);

    // === 2. Принудительно читаем RDR (очищаем флаг RXNE) ===
    volatile uint32_t dummy = USART1->RDR;
    (void)dummy;

    // === 3. Сбрасываем флаги ошибок UART ===
    __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_PE | UART_FLAG_FE | UART_FLAG_NE | UART_FLAG_ORE);

    // === 4. Очищаем буфер приёма ===
//    memset(rxData, 0, SIZE);

    // === 5. Перезапускаем приём DMA ===
    HAL_UART_Receive_DMA(&huart1, rxData, SIZE);

    // === 6. Сбрасываем флаги ===
    flagRxUart = RESET;
    flagTxUart = RESET;
    errorRx = RESET;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    LL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);

    if(flagRxUart)
    {
        USART1 -> ICR |= USART_ICR_IDLECF;
        flagRxUart = RESET;

        // Копируем данные
        txData[0] = rxData[0];
        txData[1] = rxData[1];
        txData[2] = rxData[2];
        txData[3] = rxData[3];
        txData[4] = rxData[4];
        txData[5] = rxData[5];
        txData[6] = rxData[6];
        txData[7] = rxData[7];
        txData[8] = rxData[8];

        if(rxData[8] == 0b01010101)
//            if(1)
        {
            shiftRx();
            bitOperation();
            setRegistr();

            registr0 = initReg0();
            registr1 = initReg1();
            registr2 = initReg2();
            registr3 = initReg3();
            registr4 = initReg4();
            registr5 = initReg5();
            shift();
            spiAdf();

            errorRx = RESET;
            testCod = 0;
        }
        	else {
        	    errorRx = SET;

        	    // === ОСТАНАВЛИВАЕМ UART И DMA ===
        	    HAL_UART_DMAStop(&MYUART);

        	    // === ЧИТАЕМ RDR, ЧТОБЫ ОЧИСТИТЬ ФЛАГ RXNE ===
        	    volatile uint32_t dummy = USART1->RDR;
        	    (void)dummy;

        	    // === СБРАСЫВАЕМ ФЛАГИ ОШИБОК UART ===
        	    __HAL_UART_CLEAR_FLAG(&MYUART, UART_FLAG_PE | UART_FLAG_FE | UART_FLAG_NE | UART_FLAG_ORE);

        	    // === ОЧИЩАЕМ БУФЕР ПРИЁМА ===
//        	    memset(rxData, 0, SIZE);

        	    // === ПЕРЕЗАПУСКАЕМ ПРИЁМ DMA ===
        	    HAL_UART_Receive_DMA(&MYUART, rxData, SIZE);

        	    // === СБРАСЫВАЕМ ФЛАГИ ===
        	    flagRxUart = RESET;
        	    flagTxUart = RESET;
        	}

        if(flagTxUart == RESET && huart->gState == HAL_UART_STATE_READY) {
            HAL_UART_Transmit_IT(&MYUART, txData, SIZE);
            flagTxUart = SET;
        }

        tick = 0;
    }
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
        if (huart == &huart1)
        {
                uint32_t er = HAL_UART_GetError(&huart1);

                if (er & HAL_UART_ERROR_PE)
                {

                        __HAL_UART_CLEAR_PEFLAG(&huart1);
                }
                if (er & HAL_UART_ERROR_NE)
                {

                        __HAL_UART_CLEAR_NEFLAG(&huart1);
                }
                if (er & HAL_UART_ERROR_FE)
                {

                        __HAL_UART_CLEAR_FEFLAG(&huart1);
                }
                if (er & HAL_UART_ERROR_ORE)
                {

                        __HAL_UART_CLEAR_OREFLAG(huart);
                }
                if (er & HAL_UART_ERROR_DMA)
                {

                        __HAL_UART_CLEAR_NEFLAG(&huart1);
                }
                huart->ErrorCode = HAL_UART_ERROR_NONE;
      		  flagRxUart = RESET;
    		  flagTxUart = RESET;

        }
}

void tickUart (void)
{
	  if(flagTxUart && tickTx < 10)
	  {
		  tickTx++;
	  }
	  else
	  {
		  flagTxUart = RESET;
		  tickTx = 0;
	  }
	  if(flagRxUart == SET && tickRx < 10)
	  {
		  tickRx++;
	  }
	  else
	  {
		  flagRxUart = RESET;
		  tickRx = 0;
	  }
}


void setAtt (void)
{

	 if(txRem2.attenuator1 == 1)
	 {
		 LL_GPIO_SetOutputPin(ATT_1_GPIO_Port, ATT_1_Pin);
	 }
	 else{
		 LL_GPIO_ResetOutputPin(ATT_1_GPIO_Port, ATT_1_Pin);
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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
   defoultSet ();
   LL_GPIO_SetOutputPin(CE_1_GPIO_Port, CE_1_Pin);
   LL_GPIO_SetOutputPin(RF_OUT_EN_GPIO_Port, RF_OUT_EN_Pin);
   LL_GPIO_SetOutputPin(LE_1_GPIO_Port, LE_1_Pin);
  //  memcpy (pData, &registr, SIZE);
   LL_mDelay(100);
//	tipDef_0.integ = 89;
//	tipDef_4.bandSelectclockDividerValue = 0b100;
//	tipDef_4.RFdividerSelect = 0b100;
	   float minF = 800.0;
	//   float maxF = 2500.0;
	   fwc = minF;
/*	   shift();
	   spiAdf ();*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
/*	   registr0 = initReg0 ();
	   registr1 = initReg1 ();
	   registr2 = initReg2 ();
	   registr3 = initReg3 ();
	   registr4 = initReg4 ();
	   registr5 = initReg5 ();
	   shift();
	   spiAdf ();
		 LL_GPIO_SetOutputPin(ATT_1_GPIO_Port, ATT_1_Pin);*/
  while (1)
  {

	  // проверочный код
/*	  LL_mDelay(500);
		LL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
		if( tipDef_5.LDpinMode == 0b11)
		{
			tipDef_5.LDpinMode = 0b00;
		}
		else{
			tipDef_5.LDpinMode = 0b11;
		}

		   registr0 = initReg0 ();
		   registr1 = initReg1 ();
		   registr2 = initReg2 ();
		   registr3 = initReg3 ();
		   registr4 = initReg4 ();
		   registr5 = initReg5 ();
		   shift();
		   spiAdf ();*/

	  // рабочий код
	  if(flagRxUart == RESET)
		{
		  flagRxUart = SET;
/*		  rxData [0] = 0;
		  rxData [1] = 0;
		  rxData [2] = 0;
		  rxData [3] = 0;
		  rxData [4] = 0;
		  rxData [5] = 0;
		  rxData [6] = 0;
		  rxData [7] = 0;*/
//			MYUART.Instance->CR1 |= USART_ISR_RXNE;

		  if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE) == SET)
		  {
			  if(__HAL_UART_GET_FLAG(&huart1,USART_ISR_RXNE) == SET )
			  {
				  USART1 -> ISR &= USART_ISR_RXNE;
//				  HAL_UART_AbortReceive(&huart1);
//			  registrFB = MYUART.Instance->RDR;
				  HAL_UART_Receive_DMA(&huart1, rxData, SIZE);
			  }
		  }

         }
	  else
	  {
		  tickUart();
	  }

//		LL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);




/*	   if(fwc < maxF)
	   {
		   fricvancy (fwc);
		   fwc += 1;
	   }
	   else
	   {
		   fwc = minF;
	   }*/


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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_SetSystemCoreClock(8000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_EVEN;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
  huart1.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
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
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(LED_1_GPIO_Port, LED_1_Pin);

  /**/
  LL_GPIO_ResetOutputPin(RF_OUT_EN_GPIO_Port, RF_OUT_EN_Pin);

  /**/
  LL_GPIO_ResetOutputPin(ATT_1_GPIO_Port, ATT_1_Pin);

  /**/
  LL_GPIO_ResetOutputPin(CE_1_GPIO_Port, CE_1_Pin);

  /**/
  LL_GPIO_ResetOutputPin(LE_1_GPIO_Port, LE_1_Pin);

  /**/
  GPIO_InitStruct.Pin = LED_1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED_1_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = RF_OUT_EN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(RF_OUT_EN_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = ATT_1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(ATT_1_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CE_1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CE_1_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LE_1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LE_1_GPIO_Port, &GPIO_InitStruct);

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
