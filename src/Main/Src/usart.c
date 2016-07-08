#include "main.h"
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
	GPIO_InitTypeDef  GPIO_InitStruct;
	if(huart->Instance==USART1)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		
		__HAL_RCC_USART1_CLK_ENABLE(); 
		
		GPIO_InitStruct.Pin 	  = GPIO_PIN_6;
		GPIO_InitStruct.Mode	  = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull	  = GPIO_PULLUP;
		GPIO_InitStruct.Speed	  = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = GPIO_PIN_7;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(USART1_IRQn);

	}
	else
	{
		USARTx_TX_GPIO_CLK_ENABLE();
		USARTx_RX_GPIO_CLK_ENABLE();

		USARTx_CLK_ENABLE(); 

		GPIO_InitStruct.Pin       = USARTx_TX_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = USARTx_TX_AF;

		HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = USARTx_RX_PIN;
		GPIO_InitStruct.Alternate = USARTx_RX_AF;

		HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(USARTx_IRQn);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{	
		__HAL_RCC_USART1_FORCE_RESET();
		__HAL_RCC_USART1_RELEASE_RESET();
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
		HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
	else
	{
		USARTx_FORCE_RESET();
		USARTx_RELEASE_RESET();
		HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
		HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
		HAL_NVIC_DisableIRQ(USARTx_IRQn);
	}
}
