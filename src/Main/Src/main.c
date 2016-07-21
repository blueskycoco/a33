#include "main.h"  

USBD_HandleTypeDef USBD_Device;
__IO ITStatus SpiReady = RESET;
__IO ITStatus UartReady = RESET;

uint8_t aRxBuffer[7];
uint8_t aTxBuffer[2068]={0};// = "****SPI - Two Boards communication based on Interrupt **** SPI Message ******** SPI Message ******** SPI Message ****";
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
#define BUFFERSIZE                       (COUNTOF(aTxBuffer) - 1)
uint8_t spi_data=0;
static void SystemClock_Config(void);
UART_HandleTypeDef UartHandle;
UART_HandleTypeDef CmdHandle;
SPI_HandleTypeDef SpiHandle;

int uart_send(unsigned char data)
{
	return HAL_UART_Transmit(&UartHandle,(uint8_t *)&data,1,100);
}
int uart_read()
{
	char data;
	HAL_UART_Receive(&UartHandle,(uint8_t *)&data,1,100);
	return data;
}
void send_spi()
{
	SpiReady=RESET;
	printf("spi_data %d \n",spi_data);
	memset(aTxBuffer,spi_data,2068);
	spi_data++;
	HAL_SPI_Transmit_IT(&SpiHandle, (uint8_t*)aTxBuffer, 2068);
	while(!SpiReady);
}
void HandleCmd(uint8_t *cmd)
{
	int crc=0;
	int i;
	printf("HandleCmd\n");
	for(i=0;i<7;i++)
		printf("%2X ",cmd[i]);
	printf("\n");
	if(cmd[0]==0xaa && cmd[6]==0x0d)
	{
		crc=cmd[0]+cmd[1]+cmd[2]+cmd[3]*256;
		if(crc==(cmd[4]+cmd[5]*256))
		{
			switch(cmd[1])
			{
				case 0x01://danci
				case 0x02://duoci
				{
					if(cmd[1]==0x01)
						printf("Once cap, jifen %d\n",cmd[2]+cmd[3]*256);
					else
						printf("Multi cap, jifen %d\n",cmd[2]+cmd[3]*256);
					HAL_Delay(cmd[2]+cmd[3]*256);
					send_spi();
				}
				break;
				case 0x03:
				{
					printf("Stop cap\n");
				}
				break;
				case 0x04:
				{
					printf("Open laser, power %d\n",cmd[2]+cmd[3]*256);
				}
				break;
				case 0x05:
				{
					printf("Close laser\n");
				}
				break;
				case 0x06:
				{
					if(cmd[2]==0x01)
						printf("Xian Zhen\n");
					else
						printf("Mian Zhen\n");
				}
				break;
				default:
					printf("unknown cmd\n");
					break;
			}
		}
		else
			printf("Rcv Cmd crc error\n");
	}
	else
		printf("Rcv Cmd unknown\n");
}
int main(void)
{
	HAL_Init();
	SystemClock_Config();
	UartHandle.Instance		 = USARTx;
	UartHandle.Init.BaudRate   = 115200;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits   = UART_STOPBITS_1;
	UartHandle.Init.Parity	  = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode 	  = UART_MODE_TX_RX;
	UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; 
	HAL_UART_DeInit(&UartHandle);
	HAL_UART_Init(&UartHandle);
	
	CmdHandle.Instance		 = USART1;
	CmdHandle.Init.BaudRate   = 115200;
	CmdHandle.Init.WordLength = UART_WORDLENGTH_8B;
	CmdHandle.Init.StopBits   = UART_STOPBITS_1;
	CmdHandle.Init.Parity	  = UART_PARITY_NONE;
	CmdHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	CmdHandle.Init.Mode 	  = UART_MODE_TX_RX;
	CmdHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; 
	HAL_UART_DeInit(&CmdHandle);
	HAL_UART_Init(&CmdHandle);
	SpiHandle.Instance               = SPIx;
	SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
	SpiHandle.Init.CLKPhase          = SPI_PHASE_2EDGE;
	SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
	SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	SpiHandle.Init.CRCPolynomial     = 7;
	SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
	SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	SpiHandle.Init.NSS               = SPI_NSS_SOFT;
	SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
	SpiHandle.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
	SpiHandle.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
	SpiHandle.Init.Mode 			 = SPI_MODE_SLAVE;
	HAL_SPI_Init(&SpiHandle);
	printf("STM32F302R8T6 A20 Init\r\n");
	HAL_UART_Receive_IT(&CmdHandle, (uint8_t *)aRxBuffer, 7);
	while (1)
	{
		if(UartReady)
		{
			HandleCmd(aRxBuffer);
			UartReady=RESET;
			HAL_UART_Receive_IT(&CmdHandle, (uint8_t *)aRxBuffer, 7);
		}
	}
}

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  RCC_PeriphClkInit;
  
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  HAL_RCCEx_GetPeriphCLKConfig(&RCC_PeriphClkInit);
  RCC_PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
  
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
  
  __HAL_RCC_PWR_CLK_ENABLE();
}
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	SpiReady=SET;
	printf("SPI Send done\n");
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle->Instance==USART1)
	{
		UartReady = SET;
		printf("Uart Read done\n");  
	}
}

void Toggle_Leds(void)
{
  static uint32_t ticks;
  
  if(ticks++ == 100)
  {
    ticks = 0;
  }  
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
  while (1)
  {
  }
}
#endif
